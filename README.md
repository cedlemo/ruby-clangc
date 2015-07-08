# ruby-clangc

ruby bindings to the clang C interface

Just a hobby project for now.
This is free software shared under the GNU GPL 3.

# Installation:

    gem build
    gem install clangc-x.x.x.gem

# Status:
*    48/249 functions wrapped => 19.27710843373494%
*    CXIndex
*    CXTranslationUnit
*    CXDiagnostic
*    CXFile
*    CXSourceRange
*    CXSourceLocation
*    CXCursor

# Examples (see samples directory): 

```ruby
#!/usr/bin/env ruby

require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"
options = Clangc::TranslationUnit_Flags::None

tu = cindex.parse_translation_unit(source, clang_headers_path, options)

exit unless tu

tu.diagnostics.each do |diagnostic|
  puts "Default:"
  puts  "\t #{diagnostic.format(Clangc::default_diagnostic_display_options)}"
  puts "None"
  puts  "\t #{diagnostic.format(0)}"
  puts "None + Source Location"
  puts  "\t #{diagnostic.format(Clangc::DiagnosticDisplayOptions::Displaysourcelocation)}"
  puts "None + Source Location + Column"
  puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::Displaysourcelocation|
                                Clangc::DiagnosticDisplayOptions::Displaycolumn)}"
  puts "None + Source Location + Column + Category Name"
  puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::Displaysourcelocation|
                                Clangc::DiagnosticDisplayOptions::Displaycolumn|
                                Clangc::DiagnosticDisplayOptions::Displaycategoryname)}"
end
```
