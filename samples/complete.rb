#!/usr/bin/env ruby
require "clangc"
# Adpatation of https://gist.github.com/Rip-Rip/758615
# not yet tested
#
PATH = File.expand_path(File.dirname(__FILE__)) 
CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

filename = ARGV[0]
line = ARGV[1].to_i
column = ARGV[2].to_i

if ARGV.size > 3
  args = ARGV[3..(- 1)] + CLANG_HEADERS_PATH
else
  args = CLANG_HEADERS_PATH
end

cindex = Clangc::Index.new(false, false)
tu = cindex.create_translation_unit_from_source_file(filename, 
                                                     args)

if tu == nil
  puts "Failed to create a translation unit"
  exit 1
end

options = Clangc.default_code_complete_options
complete_results = tu.code_complete_at(filename,
                                       line,
                                       column,
                                       options)
if complete_results == nil
  puts "Failed to complete"
  exit 1
end

complete_results.sort_results
complete_results.diagnostics.each do |d|
  puts d.spelling
end

complete_results.results.each do |r|
  r.completion_string.chunk_texts.each do |c|
    puts c
  end
end
