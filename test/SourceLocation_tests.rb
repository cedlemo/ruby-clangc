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

class TestSourceLocation < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_source_location_is_in_system_header
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_range = diagnostics[1].source_ranges[range_number - 1]
    source_location = source_range.start
    assert_equal(false, source_location.is_in_system_header)
  end

  def test_source_location_is_from_main_file
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    assert_equal(true, source_location.is_from_main_file)
  end

  def test_source_location_equal
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location_start = diagnostics[1].source_ranges.last.start
    source_location_end = diagnostics[1].source_ranges.last.end
    source_location_end_1 = diagnostics[1].source_ranges.last.end
    assert_equal(false, source_location_start.is_equal(source_location_end))
    assert_equal(true, source_location_end.is_equal(source_location_end_1))
  end

  def test_source_location_spelling
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    spelling = source_location.spelling
    # file
    assert_equal(SOURCE_FILE_TWO_ERRORS, spelling[0].name)
    # line
    assert_equal(14, spelling[1])
    # column
    assert_equal(1, spelling[2])
    # offset
    assert_equal(179, spelling[3])
  end

  def test_source_location_spelling_end
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    spelling = source_location.spelling
    if Clangc.version =~ /clang version 3.(6|7)/
      # file
      assert_equal(nil, spelling[0].name)
      # line
      assert_equal(0, spelling[1])
      # column
      assert_equal(0, spelling[2])
      # offset
      assert_equal(0, spelling[3])
    else
      # file
      assert_equal(SOURCE_FILE_TWO_ERRORS, spelling[0].name)
      # line
      assert_equal(14, spelling[1])
      # column
      assert_equal(2, spelling[2])
      # offset
      assert_equal(180, spelling[3])
    end
  end

  def test_source_location_file_location
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.start
    file_location = source_location.file_location
    # file
    assert_equal(SOURCE_FILE_TWO_ERRORS, file_location[0].name)
    # line
    assert_equal(14, file_location[1])
    # column
    assert_equal(1, file_location[2])
    # offset
    assert_equal(179, file_location[3])
  end

  def test_source_location_file_location_end
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    source_location = diagnostics[1].source_ranges.last.end
    file_location = source_location.file_location
    if Clangc.version =~ /clang version 3.(6|7)/
      # file
      assert_equal(nil, file_location[0].name)
      # line
      assert_equal(0, file_location[1])
      # column
      assert_equal(0, file_location[2])
      # offset
      assert_equal(0, file_location[3])
    else
      # file
      assert_equal(SOURCE_FILE_TWO_ERRORS, file_location[0].name)
      # line
      assert_equal(14, file_location[1])
      # column
      assert_equal(2, file_location[2])
      # offset
      assert_equal(180, file_location[3])
    end
  end

  def test_source_location_file_location_start_end
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.location.spelling[0].name == SOURCE_FILE
        if cursor.kind == Clangc::CursorKind::STRUCT_DECL
          range = cursor.extent
          _file, start_line, start_column = range.start.file_location
          _file, stop_line, stop_column = range.end.file_location
          assert_equal([3, 9], [start_line, start_column], range.start.spelling)
          assert_equal([6, 4], [stop_line, stop_column], range.end.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
