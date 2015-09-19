#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestSourceRange < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_SourceRange_is_null_true
    source_range = Clangc.null_source_range
    assert_equal true, source_range.is_null
  end
  def test_SourceRange_is_null_false
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS,CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_ranges = diagnostics[1].source_ranges
    assert_equal false, source_ranges[range_number - 1].is_null
  end
  def test_SourceRange_is_equal_true
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS,CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]
    source_range_2 = diagnostics[1].source_ranges[range_number - 1]
    assert_equal true, source_range_1.is_equal(source_range_2)
    assert_equal false, source_range_1 == source_range_2
  end
  def test_SourceRange_is_equal_false
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_THREE_ERRORS,CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    
    range_number = diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]

    range_number = diagnostics[2].num_ranges
    source_range_2 = diagnostics[2].source_ranges[range_number - 1]
    
    assert_equal false, source_range_1.is_equal(source_range_2)
    assert_equal false, source_range_1 == source_range_2
  end
  def test_SourceRange_get_start
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS,CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of Clangc::SourceLocation, source_range.start 
  end
  def test_SourceRange_get_end
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TWO_ERRORS,CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of Clangc::SourceLocation, source_range.end 
  end
end
