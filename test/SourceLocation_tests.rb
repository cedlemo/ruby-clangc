#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestSourceLocation < MiniTest::Test
    include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_SourceLocation_is_in_system_header
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    source_location = source_range.start
    assert_equal false, source_location.is_in_system_header 
  end
  def test_SourceLocation_is_from_main_file
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    assert_equal true, source_location.is_from_main_file
  end
  def test_SourceLocation_equal
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location_start = diagnostics[1].source_ranges.last.start
    source_location_end = diagnostics[1].source_ranges.last.end
    source_location_end_1 = diagnostics[1].source_ranges.last.end
    assert_equal false, source_location_start.is_equal(source_location_end)
    assert_equal true, source_location_end.is_equal(source_location_end_1)
  end
  def test_SourceLocation_spelling
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    spelling = source_location.spelling
    # file
    assert_equal true, tu.file(SOURCE_FILE_TWO_ERRORS).is_equal(spelling[0])
    # line
    assert_equal 14, spelling[1]
    # column
    assert_equal 1, spelling[2]
    #offset
    assert_equal 179, spelling[3]
  end
  def test_SourceLocation_spelling_end
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    spelling = source_location.spelling
    # file
    assert_equal false, tu.file(SOURCE_FILE_TWO_ERRORS).is_equal(spelling[0])
    # line
    assert_equal 0, spelling[1]
    # column
    assert_equal 0, spelling[2]
    #offset
    assert_equal 0, spelling[3]
  end
  def test_SourceLocation_file_location
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    file_location = source_location.file_location
    # file
    assert_equal true, tu.file(SOURCE_FILE_TWO_ERRORS).is_equal(file_location[0])
    # line
    assert_equal 14, file_location[1]
    # column
    assert_equal 1, file_location[2]
    #offset
    assert_equal 179, file_location[3]
  end
  def test_SourceLocation_file_location_end
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS, CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    file_location = source_location.file_location
    # file
    assert_equal false, tu.file(SOURCE_FILE_TWO_ERRORS).is_equal(file_location[0])
    # line
    assert_equal 0, file_location[1]
    # column
    assert_equal 0, file_location[2]
    #offset
    assert_equal 0, file_location[3]
  end
  def test_SourceLocation_file_location_start_end
  tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE  
        if cursor.kind == Clangc::CursorKind::STRUCT_DECL 
          range = cursor.extent
          _file, start_line, start_column = range.start.file_location
          _file, stop_line, stop_column = range.end.file_location
          assert_equal([3, 9], [start_line, start_column], range.start.spelling)
          assert_equal([6, 4], [stop_line, stop_column], range.end.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
