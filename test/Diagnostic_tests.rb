#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestDiagnostic < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # C source code with two error
    @source_file_two_errors = "#{File.expand_path(File.dirname(__FILE__))}/source3.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    @clang_headers_path = "-I/usr/lib/clang/3.6.0/include/"
  end
  def test_get_diagnostics_array_no_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_instance_of Array, diagnostics
    assert_equal 0, diagnostics.size
  end
  def test_get_diagnostics_array_one_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_instance_of Array, diagnostics
    assert_equal 1, diagnostics.size
  end
  def test_get_diagnostics_array_two_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_instance_of Array, diagnostics
    assert_equal 2, diagnostics.size
  end
end
