#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2015-2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestTranslationUnitCreation < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_create_tu_from_source_file
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_create_tu_from_source_file
    tu = @cindex.create_translation_unit(source: BAD_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_nil(tu)
  end

  def test_create_tu_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_create_tu_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit(source: BAD_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_nil(tu)
  end

  def test_create_tu_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit(args: [SOURCE_FILE] +
                                               CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_create_tu_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit(args: [BAD_FILE] + CLANG_HEADERS_PATH)
    assert_nil(tu)
  end

  def test_create_tu
    tu = @cindex.create_translation_unit(ast: AST_FILE)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_create_tu
    tu = @cindex.create_translation_unit(ast: BAD_FILE)
    assert_nil(tu)
  end

  # return an error code if the Translation Unit creation fail
  def test_create_tu2
    tu = @cindex.create_translation_unit(ast: AST_FILE, error: true)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_create_tu2
    tu = @cindex.create_translation_unit(ast: BAD_FILE, error: true)
    assert_equal(Integer, tu.class.superclass)
    assert_equal(Clangc::ErrorCode::FAILURE, tu)
  end

  def test_parse_tu
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: options)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_parse_tu_sym
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: :none)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_fail_parse_tu
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: BAD_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: options)
    assert_nil(tu)
  end

  # return an error code if the Translation Unit creation fail
  def test_parse_tu2
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: options, error: true)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_parse_tu2_sym
    tu = @cindex.parse_translation_unit(source: SOURCE_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: :none, error: true)
    assert_instance_of(Clangc::TranslationUnit, tu)
  end

  def test_failparse_tu2
    options = Clangc::TranslationUnit_Flags::NONE
    tu = @cindex.parse_translation_unit(source: BAD_FILE,
                                        args: CLANG_HEADERS_PATH,
                                        flags: options, error: true)
    assert_equal(Integer, tu.class.superclass)
    assert_equal(Clangc::ErrorCode::FAILURE, tu)
  end
end

class TestTranslationUnitUsage < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_tu_get_num_diagnostic_zero
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(0, tu.diagnostics_num)
  end

  def test_tu_get_num_diagnostic_one
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(1, tu.diagnostics_num)
  end

  def test_tu_get_default_save_options
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(Clangc::SaveTranslationUnit_Flags::NONE,
                 tu.default_save_options)
  end

  def test_tu_get_spelling
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(SOURCE_FILE, tu.spelling)
  end

  def test_tu_get_default_reparse_options
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(Clangc::Reparse_Flags::NONE, tu.default_reparse_options)
  end

  def test_tu_get_diagnostic_one
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::Diagnostic, tu.diagnostic(0))
  end

  def test_tu_get_file_that_exists
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::File, tu.file(SOURCE_FILE))
  end

  def test_tu_get_file_that_doesnt_exist
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(nil, tu.file(BAD_FILE))
  end

  def test_tu_get_cursor
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::Cursor, tu.cursor)
  end

  def test_tu_get_module
    # TODO: find a working test case
  end

  def test_tu_code_complete_at
    # TODO: check this and clean.
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMPLETION_STRING,
                                         args: CLANG_HEADERS_PATH)
    line = 1
    column = 6
    reparse_options = tu.default_reparse_options

    tu.reparse(reparse_options)

    options = [:include_macros, :include_code_patterns, :include_brief_comments] 
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

  def test_tu_reparse
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMPLETION_STRING,
                                         args: CLANG_HEADERS_PATH)
    options = tu.default_reparse_options
    error_code = tu.reparse(options)
    assert(0 == error_code, error_code)
  end
end
