#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "./clangc_utils.rb"

class TestClangcMethods < MiniTest::Test
  include ClangcUtils
  def test_clangc_version
    assert_instance_of String, Clangc.version
  end
  def test_clangc_default_diagnostic_display_options
    assert_instance_of Fixnum, Clangc.default_diagnostic_display_options
    default_display_options = Clangc::DiagnosticDisplayOptions::Displaysourcelocation|
                              Clangc::DiagnosticDisplayOptions::Displaycolumn|
                              Clangc::DiagnosticDisplayOptions::Displayoption
    assert_equal default_display_options, Clangc.default_diagnostic_display_options
  end
  def test_clangc_default_editing_translation_unit_options
    assert_instance_of Fixnum, Clangc.default_editing_translation_unit_options
  end
  def test_clangc_default_code_complete_options
    assert_instance_of Fixnum, Clangc.default_code_complete_options
  end
  def test_clangc_null_range
    assert_instance_of Clangc::SourceRange, Clangc.null_source_range
  end
  def test_clangc_null_location
    assert_instance_of Clangc::SourceLocation, Clangc.null_source_location
  end
  def test_clangc_null_cursor
    assert_instance_of Clangc::Cursor, Clangc.null_cursor
  end
  def test_clang_visit_children_with_block
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE, 
                                                         CLANG_HEADERS_PATH)
    cursor = tu.cursor
    Clangc.visit_children_with_block(cursor) do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor
      assert_instance_of Clangc::Cursor, parent
    end
  end
  def test_clang_visit_children_with_proc
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE, 
                                                         CLANG_HEADERS_PATH)
    cursor = tu.cursor
    myproc = proc do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor
      assert_instance_of Clangc::Cursor, parent
    end
    Clangc.visit_children_with_proc(cursor, myproc)
  end
  def test_clang_visit_children
    index = Clangc::Index.new(false, false)
    tu = index.create_translation_unit_from_source_file(SOURCE_FILE, 
                                                         CLANG_HEADERS_PATH)
    cursor = tu.cursor
    Clangc.visit_children(cursor: cursor) do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor
      assert_instance_of Clangc::Cursor, parent
    end
    myproc = proc do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor
      assert_instance_of Clangc::Cursor, parent
    end
    Clangc.visit_children(cursor: cursor, visitor: myproc)
  end
end
