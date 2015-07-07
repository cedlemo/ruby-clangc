#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"

class TestSourceLocation < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # Good C test file with gnu-binary-literal option warning
    @source_file_option_warnings = "#{File.expand_path(File.dirname(__FILE__))}/source_option_warnings.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # C source code with two error
    @source_file_two_errors = "#{File.expand_path(File.dirname(__FILE__))}/source3.c"
    # C source code with three error
    @source_file_three_errors = "#{File.expand_path(File.dirname(__FILE__))}/source4.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_SourceLocation_is_in_system_header
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    source_location = source_range.start
    assert_equal false, source_location.is_in_system_header 
  end
  def test_SourceLocation_is_from_main_file
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    assert_equal true, source_location.is_from_main_file
  end
  def test_SourceLocation_equal
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location_start = diagnostics[1].source_ranges.last.start
    source_location_end = diagnostics[1].source_ranges.last.end
    source_location_end_1 = diagnostics[1].source_ranges.last.end
    assert_equal false, source_location_start.is_equal(source_location_end)
    assert_equal true, source_location_end.is_equal(source_location_end_1)
  end
  def test_SourceLocation_spelling
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    spelling = source_location.spelling
    # file
    assert_equal true, tu.file(@source_file_two_errors).is_equal(spelling[0])
    # line
    assert_equal 14, spelling[1]
    # column
    assert_equal 1, spelling[2]
    #offset
    assert_equal 179, spelling[3]
  end
  def test_SourceLocation_spelling_end
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    spelling = source_location.spelling
    # file
    assert_equal false, tu.file(@source_file_two_errors).is_equal(spelling[0])
    # line
    assert_equal 0, spelling[1]
    # column
    assert_equal 0, spelling[2]
    #offset
    assert_equal 0, spelling[3]
  end
  def test_SourceLocation_file_location
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    file_location = source_location.file_location
    # file
    assert_equal true, tu.file(@source_file_two_errors).is_equal(file_location[0])
    # line
    assert_equal 14, file_location[1]
    # column
    assert_equal 1, file_location[2]
    #offset
    assert_equal 179, file_location[3]
  end
  def test_SourceLocation_file_location_end
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors, @clang_headers_path)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    file_location = source_location.file_location
    # file
    assert_equal false, tu.file(@source_file_two_errors).is_equal(file_location[0])
    # line
    assert_equal 0, file_location[1]
    # column
    assert_equal 0, file_location[2]
    #offset
    assert_equal 0, file_location[3]
  end
#  def test_SourceRange_get_end
#    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,@clang_headers_path)
#    diagnostics = tu.diagnostics
#    range_number = tu.diagnostics[1].num_ranges
#    source_range = diagnostics[1].source_ranges[range_number - 1]
#    assert_instance_of Clangc::SourceLocation, source_range.end 
#  end
end
