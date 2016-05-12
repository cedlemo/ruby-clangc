#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2015-2016 Cédric Le Moigne cedlemo <cedlemo@gmx.com>
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

class TestFile < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
    @tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                          args: CLANG_HEADERS_PATH)
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_file_get_name
    file = @tu.file(SOURCE_FILE)
    assert_equal(SOURCE_FILE, file.name)
  end

  def test_file_time
    file = @tu.file(SOURCE_FILE)
    assert_equal(File.mtime(SOURCE_FILE).asctime, file.mtime.asctime)
  end

  def test_file_multiple_include_guarded_false
    file = @tu.file(SOURCE_FILE)
    assert_equal(false, file.is_multiple_include_guarded)
  end

  def test_file_multiple_include_guarded_true
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_WITH_INCLUDE_GUARD,
                                         args: CLANG_HEADERS_PATH)
    file = tu.file(SOURCE_FILE_WITH_INCLUDE_GUARD)
    assert_equal(true, file.is_multiple_include_guarded)
  end

  if Clangc.version =~ /clang version 3.(6|7)/
    def test_file_equal_true
      file = @tu.file(SOURCE_FILE)
      another_file_object = @tu.file(SOURCE_FILE)
      assert_equal(false, file == another_file_object)
      assert_equal(true, file.is_equal(another_file_object))
    end
  end
end
