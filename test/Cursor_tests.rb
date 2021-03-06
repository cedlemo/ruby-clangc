#!/usr/bin/env ruby
# ruby-clangc ruby bindings for the C interface of Clang
# Copyright (C) 2015-2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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

require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"

class TestCursorUsage < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  def test_translation_unit_get_cursor
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_instance_of(Clangc::Cursor, tu.cursor)
  end

  def test_cursor_is_null
    cursor = Clangc.null_cursor
    assert_equal(true, cursor.is_null)
  end

  def test_cursor_is_not_null
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(false, tu.cursor.is_null)
  end

  def test_cursor_is_equal
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    cursor = tu.cursor
    cursor1 = tu.cursor
    assert_equal(false, cursor.is_equal(Clangc.null_cursor))
    assert_equal(true, cursor.is_equal(cursor1))
  end

  def test_cursor_hash
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    hash = tu.hash
    assert_instance_of(Fixnum, hash)
  end

  def test_cursor_get_kind
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      parent_kind_found = false
      cursor_kind_found = false
      Clangc::CursorKind.constants.each do |c|
        if parent.kind == Clangc::CursorKind.const_get(c)
          parent_kind_found = true
        end
        if cursor.kind == Clangc::CursorKind.const_get(c)
          cursor_kind_found = true
        end
      end
      assert_equal(true, cursor_kind_found)
      assert_equal(true, parent_kind_found)
    end
  end

  def test_cursor_get_linkage
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      parent_linkage_found = false
      cursor_linkage_found = false
      Clangc::LinkageKind.constants.each do |l|
        if parent.linkage == Clangc::LinkageKind.const_get(l)
          parent_linkage_found = true
        end
        if cursor.linkage == Clangc::LinkageKind.const_get(l)
          cursor_linkage_found = true
        end
      end
      assert_equal(true, cursor_linkage_found)
      assert_equal(true, parent_linkage_found)
    end
  end

  def test_cursor_get_availability
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      parent_availability_found = false
      cursor_availability_found = false
      Clangc::AvailabilityKind.constants.each do |l|
        if parent.availability == Clangc::AvailabilityKind.const_get(l)
          parent_availability_found = true
        end
        if cursor.availability == Clangc::AvailabilityKind.const_get(l)
          cursor_availability_found = true
        end
      end
      assert_equal(true, cursor_availability_found)
      assert_equal(true, parent_availability_found)
    end
  end

  def test_cursor_get_language
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      parent_language_found = false
      cursor_language_found = false
      Clangc::LanguageKind.constants.each do |l|
        if parent.language == Clangc::LanguageKind.const_get(l)
          parent_language_found = true
        end
        if cursor.language == Clangc::LanguageKind.const_get(l)
          cursor_language_found = true
        end
      end
      assert_equal(true, cursor_language_found)
      assert_equal(true, parent_language_found)
    end
  end

  def test_cursor_get_type
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::Type, cursor.type)
      assert_instance_of(Clangc::Type, parent.type)
    end
  end

  def test_cursor_get_translation_unit
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      tu
      assert_instance_of(Clangc::TranslationUnit, tu)
    end
  end

  def test_cursor_get_semantic_parent
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor.semantic_parent)
      assert_instance_of(Clangc::Cursor, parent.semantic_parent)
    end
  end

  def test_cursor_get_lexical_parent
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::Cursor, cursor.lexical_parent)
      assert_instance_of(Clangc::Cursor, parent.lexical_parent)
    end
  end

  def test_cursor_get_location
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::SourceLocation, cursor.location)
      assert_instance_of(Clangc::SourceLocation, parent.location)
    end
  end

  def test_cursor_get_extent
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(Clangc::SourceRange, cursor.extent)
      assert_instance_of(Clangc::SourceRange, parent.extent)
    end
  end

  def test_cursor_get_spelling
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of(String, cursor.spelling)
      assert_instance_of(String, parent.spelling)
    end
  end

  def test_cursor_get_typedef_decl_underlying_type
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::TYPEDEF_DECL
        assert_instance_of(Clangc::Type, cursor.typedef_decl_underlying_type)
      end
    end
  end

  def test_cursor_get_included_file
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::INCLUSION_DIRECTIVE
        assert_instance_of(Clangc::File, cursor.included_file)
        assert_equal(SOURCE_FILE, cursor.included_file.name)
      end
    end
  end

  def test_cursor_is_declaration
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, _line, _pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE && cursor.spelling == "data"
        assert_equal(true, cursor.is_declaration, cursor.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_reference
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, _line, _pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE && cursor.spelling == "struct data"
        assert_equal(true, cursor.is_reference, cursor.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_expression
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, _line, _pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE && cursor.spelling == "printf"
        assert_equal true, cursor.is_expression, cursor.spelling
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_statement
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, line, pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE && line == 9 && pos == 28
        assert_equal(true, cursor.is_statement, cursor.location.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_attribute
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, line, pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE && line == 17 && pos == 25
        assert_equal(true, cursor.is_attribute, cursor.location.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_invalid
    assert_equal(true, Clangc.null_cursor.is_invalid, "Not null")
  end

  def test_cursor_is_translation_unit
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    assert_equal(true, tu.cursor.is_translation_unit)
  end

  def test_cursor_is_preprocessing
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_MACRO,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_equal(true, cursor.is_preprocessing, cursor.location.spelling)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_enum_decl_integer_type
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ENUM,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::ENUM_DECL
        assert_instance_of(Clangc::Type, cursor.enum_decl_integer_type)
        assert_equal(Clangc::TypeKind::U_INT,
                     cursor.enum_decl_integer_type.kind,
                     cursor.enum_decl_integer_type.spelling)
      end
    end
  end

  def test_cursor_get_enum_const_decl_value
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ENUM,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::ENUM_CONSTANT_DECL
        case cursor.spelling
        when "RANDOM"
          assert_equal(0, cursor.enum_const_decl_value, cursor.spelling)
        when "IMMEDIATE"
          assert_equal(1, cursor.enum_const_decl_value, cursor.spelling)
        when "SEARCH"
          assert_equal(2, cursor.enum_const_decl_value, cursor.spelling)
        when "HOP"
          assert_equal(-1, cursor.enum_const_decl_value, cursor.spelling)
        when "TOTO"
          assert_equal(-2, cursor.enum_const_decl_value, cursor.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_enum_const_decl_unsigned_value
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_ENUM,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::ENUM_CONSTANT_DECL
        case cursor.spelling
        when "RANDOM"
          assert_equal(0, cursor.enum_const_decl_unsigned_value)
        when "IMMEDIATE"
          assert_equal(1, cursor.enum_const_decl_unsigned_value)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_field_decl_bit_width
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_STRUCT_BITFIELD,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FIELD_DECL
        case cursor.spelling
        when "opaque"
          assert_equal(1, cursor.field_decl_bit_width, cursor.spelling)
        when "fill_color"
          assert_equal(3, cursor.field_decl_bit_width, cursor.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_num_arguments
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        case cursor.spelling
        when "stupid_function"
          assert_equal(1, cursor.num_arguments, cursor.spelling)
        when "main"
          assert_equal(2, cursor.num_arguments, cursor.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_argument
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        case cursor.spelling
        when "stupid_function"
          arg = cursor.argument(0)
          assert_equal(Clangc::TypeKind::DOUBLE, arg.type.kind,
                       arg.type.spelling)
        when "main"
          arg = cursor.argument(0)
          assert_equal(Clangc::TypeKind::INT, arg.type.kind,
                       arg.type.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_arguments
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        case cursor.spelling
        when "stupid_function"
          args = cursor.arguments
          assert_instance_of(Array, args)
          assert_equal(1, args.size, args.inspect)
          assert_equal(Clangc::TypeKind::DOUBLE, args[0].type.kind,
                       args[0].type.spelling)
        when "main"
          args = cursor.arguments
          assert_instance_of(Array, args)
          assert_equal(2, args.size, args.inspect)
          assert_equal(Clangc::TypeKind::INT, args[0].type.kind,
                       args[0].type.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_decl_obj_c_type_encoding
    arguments = ["-x", "objective-c"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OBJECTC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, _line, _pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE_OBJECTC && cursor.spelling == "tata"
        assert_equal("i", cursor.decl_obj_c_type_encoding, cursor.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_result_type
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        if cursor.spelling == "stupid_function"
          assert_instance_of(Clangc::Type, cursor.result_type)
          assert_equal("int", cursor.result_type.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_bit_field
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_STRUCT_BITFIELD,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FIELD_DECL
        assert_equal(true, cursor.is_bit_field, cursor.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_virtual_base
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_BASE_SPECIFIER
        if cursor.spelling == "class Animal"
          assert_equal(true, cursor.is_virtual_base, cursor.spelling)
        else
          assert_equal(false, cursor.is_virtual_base, cursor.spelling)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_cxx_access_specifier
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_BASE_SPECIFIER
        if cursor.spelling == "class Animal"
          assert_equal(Clangc::CXXAccessSpecifier::CXX_PUBLIC,
                       cursor.cxx_access_specifier,
                       cursor.cxx_access_specifier)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_num_overloaded_decls
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OVERL_FUNC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::OVERLOADED_DECL_REF
        assert_equal(2, cursor.num_overloaded_decls,
                     cursor.location.spelling[1])
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_overloaded_decl
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OVERL_FUNC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::OVERLOADED_DECL_REF
        assert_equal(2, cursor.num_overloaded_decls,
                     cursor.location.spelling[1])
        assert_equal(10, cursor.overloaded_decl(1).location.spelling[1],
                     cursor.overloaded_decl(1).location.spelling[1])
        assert_equal(13, cursor.overloaded_decl(0).location.spelling[1],
                     cursor.overloaded_decl(0).location.spelling[1])
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_overloaded_decls
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OVERL_FUNC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::OVERLOADED_DECL_REF
        assert_equal(2, cursor.overloaded_decls.size)
        assert_equal(10, cursor.overloaded_decls[1].location.spelling[1])
        assert_equal(13, cursor.overloaded_decls[0].location.spelling[1])
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_ib_outlet_collection_type
    arguments = ["-x", "objective-c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_IBOUTLET,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert_equal(false, cursor.kind == Clangc::CursorKind::IB_OUTLET_ATTR,
                   cursor.spelling)
      assert_instance_of(Clangc::Type, cursor.ib_outlet_collection_type)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_usr
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of String, cursor.usr
      # TODO
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_display_name
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_equal(cursor.spelling, cursor.display_name, cursor.display_name)
    end
  end

  def test_cursor_get_referenced
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert_instance_of(Clangc::Cursor, cursor.referenced)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_definition
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of(Clangc::Cursor, cursor.definition)
      # TODO
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_definition
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert([true, false].include?(cursor.is_definition))
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_canonical_cursor
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert_instance_of(Clangc::Cursor, cursor.canonical_cursor)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_obj_c_selector_index
    arguments = ["-x", "objective-c"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OBJECTC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of(Fixnum, cursor.obj_c_selector_index)
      Clangc::ChildVisitResult::RECURSE
      # TODO
    end
  end

  def test_cursor_is_dynamic_call
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert([true, false].include?(cursor.is_dynamic_call))
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_receiver_type
    arguments = ["-x", "objective-c"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OBJECTC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of(Clangc::Type, cursor.receiver_type)
      Clangc::ChildVisitResult::RECURSE
      # TODO
    end
  end

  def test_cursor_get_obj_c_decl_qualifiers
    arguments = ["-x", "objective-c"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OBJECTC,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of Fixnum, cursor.obj_c_decl_qualifiers
      Clangc::ChildVisitResult::RECURSE
      # TODO
    end
  end

  def test_cursor_is_obj_c_optional
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      # TODO
      assert([true, false].include?(cursor.is_obj_c_optional))
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_is_variadic
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VARIADIC_FN,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert([true, false].include?(cursor.is_variadic))
      if cursor.location.spelling[0].name == SOURCE_FILE_VARIADIC_FN
        assert_equal("simple_printf", cursor.spelling) if cursor.is_variadic
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_comment_range
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMMENTS,
                                         args: ["-x", "c"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        assert_instance_of(Clangc::SourceRange, cursor.comment_range)
        start_location = cursor.comment_range.start
        end_location = cursor.comment_range.end
        assert_equal(cursor.location.spelling[0].name,
                     start_location.spelling[0].name,
                     start_location.spelling[0].name)
        assert_equal(1, start_location.spelling[1], start_location.spelling[1])
        assert_equal(7, end_location.spelling[1], end_location.spelling[1])
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_raw_comment_text
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMMENTS,
                                         args: ["-x", "c"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        comment = "/**\n\
 * \\brief this is the brief part of the comment\n\
 * and it's second line.\n\
 *\n\
 *  This is the secon paragrah\n\
 *  \\returns an int\n\
 * */"
        assert_equal(comment, cursor.raw_comment_text)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_brief_comment_text
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_COMMENTS,
                                         args: ["-x", "c"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
        comment = "this is the brief part of the comment and it's second line."
        assert_equal(comment, cursor.brief_comment_text)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_cxx_method_is_pure_virtual
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_METHOD
        assert([true, false].include?(cursor.cxx_method_is_pure_virtual))
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_cxx_method_is_static
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_METHOD
        # TODO
        assert([true, false].include?(cursor.cxx_method_is_static))
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_cxx_method_is_virtual
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_METHOD
        # TODO
        assert([true, false].include?(cursor.cxx_method_is_virtual))
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_cxx_method_is_constant
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_METHOD
        # TODO
        assert([true, false].include?(cursor.cxx_method_is_const))
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_template_cursor_kind
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      kind_found = false
      # TODO
      Clangc::CursorKind.constants.each do |c|
        if cursor.template_cursor_kind == Clangc::CursorKind.const_get(c)
          kind_found = true
        end
      end
      assert(kind_found)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_specialized_cursor_template
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_VIRT_BASE_CLASS,
                                         args: arguments)
    # TODO
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of(Clangc::Cursor, cursor.specialized_cursor_template)
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_completion_string
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.location.spelling[0].name == SOURCE_FILE
        # Not a specific test see CompletionString_tests.rb for that
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_instance_of(Clangc::CompletionString, cursor.completion_string)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_decl_obj_c_property_attributes
    arguments = ["-x", "objective-c"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_OBJECTC,
                                         args: arguments)
    # TODO: find a real source to test
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      file, _line, _pos, _offset = cursor.location.spelling
      if file.name == SOURCE_FILE_OBJECTC &&
         cursor.kind == Clangc::CursorKind::OBJ_C_PROPERTY_DECL
        found = false
        Clangc::ObjCPropertyKind.constants.each do |c|
          attr = cursor.obj_c_property_attributes
          found = true if attr == Clangc::ObjCPropertyKind.const_get(c)
        end
        assert(found)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_overridden_cursors
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_METHOD_OVERRIDE,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      if cursor.kind == Clangc::CursorKind::CXX_METHOD
        # TODO: check because it doesn"t seem to find any overriden methods
        assert_instance_of(Array, cursor.overridden_cursors)
        assert_equal(0, cursor.overridden_cursors.size,
                     cursor.overridden_cursors.size)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_module
    # cindex = Clangc::Index.new(false, false)
    # arguments =  ["-x", "c++", "-fmodules", "-fcxxmodules"] +
    #               CLANG_HEADERS_PATH
    # arguments = ["-x", "c++", "-fmodules", "-fcxxmodules",
    #              "-fmodules-cache-path", "#{PATH}/cache"] +
    #              CLANG_HEADERS_PATH
    # tu = cindex.create_translation_unit(source:SOURCE_FILE_MODULE_IMPORT,
    #                                     args: arguments)
    # Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
    #   if cursor.kind == Clangc::CursorKind::MODULE_IMPORT_DECL
    #     assert(Clangc::Module != cursor.module.class, cursor.spelling)
    #   end
    #   Clangc::ChildVisitResult::RECURSE
    # end
    # TODO
  end

  def test_cursor_get_spelling_name_range
    tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                         args: CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      assert_instance_of(Clangc::SourceRange, cursor.spelling_name_range(0, 0))
      Clangc::ChildVisitResult::RECURSE
    end
  end

  def test_cursor_get_reference_name_range
    # TODO
    arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
    tu = @cindex.create_translation_unit(source: SOURCE_FILE_METHOD_OVERRIDE,
                                         args: arguments)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
      case cursor.kind
      when Clangc::CursorKind::MEMBER_REF
        name_flags = Clangc::NameRefFlags::WANT_QUALIFIER
        source_range = cursor.reference_name_range(name_flags, 0)
        assert_instance_of(Clangc::SourceRange, source_range, cursor.spelling)
        source_range = cursor.reference_name_range(:want_qualifier, 0)
        assert_instance_of(Clangc::SourceRange, source_range, cursor.spelling)
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end

class TestCursorUsagePostVersion35 < MiniTest::Test
  include ClangcUtils

  def setup
    @cindex = Clangc::Index.new(false, false)
    system(*%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE}))
  end

  def teardown
    FileUtils.rm_f(AST_FILE)
  end

  if Clangc.version =~ /clang version 3.(6|7|8)/
    def test_cursor_get_num_template_arguments
      source_file = SOURCE_FILE_FUNCTION_TEMPLATE
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      tu = @cindex.create_translation_unit(source: source_file,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_equal(3, cursor.num_template_arguments, cursor.spelling)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_offset_of_field
      tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FIELD_DECL
          assert_equal(0, cursor.offset_of_field) if cursor.spelling == "index"
        else
          assert_equal(-1, cursor.offset_of_field)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_is_anonymous
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      tu = @cindex.create_translation_unit(source: SOURCE_FILE_ANON_DECLS,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::UNION_DECL &&
           cursor.spelling != "X"
          assert_equal(true, cursor.is_anonymous, cursor.spelling)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_storage_class
      tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                           args: CLANG_HEADERS_PATH)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_equal(Clangc::StorageClass::SC_NONE, cursor.storage_class)
        elsif cursor.kind != Clangc::CursorKind::VAR_DECL
          assert_equal(Clangc::StorageClass::SC_INVALID, cursor.storage_class)
        end
      end
    end

    def test_cursor_get_mangling
      tu = @cindex.create_translation_unit(source: SOURCE_FILE,
                                           args: CLANG_HEADERS_PATH)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.location.spelling[0].name == SOURCE_FILE
          # TODO
          assert_instance_of(String, cursor.mangling)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_argument_kind
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          (0..2).each do |i|
            found = false
            Clangc::TemplateArgumentKind.constants.each do |c|
              arg_kind = cursor.template_argument_kind(i)
              if arg_kind == Clangc::TemplateArgumentKind.const_get(c)
                found = true
              end
            end
            assert(found, cursor.spelling)
          end
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end
    
    def test_cursor_get_template_arguments_kinds
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          cursor.template_arguments_kinds.each do |k|
            found = false
            Clangc::TemplateArgumentKind.constants.each do |c|
              found = true if k == Clangc::TemplateArgumentKind.const_get(c)
            end
            assert(found, cursor.spelling)
          end
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_argument_type
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_equal("float", cursor.template_argument_type(0).spelling)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_arguments_types
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          args = cursor.template_arguments_types
          assert_equal("float", args[0].spelling)
          assert_equal("", args[1].spelling, args[1].spelling)
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_argument_value
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_equal(-7, cursor.template_argument_value(1))
          assert_equal(1, cursor.template_argument_value(2))
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_arguments_values
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          values = cursor.template_arguments_values
          assert_equal(-7, values[1])
          assert_equal(1, values[2])
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_argument_unsigned_value
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE_2
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          assert_equal(2_147_483_649,
                       cursor.template_argument_unsigned_value(1))
          assert_equal(1, cursor.template_argument_unsigned_value(2))
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end

    def test_cursor_get_template_arguments_unsigned_values
      arguments = ["-x", "c++"] + CLANG_HEADERS_PATH
      file_name = SOURCE_FILE_FUNCTION_TEMPLATE_2
      tu = @cindex.create_translation_unit(source: file_name,
                                           args: arguments)
      Clangc.visit_children(cursor: tu.cursor) do |cursor, _parent|
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          values = cursor.template_arguments_unsigned_values
          assert_equal(2_147_483_649, values[1])
          assert_equal(1, values[2])
        end
        Clangc::ChildVisitResult::RECURSE
      end
    end
  end
end
