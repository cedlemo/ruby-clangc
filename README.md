# ruby-clangc

ruby bindings to the clang C interface

This is free software shared under the GNU GPL 3.
Clang version used is 3.7.1. and this have been tested only on ArchLinux.

## Installation:

### On your system.
This is not recommended, no work on compatibility on other distributions or OS have been done.
<!-- language: bash -->

    gem build clanc.gemspec
    gem install clangc-x.x.x.gem

### In a Virtual Machine (Virtual Box) with Vagrant
<!-- language: bash -->

    vagrant box add archlinux-x86_64 http://cloud.terry.im/vagrant/archlinux-x86_64.box
    mkdir ruby_clang_test
    cd ruby_clang__test
    wget https://raw.githubusercontent.com/cedlemo/ruby-clangc/master/Vagrantfile
    vagrant up
    vagrant provision

## Status:

### functions wrapped:

*    154/252 functions wrapped => 61.111111111111114%

### classes wrapped:

*    CXIndex
*    CXTranslationUnit
*    CXDiagnostic
*    CXFile
*    CXSourceRange
*    CXSourceLocation
*    CXCursor
*    CXType
*    CXCursorSet
*    CXModule

## Examples (see samples directory): 

1.   C/C++ parser

<!-- language: ruby -->

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
        @index = Clangc::Index.new(false, false)
        @translation_unit = @index.create_translation_unit_from_source_file(source_file,
                                                                            args)
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
        gcc_lib = Dir.glob(gcc_lib_base ).sort.last + "/include"
        header_paths << gcc_lib
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

    source = "#{PATH}/../tools/clang-3.4/Index.h"

    cl34 = CSourceParser.new(source)

    unless cl34.translation_unit
      puts "Parsing failed"
    end

    # This will display the names of all the functions in the header Index.h

    cl34.parse do |tu, cursor, parent|
      if cl34.cursor_in_main_file?(cursor)
        puts cursor.spelling if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
      end
      Clangc::ChildVisitResult::RECURSE
    end


2.    C simple parsing

<!-- language: ruby -->

    #!/usr/bin/env ruby

    require "clangc"

    source_file = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"

    # Get all the necessary headers
    clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}

    index = Clangc::Index.new(false, false)

    tu = index.create_translation_unit_from_source_file(source_file, 
                                                        clang_headers_path)
    exit unless tu
    cursor = tu.cursor

    def pretty_print(cursor_kind, cursor)
      printf("%s %s line %d, char %d\n",
             cursor_kind,                 
             cursor.spelling,
             cursor.location.spelling[1],
             cursor.location.spelling[2])
    end

    Clangc.visit_children(cursor: cursor) do |cursor, parent| 
      if cursor.location.spelling[0].name == source_file
        case cursor.kind 
        when Clangc::CursorKind::TYPEDEF_DECL
          pretty_print("typedef     ", cursor)
        when Clangc::CursorKind::STRUCT_DECL
          pretty_print("structure   ", cursor)
        when Clangc::CursorKind::ENUM_DECL
          pretty_print("Enumeration ", cursor)
        when Clangc::CursorKind::UNION_DECL
          pretty_print("Union       ", cursor)
        when Clangc::CursorKind::FUNCTION_DECL
          pretty_print("Function    ", cursor)
          arguments = cursor.type.arg_types
          puts "\t#{arguments.size} argument(s)"
            arguments.each do |a|
            puts "\t-\t" + a.spelling 
          end
        end
      end
      Clangc::ChildVisitResult::RECURSE
    end


3.    Deal with Clangc diagnostic formats

<!-- language: ruby -->

    #!/usr/bin/env ruby

    require "clangc"

    # excludeDeclsFromPCH = 0, displayDiagnostics=0
    cindex = Clangc::Index.new(false, false)

    clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
    source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"
    options = Clangc::TranslationUnit_Flags::NONE

    tu = cindex.parse_translation_unit(source, clang_headers_path, options)

    exit unless tu

    tu.diagnostics.each do |diagnostic|
      puts "Default:"
      puts  "\t #{diagnostic.format(Clangc::default_diagnostic_display_options)}"
      puts "None"
      puts  "\t #{diagnostic.format(0)}"
      puts "None + Source Location"
      puts  "\t #{diagnostic.format(Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION)}"
      puts "None + Source Location + Column"
      puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION|
                                    Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN)}"
      puts "None + Source Location + Column + Category Name"
      puts "\t #{diagnostic.format( Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION|
                                    Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN|
                                    Clangc::DiagnosticDisplayOptions::DISPLAY_CATEGORY_NAME)}"
    end
