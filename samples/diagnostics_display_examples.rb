#!/usr/bin/env ruby
require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"

tu = cindex.parse_translation_unit(source: source, 
                                   args: clang_headers_path, 
                                   flags: :none)

exit unless tu

tu.diagnostics.each_with_index do |diagnostic, index|
  puts "################### Diagnostic N° #{index + 1 } #####################"
  puts "Default display options:"
  puts  "\t #{diagnostic.format(Clangc::default_diagnostic_display_options)}"
  puts "None:"
  puts  "\t #{diagnostic.format(0)}"
  puts "None + Source Location:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION
  puts  "\t #{diagnostic.format(:display_source_location)}"
  puts "None + Source Location + Column:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN
  puts "\t #{diagnostic.format([:display_source_location, :display_column])}"
  puts "None + Source Location + Column + Category Name:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_CATEGORY_NAME
  puts "\t #{diagnostic.format([:display_source_location,
                                :display_column,
                                :display_category_name])}"
end
