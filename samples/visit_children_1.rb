#!/usr/bin/env ruby

require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

index = Clangc::Index.new(false, false)

tu = index.create_translation_unit_from_source_file(source_file, 
                                                    clang_headers_path)
exit unless tu
cursor = tu.cursor

def pretty_print(cursor_kind, cursor)
  printf("%s %s line %d, char %d\n",
         cursor_kind,
         cursor.spelling,
         cursor.location.spelling[1],
         cursor.location.spelling[2])
end

Clangc.visit_children(cursor: cursor) do |cursor, parent| 
  if cursor.location.spelling[0].name == source_file
    case cursor.kind 
    when Clangc::CursorKind::TYPEDEF_DECL
      pretty_print("typedef     ", cursor)
    when Clangc::CursorKind::STRUCT_DECL
      pretty_print("structure   ", cursor)
    when Clangc::CursorKind::ENUM_DECL
      pretty_print("Enumeration ", cursor)
    when Clangc::CursorKind::UNION_DECL
      pretty_print("Union       ", cursor)
    when Clangc::CursorKind::FUNCTION_DECL
      pretty_print("Function    ", cursor)
      arguments = cursor.type.arg_types
      puts "\t#{arguments.size} argument(s)"
        arguments.each do |a|
        puts "\t-\t" + a.spelling 
      end
    end
  end
  Clangc::ChildVisitResult::RECURSE
end
