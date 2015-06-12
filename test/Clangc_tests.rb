#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestClangcMethods < MiniTest::Test
  def test_clangc_version
    assert_instance_of String, Clangc.version
  end
  def test_clangc_default_diagnostic_display_options
    assert_instance_of Fixnum, Clangc.default_diagnostic_display_options
    default_display_options = Clangc::DiagnosticDisplayOptions::Displaysourcelocation|
                              Clangc::DiagnosticDisplayOptions::Displaycolumn|
                              Clangc::DiagnosticDisplayOptions::Displayoption
    assert_equal default_display_options, Clangc.default_diagnostic_display_options
  end
end
