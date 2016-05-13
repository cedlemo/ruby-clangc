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

class TestSourceRange < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_source_range_is_null_true
    source_range = Clangc.null_source_range
    assert_equal(true, source_range.is_null)
  end

  def test_source_range_is_null_false
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_ranges = diagnostics[1].source_ranges
    assert_equal(false, source_ranges[range_number - 1].is_null)
  end

  def test_source_range_is_equal_true
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]
    source_range_2 = diagnostics[1].source_ranges[range_number - 1]
    assert_equal(true, source_range_1.is_equal(source_range_2))
    assert_equal(false, source_range_1 == source_range_2)
  end

  def test_source_range_is_equal_false
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_THREE_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = diagnostics[1].num_ranges
    source_range_1 = diagnostics[1].source_ranges[range_number - 1]
    range_number = diagnostics[2].num_ranges
    source_range_2 = diagnostics[2].source_ranges[range_number - 1]

    assert_equal(false, source_range_1.is_equal(source_range_2))
    assert_equal(false, source_range_1 == source_range_2)
  end

  def test_source_range_get_start
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of(Clangc::SourceLocation, source_range.start)
  end

  def test_source_range_get_end
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    assert_instance_of(Clangc::SourceLocation, source_range.end)
  end

  def test_source_range_start_end
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.location.spelling[0].name == SOURCE_FILE
        if cursor.kind == Clangc::CursorKind::STRUCT_DECL
          range = cursor.extent
          _file, start_line, start_column = range.start.spelling
          _file, stop_line, stop_column = range.end.spelling
          assert_equal([3, 9], [start_line, start_column], range.start.spelling)
          assert_equal([6, 4], [stop_line, stop_column], range.end.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
