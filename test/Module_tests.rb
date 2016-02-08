#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCursorUsage < MiniTest::Test
  include ClangcUtils
  def setup
    # TODO
    cindex = Clangc::Index.new(false, false)
      #tu = cindex.create_translation_unit_from_source_file(SOURCE_FILE_MODULE_IMPORT, ["-x", "c++", "-fmodules", "-fcxxmodules"] + CLANG_HEADERS_PATH)
    tu = cindex.create_translation_unit_from_source_file(SOURCE_FILE_MODULE_IMPORT,
                                                         ["-x", "c++", "-fmodules", "-fcxxmodules", "-fmodules-cache-path", "#{PATH}/cache"] + CLANG_HEADERS_PATH)
#    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
#      if cursor.kind == Clangc::CursorKind::MODULE_IMPORT_DECL
#        @module = cursor.module
#      end
#      Clangc::ChildVisitResult::RECURSE
#    end
  end
  def test_module_get_ast_file
    # TODO
  end
  def test_module_get_parent
    # TODO
  end
  def test_module_get_name
    # TODO
  end
  def test_module_get_full_name
    # TODO
  end
  def test_module_is_system
    # TODO
  end
  def test_module_get_num_top_level_headers
    # TODO
  end
  def test_module_get_top_level_header
    # TODO
  end
  def test_module_get_top_level_headers
    # TODO
  end
end