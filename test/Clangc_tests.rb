#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestClangcMethods < MiniTest::Test
  def test_clangc_version
    assert_instance_of String, Clangc.version
  end
end
