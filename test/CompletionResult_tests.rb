#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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
    assert_equal(1, complete_results.num_diagnostics)
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
    #assert_equal("redefinition of 'ptr' with a different type: 'int' vs 'struct data *'", complete_results.diagnostic(0).spelling)
    assert_equal("use of undeclared identifier 'pt'", complete_results.diagnostic(0).spelling)
  end
  def test_CompletionResult_get_diagnostics
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_equal(1, complete_results.diagnostics.size)
    #assert_equal("redefinition of 'ptr' with a different type: 'int' vs 'struct data *'", complete_results.diagnostics[0].spelling)
    assert_equal("use of undeclared identifier 'pt'", complete_results.diagnostics[0].spelling)
  end
end
