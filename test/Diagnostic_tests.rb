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

class TestDiagnostic < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end
  
  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_get_diagnostics_array_no_error
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE,
                                                          CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Array, diagnostics)
    assert_equal(0, diagnostics.size)
  end

  def test_get_diagnostics_array_one_error
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ONE_ERROR,
                                                          CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Array, diagnostics)
    assert_equal(1, diagnostics.size)
  end

  def test_get_diagnostics_array_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Array, diagnostics)
    assert_equal(2, diagnostics.size)
  end

  def test_get_diagnostic_severity_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Fixnum, diagnostics[0].severity)
    assert_equal(Clangc::DiagnosticSeverity::ERROR, diagnostics[0].severity)
  end

  def test_get_diagnostic_severity_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Fixnum, diagnostics[1].severity)
    assert_equal(Clangc::DiagnosticSeverity::WARNING, diagnostics[1].severity)
  end

  def test_get_diagnostic_spelling_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("expected \';\' after top level declarator",
                 diagnostics[0].spelling)
  end

  def test_get_diagnostic_spelling_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("type specifier missing, defaults to \'int\'",
                 diagnostics[1].spelling)
  end

  def test_get_diagnostic_category_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(4, diagnostics[0].category)
  end

  def test_get_diagnostic_category_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(2, diagnostics[1].category)
  end

  def test_get_diagnostic_category_name_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("Parse Issue", diagnostics[0].category_name)
  end

  def test_get_diagnostic_category_name_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("Semantic Issue", diagnostics[1].category_name)
  end

  def test_get_diagnostic_category_text_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("Parse Issue", diagnostics[0].category_text)
  end

  def test_get_diagnostic_category_text_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal("Semantic Issue", diagnostics[1].category_text)
  end

  def test_get_diagnostic_num_ranges_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(0, diagnostics[0].num_ranges)
  end

  def test_get_diagnostic_num_ranges_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(1, diagnostics[1].num_ranges)
  end

  def test_get_diagnostic_num_fixits_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(1, diagnostics[0].num_fixits)
  end

  def test_get_diagnostic_num_fixits_two_errors
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_equal(0, diagnostics[1].num_fixits)
  end

  def test_format_diagnostic_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    format = diagnostics[0].format(Clangc.default_diagnostic_display_options)
    assert_instance_of(String, format)
    reference = "error: expected \';\' after top level declarator [Parse Issue]"
    assert_equal(reference, diagnostics[0].format(:display_category_name))
  end

  def test_get_diagnostic_options
    options = CLANG_HEADERS_PATH + %w(-Wall -pedantic)
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OPTION_WARNINGS,
                                         args: options)
    assert_equal(20, tu.diagnostics.size)
    assert_equal("-Wgnu-binary-literal", tu.diagnostics[15].option[0])
    assert_equal("-Wno-gnu-binary-literal", tu.diagnostics[15].option[1])
  end

  def test_get_diagnostic_range
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    assert_instance_of(Clangc::SourceRange,
                       diagnostics[1].source_range(range_number - 1))
  end

  def test_get_diagnostic_ranges
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_TWO_ERRORS,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    range_number = tu.diagnostics[1].num_ranges
    source_ranges = diagnostics[1].source_ranges
    assert_equal range_number, source_ranges.size
    assert_instance_of(Clangc::SourceRange, source_ranges[range_number - 1])
  end

  def test_get_diagnostics_location_one_error
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ONE_ERROR,
                                         args: CLANG_HEADERS_PATH)
    diagnostics = tu.diagnostics
    assert_instance_of(Clangc::SourceLocation, diagnostics[0].source_location)
  end
end
