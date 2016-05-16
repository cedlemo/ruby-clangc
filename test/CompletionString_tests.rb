#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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

class TestCompletionString < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
  end
  
  def test_CompletionString_from_cursor
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if line == 1 && cursor.spelling == "f"
          assert_instance_of(Clangc::CompletionString,
                             cursor.completion_string,
                             "#{cursor.spelling} #{line}")
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_availability
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,  ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if line == 1 && cursor.spelling == "f"
          assert_equal(Clangc::AvailabilityKind::AVAILABLE,
                       cursor.completion_string.availability)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_priority
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,  ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if line == 1 && cursor.spelling == "f"
          completion_string = cursor.completion_string
          assert_equal(50, completion_string.priority)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_num_chunks
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,  ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          assert_equal(6, completion_string.num_chunks, pos)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_kind
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          assert_equal(Clangc::CompletionChunkKind::RESULT_TYPE, # void
                       completion_string.chunk_kind(0))
          assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT,  # f
                       completion_string.chunk_kind(1))
          assert_equal(Clangc::CompletionChunkKind::LEFT_PAREN,  # (
                       completion_string.chunk_kind(2))
          assert_equal(Clangc::CompletionChunkKind::PLACEHOLDER, # int x
                       completion_string.chunk_kind(3))
          assert_equal(Clangc::CompletionChunkKind::OPTIONAL, # ,
                       completion_string.chunk_kind(4))
          assert_equal(Clangc::CompletionChunkKind::RIGHT_PAREN,
                       completion_string.chunk_kind(5))
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_kinds
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,  ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          chunk_kinds = cursor.completion_string.chunk_kinds
          assert_equal(Clangc::CompletionChunkKind::RESULT_TYPE, # void
                       chunk_kinds[0])
          assert_equal(Clangc::CompletionChunkKind::TYPED_TEXT,  # f
                       chunk_kinds[1])
          assert_equal(Clangc::CompletionChunkKind::LEFT_PAREN,  # (
                       chunk_kinds[2])
          assert_equal(Clangc::CompletionChunkKind::PLACEHOLDER, # int x
                       chunk_kinds[3])
          assert_equal(Clangc::CompletionChunkKind::OPTIONAL, # ,
                       chunk_kinds[4])
          assert_equal(Clangc::CompletionChunkKind::RIGHT_PAREN,
                       chunk_kinds[5])
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_text
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,  ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          assert_equal("void",
                       completion_string.chunk_text(0))
          assert_equal("f",
                       completion_string.chunk_text(1))
          assert_equal("(",
                       completion_string.chunk_text(2))
          assert_equal("int x",
                       completion_string.chunk_text(3))
          assert_equal("",
                       completion_string.chunk_text(4))
          assert_equal(")",
                       completion_string.chunk_text(5))
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_texts
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING,   ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          chunk_texts = cursor.completion_string.chunk_texts
          assert_equal("void",
                       chunk_texts[0])
          assert_equal("f",
                       chunk_texts[1])
          assert_equal("(",
                       chunk_texts[2])
          assert_equal("int x",
                       chunk_texts[3])
          assert_equal("",
                       chunk_texts[4])
          assert_equal(")",
                       chunk_texts[5])
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_num_annotations
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          assert_equal( 0, completion_string.num_annotations)
        end
        # NOTE find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_annotation
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        # NOTE find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_annotations
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        # NOTE find real example
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_brief_comment
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          # NOTE find real example
          completion_string = cursor.completion_string
          assert_equal("", completion_string.brief_comment)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_completion_string
   tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          chunks_kinds = completion_string.chunk_kinds 
          chunks_kinds.each_with_index do |k, index|
            assert_instance_of(Clangc::CompletionString, 
                         completion_string.chunk_completion_string(index))
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_CompletionString_chunk_completion_strings
   tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_COMPLETION_STRING, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      file, line, pos, offset = cursor.location.spelling
      if file.name == SOURCE_FILE_COMPLETION_STRING
        if cursor.spelling == "f"
          completion_string = cursor.completion_string
          completion_string.chunk_completion_strings.each do |cs|
            assert_instance_of(Clangc::CompletionString, cs)
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
