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
  def test_CompletionString_availability
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
  def test_CompletionString_priority
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
            assert_equal 50, completion_string.priority, "#{code} #{line} #{completion_string.priority}"
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_num_completion_chunks
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_equal 1, completion_string.num_completion_chunks, "#{code} #{line} #{completion_string.num_completion_chunks}"
          else
            assert_equal 2, completion_string.num_completion_chunks, "#{code} #{line} #{completion_string.num_completion_chunks}"
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_chunk_completion_kind
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         completion_string.completion_chunk_kind(0),
                         "#{code} #{line} #{completion_string.completion_chunk_kind(0)}")
            elsif line == 2
            assert_equal(Clangc::CompletionChunkKind::RESULT_TYPE, 
                         completion_string.completion_chunk_kind(0),
                         "#{code} #{line} #{completion_string.completion_chunk_kind(0)}")
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         completion_string.completion_chunk_kind(1),
                         "#{code} #{line} #{completion_string.completion_chunk_kind(1)}")
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_completion_chunk_kinds
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          kinds = completion_string.completion_chunk_kinds
          if line == 1
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         kinds[0],
                         "#{code} #{line} #{kinds[0]}")
            elsif line == 2
            assert_equal(Clangc::CompletionChunkKind::RESULT_TYPE, 
                         kinds[0],
                         "#{code} #{line} #{kinds[0]}")
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         kinds[1],
                         "#{code} #{line} #{kinds[1]}")
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
