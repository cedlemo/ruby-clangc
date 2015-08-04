#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
class TestTypeUsage < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # C source file with pointer
    @source_file_pointer = "#{File.expand_path(File.dirname(__FILE__))}/source5.c"
    # C source file with qualified type
    @source_file_qualified = "#{File.expand_path(File.dirname(__FILE__))}/source6.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_Type_kind
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
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
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    cursor_type = nil
    parent_type = nil
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of String, cursor.type.spelling
      assert_instance_of String, parent.type.spelling
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Type_is_equal
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_equal false, cursor.type == cursor.type
      assert_equal false, parent.type == parent.type
      assert_equal true, cursor.type.is_equal(cursor.type)
      assert_equal true, parent.type.is_equal(parent.type)
      Clangc::ChildVisitResult::Break
    end
  end
  def test_Type_canonical_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of Clangc::Type, cursor.type.canonical_type
      assert_instance_of Clangc::Type, parent.type.canonical_type
    end
  end
  def test_Type_canonical_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      assert_instance_of Clangc::Type, cursor.type.canonical_type
      assert_instance_of Clangc::Type, parent.type.canonical_type
    end
  end
  def test_Type_pointee_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file_pointer, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      cursor_pointee = cursor.type.pointee_type
      parent_pointee = parent.type.pointee_type
      assert_instance_of Clangc::Type, cursor_pointee
      assert_instance_of Clangc::Type, parent_pointee
      if cursor.type.kind == Clangc::TypeKind::Pointer
        assert_equal Clangc::TypeKind::Int, cursor_pointee.kind, cursor_pointee.spelling
      else
        assert_equal Clangc::TypeKind::Invalid, cursor_pointee.kind, cursor_pointee.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Type_constant_qualified_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file_qualified, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
      if cursor.location.spelling[1] == 1
        assert_equal true, cursor.type.is_const_qualified
      else
        assert_equal false, cursor.type.is_const_qualified
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
#  def test_Cursor_get_typedef_decl_underlying_type
#    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
#    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
#      if cursor.type == Clangc::TypeKind::Typedef
#        assert_instance_of Clangc::Type, cursor.typedef_decl_underlying_type
#      end
#    end
#  end
end
