#!/usr/bin/env ruby
require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

index = Clangc::Index.new(false, false)

tu = index.create_translation_unit_from_source_file(source_file, 
                                                    clang_headers_path)
exit unless tu

Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
  if cursor.location.spelling[0].name == source_file
    puts "#{cursor.spelling} #{cursor.kind}"
  end
  1/0
  #puts parent.spelling
  Clangc::ChildVisitResult::RECURSE  
end

