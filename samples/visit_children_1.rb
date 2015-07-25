#!/usr/bin/env ruby

require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

index = Clangc::Index.new(false, false)

tu = index.create_translation_unit_from_source_file(source_file, 
                                                    clang_headers_path)
exit unless tu
cursor = tu.cursor

Clangc.visit_children(cursor: cursor) do |cursor, parent| 
  source_location = cursor.location
  puts "File = #{source_location.spelling[0]}, line #{source_location.spelling[1]}, column #{source_location.spelling[2]}"
  #source_range = cursor.extent
  puts cursor.spelling
end
