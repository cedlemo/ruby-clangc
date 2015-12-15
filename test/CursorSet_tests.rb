#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCursorSet < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    @tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
end
  def teardown
  end
  def test_CursorSet_new
    assert_instance_of Clangc::CursorSet, Clangc::CursorSet.new
  end
  def test_CursorSet_contains
    cursor_set = Clangc::CursorSet.new
    Clangc.visit_children(cursor: @tu.cursor) do |cursor, parent|
      assert_equal false, cursor_set.contains(cursor)
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CursorSet_insert
    cursor_set = Clangc::CursorSet.new
    Clangc.visit_children(cursor: @tu.cursor) do |cursor, parent|
      assert_equal true, cursor_set.insert(cursor)
      assert_equal true, cursor_set.contains(cursor)
      assert_equal false, cursor_set.insert(cursor)

      Clangc::ChildVisitResult::BREAK
    end
  end
end
