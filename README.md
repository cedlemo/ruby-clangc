# ruby-clangc

[![Build Status](https://travis-ci.org/cedlemo/ruby-clangc.svg?branch=master)](https://travis-ci.org/cedlemo/ruby-clangc)

ruby bindings to the clang C interface

This is free software shared under the GNU GPL 3.
Those bindings have been tested and work with :

*    Clang v3.5 to v3.8.
*    ruby 2.1 to 2.3



*  [Installation](#installation)
  *  [On Your system](#on-your-system)
  *  [With Virtual Box and Vagrant](#with-virtual-box-and-vagrant)
    *  [ArchLinux:](#archlinux)
    *  [Fedora](#fedora)
*  [Examples](#examples)
  *  [Code completion](#code-completion)
  *  [C and C++ parser](#c-and-c++-parser)
  *  [C simple parsing](#c-simple-parsing)
  *  [Displaying code diagnostics](#displaying-code-diagnostics)
*  [Status](#status)

## Installation

### On your system
This is not recommended, no work on compatibility on other distributions or OS have been done.

```bash
    gem build clanc.gemspec
    gem install clangc-x.x.x.gem
```

### With Virtual Box and Vagrant

#### ArchLinux

```bash
    vagrant box add archlinux-x86_64 http://cloud.terry.im/vagrant/archlinux-x86_64.box
    mkdir ruby_clang_test
    cd ruby_clang_test
    wget https://raw.githubusercontent.com/cedlemo/ruby-clangc/master/ArchLinux_x86_64_Vagrantfile
    vagrant up
    vagrant provision
```
#### Fedora

```bash
    vagrant add bento/fedora-23
    mkdir ruby_clang_test
    cd ruby_clang_test
    wget https://raw.githubusercontent.com/cedlemo/ruby-clangc/master/Fedora-23_Vagrantfile
    vagrant up
    vagrant provision
```

## Examples 
See in the samples directory

### Code completion

```ruby
cindex = Clangc::Index.new(false, false)
tu = cindex.create_translation_unit(source: filename,
                                    args: args)

reparse_options = tu.default_reparse_options
tu.reparse(reparse_options)

options = [:include_code_patterns, :include_macros, :include_brief_comments]
complete_results = tu.code_complete_at(filename,
                                       line,
                                       column,
                                       options)
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
```

###  C and C++ parser

```ruby
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

# This will display the names of all the functions in the header Index.h

cl35.parse do |tu, cursor, parent|
  if cl35.cursor_in_main_file?(cursor)
    puts cursor.spelling if cursor.kind == Clangc::CursorKind::FUNCTION_DECL
  end
  Clangc::ChildVisitResult::RECURSE
end
```

### C simple parsing

```ruby
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
```

### Displaying code diagnostics

```ruby
#!/usr/bin/env ruby
require "clangc"

# excludeDeclsFromPCH = 0, displayDiagnostics=0
cindex = Clangc::Index.new(false, false)

clang_headers_path = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
source = "#{File.expand_path(File.dirname(__FILE__))}/list.c"

tu = cindex.parse_translation_unit(source: source, 
                                   args: clang_headers_path, 
                                   flags: :none)

exit unless tu

tu.diagnostics.each_with_index do |diagnostic, index|
  puts "################### Diagnostic N° #{index + 1 } #####################"
  puts "Default display options:"
  puts  "\t #{diagnostic.format(Clangc::default_diagnostic_display_options)}"
  puts "None:"
  puts  "\t #{diagnostic.format(0)}"
  puts "None + Source Location:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION
  puts  "\t #{diagnostic.format(:display_source_location)}"
  puts "None + Source Location + Column:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN
  puts "\t #{diagnostic.format([:display_source_location, :display_column])}"
  puts "None + Source Location + Column + Category Name:"
  # Clangc::DiagnosticDisplayOptions::DISPLAY_SOURCE_LOCATION |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_COLUMN |
  # Clangc::DiagnosticDisplayOptions::DISPLAY_CATEGORY_NAME
  puts "\t #{diagnostic.format([:display_source_location,
                                :display_column,
                                :display_category_name])}"
end
```

## Status

### functions wrapped:

*    181/257 functions wrapped => 70.42801556420234%

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
*    CXCompletionString
*    CXCodeCompleteResults
*    CXCompletionResult

