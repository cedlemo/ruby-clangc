#!/usr/bin/env ruby

require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
source = "#{File.expand_path(File.dirname(__FILE__) + "/../test")}/source3.c"
puts source
tu = cindex.create_translation_unit_from_source_file(source, clang_headers_path)

exit unless tu

def print_location_spelling(source_location)
  spelling = source_location.spelling
  file = spelling[0].name || "none"
  line = spelling[1]
  column = spelling[2]
  offset = spelling[3]

  puts "<#{file} #{line} #{column} #{offset}>"
end
tu.diagnostics.each do |diagnostic|
  diagnostic.source_ranges.each do |sr|
    print_location_spelling(sr.start)
    print_location_spelling(sr.end)
  end
end
