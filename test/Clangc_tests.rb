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
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestClangcMethods < MiniTest::Test
  include ClangcUtils

  def test_clangc_version
    assert_instance_of(String, Clangc.version)
  end

  def test_clangc_default_diagnostic_display_options
    assert_instance_of(Fixnum, Clangc.default_diagnostic_display_options)
    default = Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION |
              Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN |
              Clangc::DiagnosticDisplayOptions::DISPLAY_OPTION
    assert_equal(default, Clangc.default_diagnostic_display_options)
  end

  def test_clangc_default_editing_translation_unit_options
    assert_instance_of(Fixnum, Clangc.default_editing_translation_unit_options)
  end

  def test_clangc_default_code_complete_options
    assert_instance_of(Fixnum, Clangc.default_code_complete_options)
  end

  def test_clangc_null_range
    assert_instance_of(Clangc::SourceRange, Clangc.null_source_range)
  end

  def test_clangc_null_location
    assert_instance_of(Clangc::SourceLocation, Clangc.null_source_location)
  end

  def test_clangc_null_cursor
    assert_instance_of(Clangc::Cursor, Clangc.null_cursor)
  end

  def test_clang_visit_children_with_block
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE,
                                                        CLANG_HEADERS_PATH)
    Clangc.visit_children_with_block(tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor)
      assert_instance_of(Clangc::Cursor, parent)
    end
  end

  def test_clang_visit_children_with_proc
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE,
                                                        CLANG_HEADERS_PATH)
    myproc = proc do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor)
      assert_instance_of(Clangc::Cursor, parent)
    end
    Clangc.visit_children_with_proc(tu.cursor, myproc)
  end

  def test_clang_visit_children
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE,
                                                        CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor)
      assert_instance_of(Clangc::Cursor, parent)
    end
    myproc = proc do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor)
      assert_instance_of(Clangc::Cursor, parent)
    end
    Clangc.visit_children(cursor: tu.cursor, visitor: myproc)
  end

  def test_source_location_file_location_start_end
    cindex = Clangc::Index.new(false, false)
    tu = cindex.create_translation_unit_from_source_file(SOURCE_FILE,
                                                         CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.location.spelling[0].name == SOURCE_FILE
        if cursor.kind == Clangc::CursorKind::STRUCT_DECL
          initial_range = cursor.extent
          assert_instance_of(Clangc::SourceLocation, initial_range.start)
          assert_instance_of(Clangc::SourceLocation, initial_range.end)
          recomputed_range = Clangc.range(initial_range.start,
                                          initial_range.end)
          assert(initial_range.is_equal(recomputed_range))
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
