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
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.kind == Clangc::CursorKind::MODULE_IMPORT_DECL
        @module = cursor.module
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

end
