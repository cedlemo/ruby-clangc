#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCompletionResult < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
  end
  def test_CompletionResult_get_cursor_kind
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    completion_result = complete_results.result(0)
    # TODO
    found = false
    Clangc::CursorKind.constants.each do |c|
      found = true if completion_result.cursor_kind == Clangc::CursorKind.const_get(c)  
    end
  end
  def test_CompletionResult_get_completion_string
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    completion_result = complete_results.result(0)
    # TODO
    assert_instance_of(Clangc::CompletionString, completion_result.completion_string)  
  end
  def test_CompletionResult_get_container_usr
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_instance_of(String, complete_results.container_usr)
    assert_equal("", complete_results.container_usr)
  end
  def test_CompletionResult_get_num_diagnostics
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    assert_equal(8, complete_results.num_diagnostics)
  end
  def test_CompletionResult_get_diagnostic
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_instance_of(Clangc::Diagnostic, complete_results.diagnostic(0))
    assert_equal("redefinition of 'ptr' with a different type: 'int' vs 'struct data *'", complete_results.diagnostic(0).spelling)

  end
end
