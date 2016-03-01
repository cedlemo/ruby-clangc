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
        elsif cursor.location.spelling[1] == 8
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
        unless (cursor.location.spelling[1] == 7 && cursor.spelling == "struct data") || !cursor.completion_string
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
  def test_CompletionString_num_chunks
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_equal 1, completion_string.num_chunks, "#{code} #{line} #{completion_string.num_chunks}"
          elsif line == 2
            assert_equal 2, completion_string.num_chunks, "#{code} #{line} #{completion_string.num_chunks}"
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_chunk_kind
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         completion_string.chunk_kind(0),
                         "#{code} #{line} #{completion_string.chunk_kind(0)}")
            elsif line == 2
            assert_equal(Clangc::CompletionChunkKind::RESULT_TYPE, 
                         completion_string.chunk_kind(0),
                         "#{code} #{line} #{completion_string.chunk_kind(0)}")
            assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT, 
                         completion_string.chunk_kind(1),
                         "#{code} #{line} #{completion_string.chunk_kind(1)}")
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_chunk_kinds
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          kinds = completion_string.chunk_kinds
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
  def test_CompletionString_chunk_text
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_equal("data", 
                         completion_string.chunk_text(0),
                         "#{code} #{line} #{completion_string.chunk_text(0)}")
            elsif line == 2
            assert_equal("int", 
                         completion_string.chunk_text(0),
                         "#{code} #{line} #{completion_string.chunk_text(0)}")
            assert_equal("a", 
                         completion_string.chunk_text(1),
                         "#{code} #{line} #{completion_string.chunk_text(1)}")
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_chunk_texts
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          texts = completion_string.chunk_texts
          if line == 1
            assert_equal("data", 
                         texts[0],
                         "#{code} #{line} #{texts[0]}")
            elsif line == 2
            assert_equal("int", 
                         texts[0],
                         "#{code} #{line} #{texts[0]}")
            assert_equal("a", 
                         texts[1],
                         "#{code} #{line} #{texts[1]}")
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_num_annotations
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
            assert_equal 0, completion_string.num_annotations, "#{code} #{line} #{completion_string.num_annotations}"
        end
      # TODO find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_annotation
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if completion_string.num_annotations > 0  
            assert_equal "", completion_string.annotation(0), "#{code} #{line} #{completion_string.annotation(0)}"
          end
        end
      # TODO find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_annotations
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
            assert_equal 0, completion_string.num_annotations, "#{code} #{line} #{completion_string.annotations.size}"
        end
      # TODO find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_brief_comment
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling 
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
            assert_equal "", completion_string.brief_comment, "#{code} #{line} #{completion_string.annotation(0)}"
        end
      # TODO find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_CompletionString_chunk_completion_string
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_COMPLETION_STRING
        completion_string = cursor.completion_string
        code = cursor.spelling
        _file, line, _column = cursor.location.spelling
        if completion_string && completion_string.availability == Clangc::AvailabilityKind::AVAILABLE
          if line == 1
            assert_instance_of(Clangc::CompletionString, 
                         completion_string.chunk_completion_string(0))
            elsif line == 2
            assert_instance_of(Clangc::CompletionString, 
                         completion_string.chunk_completion_string(0))
            assert_instance_of(Clangc::CompletionString, 
                         completion_string.chunk_completion_string(1))
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
