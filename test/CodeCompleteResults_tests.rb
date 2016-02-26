#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCodeCompleteResults < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
  end
  def test_CodeCompleteResults_num_results
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_instance_of(Fixnum, complete_results.num_results)

  end
  def test_CodeCompleteResults_get_result
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_instance_of(Clangc::CompletionResult, complete_results.result(0))
  end
end
