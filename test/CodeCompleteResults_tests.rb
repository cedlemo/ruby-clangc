#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2016 Cédric Le Moigne cedlemo <cedlemo@gmx.com>
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
  def test_CodeCompleteResults_get_results
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO
    assert_instance_of(Array, complete_results.results)
    assert_equal(complete_results.num_results, complete_results.results.size)
    assert_equal(complete_results.result(0).cursor_kind,
                 complete_results.results[0].cursor_kind)
  end
  def test_CodeCompleteResults_get_contexts
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    line = 12 
    column = 5 
    options = Clangc.default_code_complete_options
    complete_results = tu.code_complete_at(SOURCE_FILE_COMPLETION_STRING,
                                           line,
                                           column,
                                           options)
    # TODO 
    assert_instance_of(Fixnum, complete_results.contexts)
  end
end
