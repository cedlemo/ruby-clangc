#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestTranslationUnitCreation < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
  end
  def test_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(@source_file,[""])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file
    tu = @cindex.create_translation_unit_from_source_file(@bad_file,[""])
    assert_nil tu 
  end
  def test_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(@source_file,[])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_empty_cmdline
    tu = @cindex.create_translation_unit_from_source_file(@bad_file,[])
    assert_nil tu 
  end
  def test_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[@source_file])
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU_from_source_file_in_cmdline
    tu = @cindex.create_translation_unit_from_source_file("",[@bad_file])
    assert_nil tu 
  end
  def test_create_TU
    tu = @cindex.create_translation_unit(@ast_file)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU
    tu = @cindex.create_translation_unit(@bad_file)
    assert_nil tu 
  end
  # return an error code if the Translation Unit creation fail
  def test_create_TU2
    tu = @cindex.create_translation_unit2(@ast_file)
    assert_instance_of Clangc::TranslationUnit, tu 
  end
  def test_fail_create_TU2
    tu = @cindex.create_translation_unit2(@bad_file)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
  def test_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(@source_file,[], options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_fail_parse_TU
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit(@bad_file,[], options)
    assert_nil tu
  end
  # return an error code if the Translation Unit creation fail
  def test_parse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(@source_file,[], options)
    assert_instance_of Clangc::TranslationUnit, tu
  end
  def test_failparse_TU2
    options = Clangc::TranslationUnit_Flags::None
    tu = @cindex.parse_translation_unit2(@bad_file,[], options)
    assert_equal Integer, tu.class.superclass
    assert_equal Clangc::ErrorCode::Failure, tu
  end
end
