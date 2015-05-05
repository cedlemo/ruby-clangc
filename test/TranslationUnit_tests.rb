#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestTranslationUnitCreation < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    @source_file = "#{File.basename(__FILE__)}/source1.c"
    @ast_file = "#{File.basename(__FILE__)}/source1.ast"
  end
  def test_create_TU_from_source_file

  end
  def test_create_TU_from_source_file_in_cmdline

  end
  def test_create_TU

  end
  # return an error code if the Translation Unit creation fail
  def test_create_TU2

  end
  def test_parse_TU

  end
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2

  end
end
