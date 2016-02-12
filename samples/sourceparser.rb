require "clangc"
require "fileutils"

PATH = File.expand_path(File.dirname(__FILE__))

class SourceParser
  def initialize(source_file, base_dir = nil, lang = "c")
    @source_file = source_file
    @base_dir = base_dir
    include_libs = build_default_include_libs
    args = ["-x", lang] + include_libs
    @index = Clangc::Index.new(false, false)
    @translation_unit = @index.create_translation_unit_from_source_file(source_file, args)
    return nil unless @translation_unit
  end
  
  def parse
    cursor = @translation_unit.cursor
    Clangc.visit_children(cursor: cursor) do |cxcursor, parent|
      if cursor_in_main_file?(cxcursor)
        puts cxcursor.spelling if cxcursor.kind == Clangc::CursorKind::FUNCTION_DECL
      end
      Clangc::ChildVisitResult::RECURSE
    end
  end
  
  def cursor_in_main_file?(cursor)
    cursor_file = cursor.location.spelling[0]
    main_file = @translation_unit.file(@source_file)
    cursor_file.is_equal(main_file)
  end

  private

  def build_default_include_libs
    header_paths = []
    if @base_dir && Dir.exist?(@base_dir)
      @base_dir = File.expand_path(@base_dir)
    else
      @base_dir = File.expand_path(File.dirname(@source_file))
    end
    gcc_lib_base='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
    gcc_lib = Dir.glob(gcc_lib_base ).sort.last + "/include"
    header_paths << gcc_lib
    header_paths << "/usr/include"
    header_paths << @base_dir
    header_paths.collect {|h| "-I#{h}"}
  end
end

source = "#{PATH}/../tools/clang-3.4/Index.h"

cl34 = SourceParser.new(source)
cl34.parse
