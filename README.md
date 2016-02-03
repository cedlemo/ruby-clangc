# ruby-clangc

ruby bindings to the clang C interface

This is free software shared under the GNU GPL 3.
Clang version used is 3.7.1. and this have been tested only on ArchLinux.

## Installation:

### On your system.
This is not recommended, no work on compatibility on other distributions or OS have been done.

    gem build clanc.gemspec
    gem install clangc-x.x.x.gem

### In a Virtual Machine (Virtual Box) with Vagrant
  
    vagrant box add archlinux-x86_64 http://cloud.terry.im/vagrant/archlinux-x86_64.box
    mkdir ruby_clang_test
    cd ruby_clang__test
    wget https://raw.githubusercontent.com/cedlemo/ruby-clangc/master/Vagrantfile
    vagrant up

### Vagrant file content:

    Vagrant.configure(2) do |config|
      config.vm.box = "archlinux-x86_64"

      config.vm.provider "virtualbox" do |vb|
        # Display the VirtualBox GUI when booting the machine
        vb.gui = true
        # Customize the amount of memory on the VM:
        vb.memory = "1024"
      end
      shell_script = <<-SHELL
        yaourt -Syu --noconfirm
        yaourt --noconfirm -S clang llvm llvm-libs git cppunit ruby
        git clone https://github.com/cedlemo/ruby-clangc.git
        cd ruby-clangc
        gem build clangc.gemspec
        gem install clangc-0.0.1.gem
      SHELL
      config.vm.provision "shell", inline: shell_script, privileged: false
    end

## Status:

### functions wrapped:

*    153/252 functions wrapped => 60.71428571428571%

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

```ruby
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
```

