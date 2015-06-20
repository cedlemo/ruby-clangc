#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"

class TestSourceRange < MiniTest::Test
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
  def test_SourceRange_is_null_true
    source_range = Clangc.null_source_range
    assert_equal true, source_range.is_null
  end
  def test_SourceRange_is_null_false
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,@clang_headers_path)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_ranges = diagnostics[1].source_ranges
    assert_equal false, source_ranges[range_number - 1].is_null
  end
  def test_SourceRange_is_equal_true
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,@clang_headers_path)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]
    source_range_2 = diagnostics[1].source_ranges[range_number - 1]
    assert_equal true, source_range_1.is_equal(source_range_2)
    assert_equal false, source_range_1 == source_range_2
  end
  def test_SourceRange_is_equal_false
    tu = @cindex.create_translation_unit_from_source_file(@source_file_three_errors,@clang_headers_path)
    diagnostics = tu.diagnostics
    
    range_number = diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]

    range_number = diagnostics[2].num_ranges
    source_range_2 = diagnostics[2].source_ranges[range_number - 1]
    
    assert_equal false, source_range_1.is_equal(source_range_2)
    assert_equal false, source_range_1 == source_range_2
  end
  def test_SourceRange_get_start
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,@clang_headers_path)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of Clangc::SourceLocation, source_range.start 
  end
  def test_SourceRange_get_end
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,@clang_headers_path)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of Clangc::SourceLocation, source_range.end 
  end
end
