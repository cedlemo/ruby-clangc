#!/usr/bin/env ruby
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

require "minitest/autorun"
require "clangc"
require "fileutils"
require "#{File.expand_path(File.dirname(__FILE__))}/clangc_utils.rb"
require "fiddle"

class TestTypeUsage < MiniTest::Test
  include ClangcUtils
  def setup
    @cindex = Clangc::Index.new(false, false)
    system *%W(clang -emit-ast -o #{AST_FILE} #{SOURCE_FILE})
  end
  def teardown
    FileUtils.rm_f(AST_FILE)
  end
  def test_Type_kind
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      parent_kind_found = false
      cursor_kind_found = false
      Clangc::TypeKind.constants.each do |c|
        if cursor.type.kind == Clangc::TypeKind.const_get(c)
          cursor_kind_found = true
        end
        if parent.type.kind == Clangc::TypeKind.const_get(c)
          parent_kind_found = true
        end
      end
      assert_equal true, cursor_kind_found
      assert_equal true, parent_kind_found
    end
  end
  def test_Type_spelling
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    cursor_type = nil
    parent_type = nil
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of String, cursor.type.spelling
      assert_instance_of String, parent.type.spelling
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_is_equal
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_equal false, cursor.type == cursor.type
      assert_equal false, parent.type == parent.type
      assert_equal true, cursor.type.is_equal(cursor.type)
      assert_equal true, parent.type.is_equal(parent.type)
      Clangc::ChildVisitResult::BREAK
    end
  end
  def test_Type_canonical_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of Clangc::Type, cursor.type.canonical_type
      assert_instance_of Clangc::Type, parent.type.canonical_type
    end
  end
  def test_Type_canonical_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of Clangc::Type, cursor.type.canonical_type
      assert_instance_of Clangc::Type, parent.type.canonical_type
    end
  end
  def test_Type_pointee_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_POINTER, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      cursor_pointee = cursor.type.pointee_type
      parent_pointee = parent.type.pointee_type
      assert_instance_of Clangc::Type, cursor_pointee
      assert_instance_of Clangc::Type, parent_pointee
      if cursor.type.kind == Clangc::TypeKind::POINTER
        assert_equal Clangc::TypeKind::INT, cursor_pointee.kind, cursor_pointee.spelling
      else
        assert_equal Clangc::TypeKind::INVALID, cursor_pointee.kind, cursor_pointee.spelling
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_constant_qualified_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_QUALIFIED, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[1] == 1
        assert_equal true, cursor.type.is_const_qualified
      else
        assert_equal false, cursor.type.is_const_qualified
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_volatile_qualified_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_QUALIFIED, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
    location = cursor.location.spelling
    if location[1] == 3 && location[2] == 14
        assert_equal true, cursor.type.is_volatile_qualified, cursor.location.spelling[2]
      else
        assert_equal false, cursor.type.is_volatile_qualified, cursor.location.spelling[2]
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_restrict_qualified_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_QUALIFIED, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
    location = cursor.location.spelling
      if location[1] == 2 && location[2] == 15
        assert_equal true, cursor.type.is_restrict_qualified, location[2]
      else
       assert_equal false, cursor.type.is_restrict_qualified, location[2]
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_function_return_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_FUNCTION, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
    location = cursor.location.spelling
      if cursor.type.kind == Clangc::TypeKind::FUNCTION_PROTO
        assert_equal Clangc::TypeKind::INT, cursor.type.result_type.kind
      elsif cursor.type.kind == Clangc::TypeKind::FUNCTION_NO_PROTO
        assert_equal Clangc::TypeKind::INT, cursor.type.result_type.kind
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_function_calling_conv
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_FUNCTION, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      cursor_kind = cursor.type.kind
      if cursor_kind == Clangc::TypeKind::FUNCTION_NO_PROTO || cursor_kind == Clangc::TypeKind::FUNCTION_PROTO
        convention_found = false
        Clangc::CallingConv.constants.each do |c|
          convention_found = true if Clangc::CallingConv.const_get(c) == cursor.type.calling_conv
        end
        assert_equal true, convention_found
      end
    end
  end
  def test_Type_function_num_arg_types
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_FUNCTION, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
    ck = cursor.type.kind
      if ck == Clangc::TypeKind::FUNCTION_PROTO || ck == Clangc::TypeKind::FUNCTION_NO_PROTO
        assert_equal 2, cursor.type.num_arg_types
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_function_arg_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_FUNCTION, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
    ck = cursor.type.kind
      if ck == Clangc::TypeKind::FUNCTION_PROTO || ck == Clangc::TypeKind::FUNCTION_NO_PROTO
        assert_equal Clangc::TypeKind::CHAR_S, cursor.type.arg_type(0).kind, cursor.type.arg_type(0).kind
        assert_equal Clangc::TypeKind::INT, cursor.type.arg_type(1).kind, cursor.type.arg_type(1).kind
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_function_arg_types
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_FUNCTION, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      ck = cursor.type.kind
      if ck == Clangc::TypeKind::FUNCTION_PROTO || ck == Clangc::TypeKind::FUNCTION_NO_PROTO
        args = cursor.type.arg_types
        assert_equal Clangc::TypeKind::CHAR_S, args[0].kind
        assert_equal Clangc::TypeKind::INT, args[1].kind
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_element_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ARRAY, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::CONSTANT_ARRAY, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal Clangc::TypeKind::INT, cursor.type.element_type.kind, cursor.location.spelling.inspect
      elsif cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::VECTOR, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal Clangc::TypeKind::INT, cursor.type.element_type.kind, cursor.location.spelling.inspect
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_num_elements
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ARRAY, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::CONSTANT_ARRAY, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal 10, cursor.type.num_elements, cursor.location.spelling.inspect
      elsif cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::VECTOR, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal 2, cursor.type.num_elements, cursor.location.spelling.inspect
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_array_element_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ARRAY, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::CONSTANT_ARRAY, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal Clangc::TypeKind::INT, cursor.type.array_element_type.kind, cursor.location.spelling.inspect
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_array_size
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ARRAY, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
        assert_equal Clangc::TypeKind::CONSTANT_ARRAY, cursor.type.kind, cursor.location.spelling.inspect
        assert_equal 10, cursor.type.array_size, cursor.location.spelling.inspect
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_is_pod
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_POD, ['-x', 'c++'] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[0].name == SOURCE_FILE_POD # ensure this is not include file
        if cursor.type.kind == Clangc::TypeKind::INT
          assert_equal true, cursor.type.is_pod, cursor.location.spelling.inspect + cursor.spelling
        elsif cursor.type.kind != Clangc::TypeKind::INVALID && cursor.type.kind != Clangc::TypeKind::INT
          assert_equal false, cursor.type.is_pod, cursor.location.spelling.inspect + cursor.spelling
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_type_declaration
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE, CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
        type = cursor.type
        decl = type.type_declaration
        assert_instance_of(Clangc::Cursor, decl)
    # TODO use real test case
    Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_is_function_type_variadic
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_VARIADIC_FN, ["-x", "c++"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_VARIADIC_FN
        if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
          type = cursor.type
          if type.is_function_type_variadic
            assert_equal("simple_printf", cursor.spelling)
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_align_of
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ALIGN_OF, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_ALIGN_OF
        case cursor.spelling
        when "toto" 
          assert_equal(Fiddle::ALIGN_CHAR, cursor.type.align_of, cursor.type.align_of)
        when "a"
          assert_equal(Fiddle::ALIGN_INTPTR_T, cursor.type.align_of, cursor.type.align_of)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_size_of
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_ALIGN_OF, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_ALIGN_OF
        case cursor.spelling
        when "toto" 
          assert_equal(Fiddle::SIZEOF_CHAR, cursor.type.size_of, cursor.type.size_of)
        when "a"
          assert_equal(Fiddle::SIZEOF_INTPTR_T, cursor.type.size_of, cursor.type.size_of)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_class_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_MEMBER_PTR, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
     if cursor.location.spelling[0].name == SOURCE_FILE_MEMBER_PTR
        if cursor.spelling == "pwCaption"
        class_type = cursor.type.class_type
        assert_equal(Clangc::TypeKind::RECORD, class_type.kind, cursor.spelling)
        end
     end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_offset_of
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_TYPE_OFFSET_OF, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_TYPE_OFFSET_OF
        if cursor.kind == Clangc::CursorKind::STRUCT_DECL
          assert_equal(Fiddle::SIZEOF_INT * 8, cursor.type.offset_of("f"), cursor.kind)
          assert_equal(Fiddle::SIZEOF_FLOAT * 8 + Fiddle::SIZEOF_INT * 8, cursor.type.offset_of("c"), cursor.kind)
        else
          assert_equal(Clangc::TypeLayoutError::INVALID, cursor.type.offset_of("f"), cursor.kind)
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_num_template_arguments
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION
        type = cursor.type
        if cursor.kind == Clangc::CursorKind::CLASS_DECL  
          assert_equal 1, type.num_template_arguments, type.num_template_arguments
        else
          assert_equal -1, type.num_template_arguments, type.num_template_arguments
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_template_argument_as_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION
        if cursor.kind == Clangc::CursorKind::CLASS_DECL  
          type = cursor.type
          template_argument = type.template_argument_as_type(0)
          assert_equal Clangc::TypeKind::INT, template_argument.kind, template_argument.kind 
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_template_arguments_as_type
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION
        if cursor.kind == Clangc::CursorKind::CLASS_DECL  
          type = cursor.type
          assert_equal 1, type.template_arguments_as_type.size
          template_argument = type.template_arguments_as_type[0]
          assert_equal Clangc::TypeKind::INT, template_argument.kind, template_argument.kind 
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  def test_Type_get_cxx_ref_qualifier
    tu = @cindex.create_translation_unit_from_source_file(SOURCE_FILE_CXX_REF_QUALIFIER, ["-x", "c++", "--std", "c++11"] + CLANG_HEADERS_PATH)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == SOURCE_FILE_CXX_REF_QUALIFIER
        if cursor.kind == Clangc::CursorKind::CXX_METHOD
          type = cursor.type
          line = cursor.location.spelling[1]
          case line
          when 5
            assert_equal Clangc::RefQualifierKind::L_VALUE, type.cxx_ref_qualifier, "#{cursor.spelling} #{line} #{type.cxx_ref_qualifier}" 
          when 6 
            assert_equal Clangc::RefQualifierKind::R_VALUE, type.cxx_ref_qualifier, "#{cursor.spelling} #{line} #{type.cxx_ref_qualifier}" 
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
end
