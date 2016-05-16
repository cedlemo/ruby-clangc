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

class TestCursorSet < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    @tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                          args: CLANG_HEADERS_PATH)
  end

  def test_cursor_set_new
    assert_instance_of Clangc::CursorSet, Clangc::CursorSet.new
  end

  def test_cursor_set_contains
    cursor_set = Clangc::CursorSet.new
    Clangc.visit_children(cursor: @tu.cursor) do |cursor, _parent|
      assert_equal(false, cursor_set.contains(cursor))
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_set_insert
    cursor_set = Clangc::CursorSet.new
    Clangc.visit_children(cursor: @tu.cursor) do |cursor, _parent|
      assert_equal(true, cursor_set.insert(cursor))
      assert_equal(true, cursor_set.contains(cursor))
      assert_equal(false, cursor_set.insert(cursor))
      Clangc::ChildVisitResult::BREAK
    end
  end
end
