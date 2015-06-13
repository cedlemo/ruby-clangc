#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"

class TestTranslationUnitCreation < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(@bad_file,@clang_headers_path)
    assert_nil tu 
  end
  def test_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(@bad_file,@clang_headers_path)
    assert_nil tu 
  end
  def test_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[@source_file] + @clang_headers_path)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[@bad_file] + @clang_headers_path)
    assert_nil tu 
  end
  def test_create_TU
    tu = @cindex.create_translation_unit(@ast_file)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU
    tu = @cindex.create_translation_unit(@bad_file)
    assert_nil tu 
  end
  # return an error code if the Translation Unit creation fail
  def test_create_TU2
    tu = @cindex.create_translation_unit2(@ast_file)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU2
    tu = @cindex.create_translation_unit2(@bad_file)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
  def test_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(@source_file,@clang_headers_path, options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_fail_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(@bad_file,@clang_headers_path, options)
    assert_nil tu
  end
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(@source_file,@clang_headers_path, options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_failparse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(@bad_file,@clang_headers_path, options)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
end
class TestTranslationUnitUsage < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_TU_get_num_diagnostic_zero
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_equal 0, tu.diagnostics_num
  end
  def test_TU_get_num_diagnostic_one
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,@clang_headers_path)
    assert_equal 1, tu.diagnostics_num
  end  
  def test_TU_get_default_save_options
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_equal Clangc::SaveTranslationUnit_Flags::None, tu.default_save_options
  end
  def test_TU_get_spelling
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_equal @source_file, tu.spelling
  end
  def test_TU_get_default_reparse_options
    tu = @cindex.create_translation_unit_from_source_file(@source_file,@clang_headers_path)
    assert_equal Clangc::Reparse_Flags::None, tu.default_reparse_options
  end
  def test_TU_get_diagnostic_one
    tu = @cindex.create_translation_unit_from_source_file(@source_file_one_error,@clang_headers_path)
    assert_instance_of Clangc::Diagnostic, tu.diagnostic(0)
  end
  def test_TU_get_file_that_exists
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    assert_instance_of Clangc::File, tu.file(@source_file)
  end
  def test_TU_get_file_that_doesnt_exist
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    assert_equal nil, tu.file(@bad_file)
  end
end
