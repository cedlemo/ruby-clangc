#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestTranslationUnitCreation < MiniTest::Test
  include ClangcUtils
  
  def setup
    @cindex = Clangc::Index.new(false, false)
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
  end
  
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  
  def test_create_TU_from_source_file
    tu = @cindex.create_translation_unit(source: SOURCE_FILE, args: CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  
  def test_fail_create_TU_from_source_file
    tu = @cindex.create_translation_unit(source: BAD_FILE, args: CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  
  def test_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit(source: SOURCE_FILE, args: CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  
  def test_fail_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit(source: BAD_FILE, args: CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  
  def test_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit(args: [SOURCE_FILE] + CLANG_HEADERS_PATH)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  
  def test_fail_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit(args: [BAD_FILE] + CLANG_HEADERS_PATH)
    assert_nil tu 
  end
  
  def test_create_TU
    tu = @cindex.create_translation_unit(ast: AST_FILE)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  
  def test_fail_create_TU
    tu = @cindex.create_translation_unit(ast: BAD_FILE)
    assert_nil tu 
  end
  
  # return an error code if the Translation Unit creation fail
  def test_create_TU2
    tu = @cindex.create_translation_unit(ast: AST_FILE, error: true)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  
  def test_fail_create_TU2
    tu = @cindex.create_translation_unit(ast: BAD_FILE, error: true)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::FAILURE, tu
  end
  
  def test_parse_TU
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE,
                                  args: CLANG_HEADERS_PATH, flags: options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  
  def test_fail_parse_TU
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: BAD_FILE,args: CLANG_HEADERS_PATH,
                                  flags: options)
    assert_nil tu
  end
  
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE, args: CLANG_HEADERS_PATH,
                                   flags: options, error: true)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  
  def test_failparse_TU2
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: BAD_FILE, args: CLANG_HEADERS_PATH,
                                  flags: options, error: true)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::FAILURE, tu
  end
end
class TestTranslationUnitUsage < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
  end
  
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_TU_get_num_diagnostic_zero
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal 0, tu.diagnostics_num
  end
  def test_TU_get_num_diagnostic_one
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    assert_equal 1, tu.diagnostics_num
  end  
  def test_TU_get_default_save_options
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal Clangc::SaveTranslationUnit_Flags::NONE, tu.default_save_options
  end
  def test_TU_get_spelling
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal SOURCE_FILE, tu.spelling
  end
  def test_TU_get_default_reparse_options
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal Clangc::Reparse_Flags::NONE, tu.default_reparse_options
  end
  def test_TU_get_diagnostic_one
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of Clangc::Diagnostic, tu.diagnostic(0)
  end
  def test_TU_get_file_that_exists
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of Clangc::File, tu.file(SOURCE_FILE)
  end
  def test_TU_get_file_that_doesnt_exist
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal nil, tu.file(BAD_FILE)
  end
  def test_TU_get_cursor
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of Clangc::Cursor, tu.cursor
  end
  def test_TU_get_module
    # TODO
  end
  def test_TU_code_complete_at
    # TODO check this.
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMPLETION_STRING,
                                         args: CLANG_HEADERS_PATH)
    line = 1 
    column = 6
    reparse_options = tu.default_reparse_options

    tu.reparse(reparse_options)
    
    options = 0
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    assert_instance_of(Clangc::CodeCompleteResults,
                       complete_results)
    assert_equal(3, complete_results.results.size)
    assert_equal(1, complete_results.results[0].completion_string.num_chunks)
    assert_equal(1, complete_results.results[1].completion_string.num_chunks)
    assert_equal(1, complete_results.results[2].completion_string.num_chunks)
    all_chunks = []
    complete_results.results.each do |r|
      all_chunks << r.completion_string.chunk_texts
    end
    all_chunks.sort
    all_chunks_ref = [["const"],
                      ["volatile"],
                      ["restrict"]
                      ]
    assert_equal(all_chunks_ref, all_chunks)
    assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT,
                 complete_results.results[0].completion_string.chunk_kind(0))
  end
  def test_TU_reparse
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMPLETION_STRING,
                                         args: CLANG_HEADERS_PATH)
    options = tu.default_reparse_options
    error_code = tu.reparse(options)
    assert(0 == error_code, error_code)
  end
end
