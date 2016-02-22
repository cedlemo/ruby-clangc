#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCompletionString < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
  end
  def teardown
  end
  def test_CompletionString_from_cursor
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        if cursor.location.spelling[1] == 7 && cursor.spelling == "struct data"
        assert_equal false, Clangc::CompletionString == cursor.completion_string.class, "#{cursor.spelling} #{cursor.location.spelling[1]}"
        else
        assert_equal true, Clangc::CompletionString == cursor.completion_string.class, "#{cursor.spelling} #{cursor.location.spelling[1]}"
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_from_cursor
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        unless (cursor.location.spelling[1] == 7 && cursor.spelling == "struct data")
        assert_equal Clangc::AvailabilityKind::AVAILABLE, cursor.completion_string.availability, "#{cursor.spelling} #{cursor.location.spelling[1]} #{cursor.completion_string.availability}"
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
