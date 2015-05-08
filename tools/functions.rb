#!/usr/bin/env ruby
require "rtruckboris"

clang_c = "/usr/include/clang-c/Index.h"
#headerPaths = `pkg-config --cflags gtk+-3.0`.gsub("-I","").split(" ")
header_paths = []
#get gcc include if needed
gcc_lib_base='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
gcc_lib = Dir.glob(gcc_lib_base ).sort.last + "/include"
header_paths << gcc_lib
header_paths << "/usr/include"
header_paths << "/usr/include/clang-c"

parser = Rtruckboris::HeaderParser.new(clang_c, header_paths)

unless parser.parse(true)
  puts "Whada fock??"
  exit
end

functions = parser.functions

def is_object_generator(f, object_name)
  if f.name.match(/.*2$/)
    f.parameters.each do |p|
      if p.type.name.match(/#{object_name}\s+\*/)
        return true
      end
    end
  elsif f.return_type.name.match(/^#{object_name}$/)
    return true
  else 
    false
  end 
end
def is_object_method(f, object_name)
  f.parameters.each do |p|
    if p.type.name.match(/^#{object_name}$/)
      return true
    end
  end
end
def display_infos(object_name, generators, methods)
  puts "#{object_name} related functions"
  puts "Generators"
  generators.each do |g|
    puts g.name
  end

  puts "Methods"
  methods.each do |m|
    puts m.name
  end

end
def sumup(functions, objects)
  objects.each do |obj|
    obj_generators = []
    obj_methods = []
    functions.each do |f|
      next unless f.name.match(/#{obj.gsub(/^CX/,"")}/)
      if is_object_generator(f, obj) 
        obj_generators << f
      elsif is_object_method(f, obj)
          obj_methods << f 
      end
    end
    display_infos(obj, obj_generators, obj_methods)
  end
end
# todo CXCursor
# todo CXSourceLocation and clang_.*Location  
# todo CXRange and clang_.*range
# todo IndexAction
# todo File
sumup(functions, ["CXIndex", "CXDiagnostic(Set)*", "CXTranslationUnit"])
