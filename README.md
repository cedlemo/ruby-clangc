# ruby-clangc

ruby bindings to the clang C interface

This is free software shared under the GNU GPL 3.
Clang version used are 3.5.0 to 3.8.0

# Installation:

    gem build
    gem install clangc-x.x.x.gem

# Status:

*    133/252 functions wrapped => 52.77777777777778%
*    CXIndex
*    CXTranslationUnit
*    CXDiagnostic
*    CXFile
*    CXSourceRange
*    CXSourceLocation
*    CXCursor
*    CXType
*    CXCursorSet

# Examples (see samples directory): 

```ruby
#!/usr/bin/env ruby

require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"

# Get all the necessary headers
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
```

```ruby
#!/usr/bin/env ruby

require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"
options = Clangc::TranslationUnit_Flags::NONE

tu = cindex.parse_translation_unit(source, clang_headers_path, options)

exit unless tu

tu.diagnostics.each do |diagnostic|
  puts "Default:"
  puts  "\t #{diagnostic.format(Clangc::default_diagnostic_display_options)}"
  puts "None"
  puts  "\t #{diagnostic.format(0)}"
  puts "None + Source Location"
  puts  "\t #{diagnostic.format(Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION)}"
  puts "None + Source Location + Column"
  puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION|
                                Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN)}"
  puts "None + Source Location + Column + Category Name"
  puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION|
                                Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN|
                                Clangc::DiagnosticDisplayOptions::DISPLAY_CATEGORY_NAME)}"
end
```

