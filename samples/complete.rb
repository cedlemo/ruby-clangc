#!/usr/bin/env ruby
require "clangc"
# Adpatation of https://gist.github.com/Rip-Rip/758615
# launch in this directory:
#
# ruby complete.rb complete_test.c 14 8
#
# Should Ouput:
#
#    Diagnostics : 
#    Complete :
#    a
#    b
#    c

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


tu = cindex.create_translation_unit(source: filename, args: args)
if tu == nil
  puts "Failed to create a translation unit"
  exit 1
end

reparse_options = tu.default_reparse_options

if tu.reparse(reparse_options) != 0
  puts "Failed to reparse"
  exit 1
end

# Clangc::CodeComplete_Flags
options = [:include_code_patterns, :include_macros, :include_brief_comments]
complete_results = tu.code_complete_at(filename,
                                       line,
                                       column,
                                       options)
if complete_results == nil
  puts "Failed to complete"
  exit 1
end

complete_results.sort_results
puts "Diagnostics : "

complete_results.diagnostics.each do |d|
  puts d.spelling
end

puts "Complete :"
complete_results.results.each do |r|
  r.completion_string.chunk_texts.each_with_index do |c, i|
    if r.completion_string.chunk_kind(i) == Clangc::CompletionChunkKind::TYPED_TEXT
      puts c
    end
  end
end
