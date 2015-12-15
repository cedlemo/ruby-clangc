#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCursorSet < MiniTest::Test
  include ClangcUtils
  def setup
  end
  def teardown
  end
  def test_CursorSet_new
    assert_instance_of Clangc::CursorSet, Clangc::CursorSet.new
  end
end
