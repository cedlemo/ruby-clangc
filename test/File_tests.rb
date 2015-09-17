#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "./clangc_utils.rb"

class TestFile < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
#    # Good C test file
#    SOURCE_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
#    # C source code with one error
#    SOURCE_FILE_ONE_ERROR = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
#    # Inexistant file
#    BAD_FILE = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
#    SOURCE_FILE_WITH_INCLUDE_GUARD = "#{File.expand_path(File.dirname(__FILE__))}/include_guarded_header.h"
#    AST_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
#    CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
    @tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_File_get_name
    file = @tu.file(SOURCE_FILE)
    assert_equal SOURCE_FILE, file.name
  end
  def test_File_time
    file = @tu.file(SOURCE_FILE)
    assert_equal File.mtime(SOURCE_FILE).asctime, file.mtime.asctime
  end
  def test_File_multiple_include_guarded_false
    file = @tu.file(SOURCE_FILE)
    assert_equal false, file.is_multiple_include_guarded
  end
  def test_File_multiple_include_guarded_true
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_WITH_INCLUDE_GUARD, CLANG_HEADERS_PATH)
    file = tu.file(SOURCE_FILE_WITH_INCLUDE_GUARD)
    assert_equal true, file.is_multiple_include_guarded
  end
  def test_File_equal_true
    file = @tu.file(SOURCE_FILE)
    another_file_object = @tu.file(SOURCE_FILE)
    assert_equal false, file == another_file_object
    assert_equal true, file.is_equal(another_file_object)
  end
end
