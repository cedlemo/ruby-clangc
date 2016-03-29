#!/usr/bin/env ruby
require "clangc"

source_file = "#{File.expand_path(File.dirname(__FILE__))}/completion_string.cpp"
clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

index = Clangc::Index.new(false, true)

tu = index.create_translation_unit_from_source_file(source_file, 
                                                    ['-x'] +['c++'] +
                                                    clang_headers_path)
exit unless tu

def cursor_kind_spelling(cursor)
  default = "unexposed_decl"
  Clangc::CursorKind.constants.each do |c|
    if Clangc::CursorKind.const_get(c) == cursor.kind
      default = c.to_s.downcase
    else
      next
    end
  end
  default
end

def display_cursor_info(cursor)
  code = cursor.spelling
  kind = cursor_kind_spelling(cursor)
  "Cursor spelling : #{code} - kind : #{kind}"
end

def completion_string_availability(cs)
  default = "not_available"
  Clangc::AvailabilityKind.constants.each do |c|
     if Clangc::AvailabilityKind.const_get(c) == cs.availability
      default = c.to_s.downcase
    else
      next
    end
  end
  default
end

def display_completion_string_info(cs, margin = "")
  availability = completion_string_availability(cs)
  priority = cs.priority
  chunks = ""
  (0..(cs.num_chunks - 1)).each do |i|
    chunks += (margin + display_chunk_info(cs, i, margin + "\t"))
  end
  "#{margin}Completion string : \n\
  #{margin}availability : #{availability}\n\
  #{margin}priority :     #{priority}\n\
  #{margin}num chunks :   #{cs.num_chunks}\n\
  #{chunks}"

end

def chunk_kind_string(kind)
  default = "not_available"
  Clangc::CompletionChunkKind.constants.each do |c|
     if Clangc::CompletionChunkKind.const_get(c) == kind
      default = c.to_s.downcase
    else
      next
    end
  end
  default
end

def display_chunk_info(cs, index, margin = "")
  kind = chunk_kind_string(cs.chunk_kind(index))
  text = cs.chunk_text(index)

  if cs.chunk_kind(index) == Clangc::CompletionChunkKind::OPTIONAL
    completion_string = display_completion_string_info(cs.chunk_completion_string(index), margin + "\t") 
  end

  " Chunk infos: \n\
  #{margin}kind #{kind}\n\
  #{margin}text #{text}\n\
  #{completion_string}"
end

Clangc.visit_children(cursor: tu.cursor) do |cursor, parent| 
  if cursor.location.spelling[0].name == source_file 
    puts display_cursor_info(cursor)
    completion_string = cursor.completion_string
    puts display_completion_string_info(completion_string, margin = "\t") if completion_string
  end
  Clangc::ChildVisitResult::RECURSE
end

