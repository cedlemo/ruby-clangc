require "clangc"
require "fileutils"

PATH = File.expand_path(File.dirname(__FILE__))

class SourceParser
  attr_reader :index, :source_file, :base_dir, 
              :translation_unit, :diagnostics
  def initialize(source_file, base_dir = nil, lang = "c")
    @source_file = source_file
    @base_dir = base_dir
    include_libs = build_default_include_libs
    args = ["-x", lang] + include_libs
    puts args
    @index = Clangc::Index.new(false, false)
    @translation_unit = @index.create_translation_unit(source: source_file,
                                                       args: args)
    @diagnostics = @translation_unit.diagnostics if @translation_unit
  end
  
  def parse
    cursor = @translation_unit.cursor
    Clangc.visit_children(cursor: cursor) do |cxcursor, parent|
      if block_given?
        yield(@translation_unit, cxcursor, parent)
      else
        puts "Please provide a block"
      end
    end
  end
  
  # Check if the cursor given in argument focus on 
  # the file we want to parse and not on included
  # headers
  def cursor_in_main_file?(cursor)
    cursor_file = cursor.location.spelling[0]
    main_file = @translation_unit.file(@source_file)
    cursor_file.is_equal(main_file)
  end

  private

  # Add the directories where the default headers files
  # for the standards libs can be found
  def build_default_include_libs
    header_paths = []
    if @base_dir && Dir.exist?(@base_dir)
      @base_dir = File.expand_path(@base_dir)
    else
      @base_dir = File.expand_path(File.dirname(@source_file))
    end
    gcc_lib_base='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
    last_gcc_lib_base = Dir.glob(gcc_lib_base ).sort.last
    if last_gcc_lib_base
      gcc_lib = last_gcc_lib_base + "/include"
      header_paths << gcc_lib
    end
    header_paths << "/usr/include"
    header_paths << @base_dir
    header_paths.collect {|h| "-I#{h}"}
  end
end

class CSourceParser < SourceParser
  def initialize(source_file, base_dir = nil)
    super(source_file, base_dir, "c")
  end
end

class CPPSourceParser < SourceParser
  def initialize(source_file, base_dir = nil)
    super(source_file, base_dir, "c++")
  end
end

source = "#{PATH}/../tools/clang-3.5/Index.h"

cl35 = CSourceParser.new(source)

unless cl35.translation_unit
  puts "Parsing failed"
end

cl35.parse do |tu, cursor, parent|
  if cl35.cursor_in_main_file?(cursor)
    puts cursor.spelling if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
  end
  Clangc::ChildVisitResult::RECURSE
end
