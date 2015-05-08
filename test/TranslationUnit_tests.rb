#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestTranslationUnitCreation < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
  end
  def test_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(@source_file,[""])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(@source_file,[])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[@source_file])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_create_TU
    tu = @cindex.create_translation_unit(@ast_file)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  # return an error code if the Translation Unit creation fail
  def test_create_TU2

  end
  def test_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(@source_file,[], options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2

  end
end
