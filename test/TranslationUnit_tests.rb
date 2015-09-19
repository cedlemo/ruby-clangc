#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "./clangc_utils.rb"

class TestTranslationUnitCreation < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
#    # Good C test file
#    SOURCE_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
#    # C source code with one error
#    SOURCE_FILE_ONE_ERROR = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
#    # Inexistant file
#    BAD_FILE = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
#    AST_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
#    CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(BAD_FILE,CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  def test_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(BAD_FILE,CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  def test_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[SOURCE_FILE] + CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[BAD_FILE] + CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  def test_create_TU
    tu = @cindex.create_translation_unit(AST_FILE)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU
    tu = @cindex.create_translation_unit(BAD_FILE)
    assert_nil tu 
  end
  # return an error code if the Translation Unit creation fail
  def test_create_TU2
    tu = @cindex.create_translation_unit2(AST_FILE)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU2
    tu = @cindex.create_translation_unit2(BAD_FILE)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
  def test_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(SOURCE_FILE,CLANG_HEADERS_PATH, options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_fail_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(BAD_FILE,CLANG_HEADERS_PATH, options)
    assert_nil tu
  end
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(SOURCE_FILE,CLANG_HEADERS_PATH, options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_failparse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(BAD_FILE,CLANG_HEADERS_PATH, options)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
end
class TestTranslationUnitUsage < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    #SOURCE_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    #SOURCE_FILE_ONE_ERROR = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # Inexistant file
    #BAD_FILE = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    #AST_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
    #CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_TU_get_num_diagnostic_zero
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_equal 0, tu.diagnostics_num
  end
  def test_TU_get_num_diagnostic_one
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ONE_ERROR,CLANG_HEADERS_PATH)
    assert_equal 1, tu.diagnostics_num
  end  
  def test_TU_get_default_save_options
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_equal Clangc::SaveTranslationUnit_Flags::None, tu.default_save_options
  end
  def test_TU_get_spelling
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_equal SOURCE_FILE, tu.spelling
  end
  def test_TU_get_default_reparse_options
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,CLANG_HEADERS_PATH)
    assert_equal Clangc::Reparse_Flags::None, tu.default_reparse_options
  end
  def test_TU_get_diagnostic_one
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ONE_ERROR,CLANG_HEADERS_PATH)
    assert_instance_of Clangc::Diagnostic, tu.diagnostic(0)
  end
  def test_TU_get_file_that_exists
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    assert_instance_of Clangc::File, tu.file(SOURCE_FILE)
  end
  def test_TU_get_file_that_doesnt_exist
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    assert_equal nil, tu.file(BAD_FILE)
  end
  def test_TU_get_cursor
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    assert_instance_of Clangc::Cursor, tu.cursor
  end
end
