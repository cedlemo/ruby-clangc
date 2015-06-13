#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"

class TestFile < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
    @tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_File_get_name
    file = @tu.file(@source_file)
    assert_equal @source_file, file.name
  end
  def test_File_time
    file = @tu.file(@source_file)
    assert_equal File.mtime(@source_file).asctime, file.mtime.asctime
  end
end