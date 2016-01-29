require "fileutils"
require "clangc"
PATH = File.expand_path(File.dirname(__FILE__))
CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
index = Clangc::Index.new(false, false)

tu = index.create_translation_unit_from_source_file("#{PATH}/test.cpp",
                                                    [
                                                     "-x", "c++",
                                                     "-fmodules",
                                                     "-fcxxmodules"#,
                                                     #"-fmodules-cache-path",
                                                     #"#{PATH}/cache_path"
                                                     ] + CLANG_HEADERS_PATH)

Clangc.visit_children(cursor: tu.cursor) do |cursor, parent|
  puts cursor.spelling
  if cursor.kind == Clangc::CursorKind::MODULE_IMPORT_DECL
    puts "This is a module import declaration" 
    if Clangc::Module != cursor.module.class
      puts "#{cursor.spelling} is a module import declaration" 
    end
    Clangc::ChildVisitResult::RECURSE
  end
end
diagnostics = tu.diagnostics
if !diagnostics.empty?
  diagnostics.each do |d|
    puts "Severity #{d.severity}"
    puts "Category name #{d.category_name}"
    d.source_ranges.each do |sr|
      loc_start = sr.start
      sp = loc_start.spelling
      file = sp[0].
      line = sp[1]
      column = sp[2]
      puts "Start : file #{file.name} l:#{line} column:#{column}"
      loc_end = sr.end
      sp = loc_end.spelling
      file = sp[0].
      line = sp[1]
      column = sp[2]
      puts "End file #{file.name} l:#{line} column:#{column}"
    end
    source_location = d.source_location
    sp = source_location.spelling
    file = sp[0]
    line = sp[1]
    column = sp[2]
    puts "Source location file #{file.name} l:#{line} column:#{column}"
  end
end
