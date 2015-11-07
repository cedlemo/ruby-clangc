#!/usr/bin/env ruby
require "rtruckboris"
require "term/ansicolor"

def colorize(color, string)
  if color.is_a?(String)
    color = color.to_s
  elsif !color.is_a?(Symbol)
    raise "You should pass a string or a symbol"
    exit
  end
  "#{Term::ANSIColor.send(color.to_sym)}#{string}#{Term::ANSIColor.clear} "
end
def is_object_generator(f, object_name)
  if f.name.match(/.*2\z/)
    f.parameters.each do |p|
      if p.type.name.match(/#{object_name}/)
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
def display_infos(object_name, generators, methods, wrapped_methods = [])
  puts "#{colorize(:bold,object_name)} related functions"
  puts "\t#{colorize(:underscore,"Generators")}"
  generators.each do |g|
    gname = g.name
    if wrapped_methods.include?(gname)
      puts "\t\t#{colorize(:black,gname)}"
    else
      puts "\t\t#{gname}"
    end
  end
  # Sort on arguments number
  methods.sort! {|a,b| a.parameters_num <=> b.parameters_num}
  puts "\t#{colorize(:underscore,"Methods")} (order based on arguments number)"
  output_types = []
  input_types = []
  methods.each do |m|
    m.parameters.each do |p|
      input_types << p.type.name unless input_types.include?(p.type.name)
    end
    mname = m.name
    if wrapped_methods.include?(mname)
      puts "\t\t#{m.parameters_num} __ #{colorize(:black,mname)}"
    else
      puts "\t\t#{m.parameters_num} __ #{mname}"
    end
    output_types << m.return_type.name unless output_types.include?(m.return_type.name)
  end
#  puts "ouput_types:\n #{output_types.join(" ")}"
#  puts "input_types:\n #{input_types.join(" ")}"
end
def sumup_module_functions(functions, wrapped_methods = [])
  puts "#{colorize(:bold,"Module")} functions"
  functions.each do |f|
    if f.parameters.size == 0
      fname = f.name  
      if wrapped_methods.include?(fname)
        puts "\t\t#{colorize(:black,fname)}"
      else
        puts "\t\t#{fname}"
      end
    end
  end
  
end
def sumup_objects(functions, objects, wrapped_methods = [])
  objects.each do |obj|
    obj_generators = []
    obj_methods = []
    functions.each do |f|
      obj_generators << f if is_object_generator(f, obj)
      obj_methods << f if is_object_method(f, obj)

    end
    display_infos(obj, obj_generators, obj_methods, wrapped_methods)
  end
end

# Get clang functions in the C code written
CURRENT_PATH = File.expand_path(File.dirname(__FILE__))
SRC_FILES = Dir.glob("#{CURRENT_PATH}/../ext/clangc/*.[c|h]")
MANAGED_FUNCTIONS = []
SRC_FILES.each do |file|
  f = File.open(file, 'r')
  while !f.eof?
    line = f.readline
    if /(?<clang_function>clang_\w+)/ =~ line
      MANAGED_FUNCTIONS << clang_function
    end
  end
end

MANAGED_FUNCTIONS.uniq!

# Get a list of all Clang functions
clang_c = "/usr/include/clang-c/Index.h"
header_paths = []
gcc_lib_base='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
gcc_lib = Dir.glob(gcc_lib_base ).sort.last + "/include"
header_paths << gcc_lib
header_paths << "/usr/include"
header_paths << "/usr/include/clang-c"

parser = Rtruckboris::HeaderParser.new(clang_c, header_paths)

unless parser.parse(true)
  puts "Can't parse"
  exit
end

functions = parser.functions

count = 0
functions.each do |f|
    count += 1 if MANAGED_FUNCTIONS.include?(f.name)
end

# print global informations
color = Term::ANSIColor
print color.green, count.to_s, color.clear, "/", color.black, functions.size,
      color.clear, " functions wrapped => ", color.yellow, 
      (count/(functions.size*1.00)) * 100, color.clear, "%\n\n"

# print specific informations
sumup_module_functions(functions, MANAGED_FUNCTIONS)
sumup_objects(functions, ["CXIndex", "CXTranslationUnit", "CXDiagnostic", "CXFile","CXSourceRange", "CXSourceLocation", "CXCursor", "CXType"], MANAGED_FUNCTIONS)


print "\n", color.green, count.to_s, color.clear, "/", color.black, functions.size,
      color.clear, " functions wrapped => ", color.yellow, 
      (count/(functions.size*1.00)) * 100, color.clear, "%\n\n"

