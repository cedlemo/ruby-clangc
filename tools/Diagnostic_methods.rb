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
puts functions.size
def is_object_generator(f, object_name)
  if f.name.match(/.*2$/)
    f.parameters.each do |p|
      if p.type.name.match(/#{object_name}\s+\*/)
        return true
      end
    end
    false
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
  false
end
def display_infos(object_name, generators, methods)
  puts "#{object_name} related functions"
#  puts "Generators"
#  generators.each do |g|
#    puts g
#  end
  # Sort on arguments number
  methods.sort! {|a,b| a.parameters_num <=> b.parameters_num}
  puts "Methods"
  output_types = []
  input_types = []
  methods.each do |m|
    m.parameters.each do |p|
      input_types << p.type.name unless input_types.include?(p.type.name)
    end
    puts "#{m.parameters_num} __ #{m}"
    output_types << m.return_type.name unless output_types.include?(m.return_type.name)
  end
  puts "ouput_types:\n #{output_types.join(" ")}"
  puts "input_types:\n #{input_types.join(" ")}"
end
def sumup(functions, objects)
  objects.each do |obj|
    obj_generators = []
    obj_methods = []
    functions.each do |f|
#      next unless f.name.match(/#{obj.gsub(/^CX/,"")}/)
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
sumup(functions, ["CXDiagnostic"])
