#!/usr/bin/env ruby

require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.cpp"
clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

index = Clangc::Index.new(false, true)

tu = index.create_translation_unit_from_source_file(source_file, 
                                                    ['-x'] +['c++'] +
                                                    clang_headers_path)
exit unless tu
cursor = tu.cursor

Clangc.visit_children(cursor: cursor) do |cursor, parent| 
  puts cursor
  puts parent
end

myproc = proc do |cursor, parent| 
  puts cursor
  puts parent
end
Clangc.visit_children(cursor: cursor, visitor: myproc)

