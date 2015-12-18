#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCompletionString < MiniTest::Test
  include ClangcUtils
  def setup
  end
  def teardown
  end
end
