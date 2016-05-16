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

class TestCursorUsage < MiniTest::Test
  include ClangcUtils
  def setup
    # TODO
    cindex = Clangc::Index.new(false, false)
      #tu = cindex.create_translation_unit_from_source_file(SOURCE_FILE_MODULE_IMPORT, ["-x", "c++", "-fmodules", "-fcxxmodules"] + CLANG_HEADERS_PATH)
    tu = cindex.create_translation_unit_from_source_file(SOURCE_FILE_MODULE_IMPORT,
                                                         ["-x", "c++", "-fmodules"]  + CLANG_HEADERS_PATH)
#    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
#      if cursor.kind == Clangc::CursorKind::MODULE_IMPORT_DECL
#        @module = cursor.module
#      end
#      Clangc::ChildVisitResult::RECURSE
#    end
  end
  def test_module_get_ast_file
    # TODO
  end
  def test_module_get_parent
    # TODO
  end
  def test_module_get_name
    # TODO
  end
  def test_module_get_full_name
    # TODO
  end
  def test_module_is_system
    # TODO
  end
  def test_module_get_num_top_level_headers
    # TODO
  end
  def test_module_get_top_level_header
    # TODO
  end
  def test_module_get_top_level_headers
    # TODO
  end
end
