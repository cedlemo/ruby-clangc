#!/usr/bin/env ruby
require "minitest/autorun"
require "clangc"
require "fileutils"
class TestCursorUsage < MiniTest::Test
  def setup
    @cindex = Clangc::Index.new(false, false)
    # Good C test file
    @source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
    # C source code with one error
    @source_file_one_error = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
    # C source with macro
    @source_file_macro =  "#{File.expand_path(File.dirname(__FILE__))}/source10.c"
    # C with enum
    @source_file_enum = "#{File.expand_path(File.dirname(__FILE__))}/source11.c"
    # Inexistant file
    @bad_file = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
    @ast_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
    system *%W(clang -emit-ast -o #{@ast_file} #{@source_file})
    @clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
  end
  def teardown
    FileUtils.rm_f(@ast_file)
  end
  def test_TU_get_cursor
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    assert_instance_of Clangc::Cursor, tu.cursor
  end
  def test_Cursor_is_null
    cursor = Clangc.null_cursor
    assert_equal true, cursor.is_null
  end
  def test_Cursor_is_not_null
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    assert_equal false, tu.cursor.is_null
  end
  def test_Cursor_is_equal
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    cursor = tu.cursor
    cursor1 = tu.cursor
    assert_equal false, cursor.is_equal(Clangc.null_cursor)
    assert_equal true, cursor.is_equal(cursor1)
  end
  def test_Cursor_hash
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    cursor = tu.cursor
    hash = tu.hash
    assert_instance_of Fixnum, hash
  end
  def test_Cursor_get_kind
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
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
      assert_equal true, cursor_kind_found
      assert_equal true, parent_kind_found
    end
  end
  def test_Cursor_get_linkage
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
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
      assert_equal true, cursor_linkage_found
      assert_equal true, parent_linkage_found
    end
  end
  def test_Cursor_get_availability
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
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
      assert_equal true, cursor_availability_found
      assert_equal true, parent_availability_found
    end
  end
  def test_Cursor_get_language
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
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
      assert_equal true, cursor_language_found
      assert_equal true, parent_language_found
    end
  end
  def test_Cursor_get_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of Clangc::Type, cursor.type
      assert_instance_of Clangc::Type, parent.type
    end
  end
  def test_Cursor_get_semantic_parent
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor.semantic_parent
      assert_instance_of Clangc::Cursor, parent.semantic_parent
    end
  end
  def test_Cursor_get_lexical_parent
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of Clangc::Cursor, cursor.lexical_parent
      assert_instance_of Clangc::Cursor, parent.lexical_parent
    end
  end
  def test_Cursor_get_location
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of Clangc::SourceLocation, cursor.location
      assert_instance_of Clangc::SourceLocation, parent.location
    end
  end
  def test_Cursor_get_extent
  tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of Clangc::SourceRange, cursor.extent
      assert_instance_of Clangc::SourceRange, parent.extent
    end
  end
  def test_Cursor_get_spelling
  tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      assert_instance_of String, cursor.spelling
      assert_instance_of String, parent.spelling
    end
  end
  def test_Cursor_get_typedef_decl_underlying_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.kind == Clangc::CursorKind::Typedefdecl
        assert_instance_of Clangc::Type, cursor.typedef_decl_underlying_type
      end
    end
  end
  def test_Cursor_get_included_file
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.kind == Clangc::CursorKind::Inclusiondirective
        assert_instance_of Clangc::File, cursor.included_file
        assert_equal @source_file, cursor.included_file.name
      end
    end
  end
  def test_Cursor_is_declaration
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == @source_file && cursor.spelling == "data"
        assert_equal true, cursor.is_declaration, cursor.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_is_reference
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == @source_file && cursor.spelling == "struct data"
        assert_equal true, cursor.is_reference, cursor.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_is_expression
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == @source_file && cursor.spelling == "printf"
        assert_equal true, cursor.is_expression, cursor.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_is_statement
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == @source_file && 
        cursor.location.spelling[1] == 9 && cursor.location.spelling[2] == 28 
        assert_equal true, cursor.is_statement, cursor.location.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_is_attribute
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == @source_file && 
        cursor.location.spelling[1] == 17 && cursor.location.spelling[2] == 25 
        assert_equal true, cursor.is_attribute, cursor.location.spelling
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_is_invalid
    assert_equal true, Clangc.null_cursor.is_invalid, "Not null"
  end
  def test_Cursor_is_translation_unit
    tu = @cindex.create_translation_unit_from_source_file(@source_file, @clang_headers_path)
    assert_equal true, tu.cursor.is_translation_unit
  end
  def test_Cursor_is_preprocessing
    tu = @cindex.create_translation_unit_from_source_file(@source_file_macro, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
        assert_equal true, cursor.is_preprocessing, cursor.location.spelling
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_get_enum_decl_integer_type
    tu = @cindex.create_translation_unit_from_source_file(@source_file_enum, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.kind == Clangc::CursorKind::Enumdecl
        assert_instance_of Clangc::Type, cursor.enum_decl_integer_type
        assert_equal Clangc::TypeKind::Uint, cursor.enum_decl_integer_type.kind, cursor.enum_decl_integer_type.spelling
      end
    end
  end
  def test_Cursor_get_enum_const_decl_value
    tu = @cindex.create_translation_unit_from_source_file(@source_file_enum, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.kind == Clangc::CursorKind::Enumconstantdecl
        case cursor.spelling
        when "RANDOM"
          assert_equal 0, cursor.enum_const_decl_value, cursor.spelling
        when "IMMEDIATE"
          assert_equal 1, cursor.enum_const_decl_value, cursor.spelling
        when "SEARCH"
          assert_equal 2, cursor.enum_const_decl_value, cursor.spelling
        when "HOP"
          assert_equal -1, cursor.enum_const_decl_value, cursor.spelling
        when "TOTO"
          assert_equal -2, cursor.enum_const_decl_value, cursor.spelling
        end
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
  def test_Cursor_get_enum_const_decl_unsigned_value
    tu = @cindex.create_translation_unit_from_source_file(@source_file_enum, @clang_headers_path)
    Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
      if cursor.kind == Clangc::CursorKind::Enumconstantdecl
        case cursor.spelling
        when "RANDOM"
          assert_equal 0, cursor.enum_const_decl_unsigned_value, cursor.spelling
        when "IMMEDIATE"
          assert_equal 1, cursor.enum_const_decl_unsigned_value, cursor.spelling
        end
      end
      Clangc::ChildVisitResult::Recurse
    end
  end
end
