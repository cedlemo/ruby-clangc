#!/usr/bin/env ruby
#
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2015-2016 Cédric Le Moigne cedlemo <cedlemo@gmx.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

require "minitest/autorun"
require "clangc"

class TestIndexCreation < MiniTest::Test
  def test_new_index_dont_exclude_decl_from_pch_and_dont_display_diagnostics
    assert_equal(Clangc::Index, Clangc::Index.new(false, false).class)
  end

  def test_new_index_exclude_decl_from_pch_and_dont_display_diagnostics
    assert_equal(Clangc::Index, Clangc::Index.new(true, false).class)
  end

  def test_new_index_dont_exclude_decl_from_pch_and_display_diagnostics
    assert_equal(Clangc::Index, Clangc::Index.new(false, true).class)
  end

  def test_new_index_exclude_decl_from_pch_and_display_diagnostics
    assert_equal(Clangc::Index, Clangc::Index.new(true, true).class)
  end
end

class TestIndexGlobalOptions < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(true, true)
  end

  def test_index_set_global_options_none
    flags = Clangc::GlobalOptFlags::NONE
    @cindex.global_options = flags
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_none_sym
    flags = Clangc::GlobalOptFlags::NONE
    @cindex.global_options = :none
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_editing
    flags = Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_EDITING
    @cindex.global_options = flags
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_editing_sym
    flags = Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_EDITING
    @cindex.global_options = :thread_background_priority_for_editing
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_editing_and_indexing_sym
    flags = Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_EDITING |
            Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_INDEXING
    @cindex.global_options = [:thread_background_priority_for_editing,
                              :thread_background_priority_for_indexing]
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_and_editing
    flags =  @cindex.global_options |
             Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_EDITING
    @cindex.global_options = flags
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_and_indexing
    flags =  @cindex.global_options |
             Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_INDEXING
    @cindex.global_options = flags
    assert_equal(flags, @cindex.global_options)
  end

  def test_index_set_global_options_all
    flags =   @cindex.global_options |
              Clangc::GlobalOptFlags::THREAD_BACKGROUND_PRIORITY_FOR_ALL
    @cindex.global_options = flags
    assert_equal(flags, @cindex.global_options)
  end
end
