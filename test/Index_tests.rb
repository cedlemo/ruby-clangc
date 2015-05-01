#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"

class TestIndexCreation < MiniTest::Test
  def test_new_index_dont_exclude_decl_from_pch_and_dont_display_diagnostics
    assert_equal Clangc::Index, Clangc::Index.new(false, false).class
  end
  def test_new_index_exclude_decl_from_pch_and_dont_display_diagnostics
    assert_equal Clangc::Index, Clangc::Index.new(true, false).class
  end
  def test_new_index_dont_exclude_decl_from_pch_and_display_diagnostics
    assert_equal Clangc::Index, Clangc::Index.new(false, true).class
  end
  def test_new_index_exclude_decl_from_pch_and_display_diagnostics
    assert_equal Clangc::Index, Clangc::Index.new(true, true).class
  end
end

# index global options test
#Clangc::GlobalOptFlags.constants
#=> [:None, :Threadbackgroundpriorityforindexing, :Threadbackgroundpriorityforediting, :Threadbackgroundpriorityforall]

class TestIndexGlobalOptions < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(true, true)
  end
  def test_index_set_global_options_None
    flags = Clangc::GlobalOptFlags::None
    @cindex.global_options = flags
    assert_equal flags, @cindex.global_options
  end
  def test_index_set_global_options_editing
    flags =  Clangc::GlobalOptFlags::Threadbackgroundpriorityforediting
    @cindex.global_options = flags
    assert_equal flags, @cindex.global_options
  end
end
