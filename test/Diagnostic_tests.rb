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
  def test_get_diagnostic_severity_one_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_instance_of Fixnum, diagnostics[0].severity
    assert_equal Clangc::DiagnosticSeverity::Error , diagnostics[0].severity
  end
  def test_get_diagnostic_severity_two_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_instance_of Fixnum, diagnostics[1].severity
    assert_equal Clangc::DiagnosticSeverity::Warning , diagnostics[1].severity
  end
  def test_get_diagnostic_spelling_one_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal "expected \';\' after top level declarator" , diagnostics[0].spelling
  end
  def test_get_diagnostic_spelling_two_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal "type specifier missing, defaults to \'int\'" , diagnostics[1].spelling
  end
  def test_get_diagnostic_category_one_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal 4 , diagnostics[0].category
  end
  def test_get_diagnostic_category_two_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal 2 , diagnostics[1].category
  end
  def test_get_diagnostic_category_name_one_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal "Parse Issue" , diagnostics[0].category_name
  end
  def test_get_diagnostic_category_name_two_error
    tu = @cindex.create_translation_unit_from_source_file(@source_file_two_errors,[@clang_headers_path])
    diagnostics = tu.diagnostics
    assert_equal "Semantic Issue" , diagnostics[1].category_name
  end
end
