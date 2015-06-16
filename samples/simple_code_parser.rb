#!/usr/bin/env ruby

require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

@clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
@source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"
options = Clangc::TranslationUnit_Flags::None

tu = cindex.parse_translation_unit(@source, @clang_headers_path, options)

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
