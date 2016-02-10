#!/usr/bin/env ruby
require "rtruckboris"
require "term/ansicolor"
PATH = File.expand_path(File.dirname(__FILE__))
require "#{PATH}/toolbox.rb"

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
def display_infos(object_name, generators, methods, wrapped_methods = [], versions)
  puts "#{colorize(:bold,object_name)} related functions"
  puts "\t#{colorize(:underscore,"Generators")}"
  generators.each do |g|
    gname = g.name
    if wrapped_methods.include?(gname)
      puts "\t\t#{colorize(:black,gname)} - #{versions[gname].inspect}"
    else
      puts "\t\t#{gname} - #{versions[gname].inspect}"
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
      puts "\t\t#{m.parameters_num} __ #{colorize(:black,mname)} - #{versions[mname].inspect}"
    else
      puts "\t\t#{m.parameters_num} __ #{mname} - #{versions[mname].inspect}"
    end
    output_types << m.return_type.name unless output_types.include?(m.return_type.name)
  end
end
def sumup_module_functions(functions, wrapped_methods = [], versions)
  puts "#{colorize(:bold,"Module")} functions"
  functions.each do |f|
    if f.parameters.size == 0
      fname = f.name  
      if wrapped_methods.include?(fname)
        puts "\t\t#{colorize(:black,fname)} - #{versions[fname].inspect}"
      else
        puts "\t\t#{fname} - #{versions[fname].inspect}"
      end
    end
  end
  
end
def sumup_objects(functions, objects, wrapped_methods = [], versions)
  objects.each do |obj|
    obj_generators = []
    obj_methods = []
    functions.each do |f|
      obj_generators << f if is_object_generator(f, obj)
      obj_methods << f if is_object_method(f, obj)

    end
    display_infos(obj, obj_generators, obj_methods, wrapped_methods, versions)
  end
end

def print_global_informations(functions, managed_functions)
  count = 0
  functions.each do |f|
    count += 1 if managed_functions.include?(f.name)
  end

  color = Term::ANSIColor
  print color.green, count.to_s, color.clear, "/", color.black, 
        functions.size, color.clear, " functions wrapped => ", 
        color.yellow, (count/(functions.size*1.00)) * 100, 
        color.clear, "%\n\n"
end

# Get clang functions that are wrapped in the C bindings
SRC_FILES = Dir.glob("#{PATH}/../ext/clangc/*.[c|h]")
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

# Get Clang versions with function name:
  clang_versions = {}
  clang_versions[:v34] = Toolbox::SourceParser.new("#{PATH}/clang-3.4/Index.h")
  clang_versions[:v35] = Toolbox::SourceParser.new("#{PATH}/clang-3.5/Index.h")
  clang_versions[:v36] = Toolbox::SourceParser.new("#{PATH}/clang-3.6/Index.h")
  clang_versions[:v37] = Toolbox::SourceParser.new("#{PATH}/clang-3.7/Index.h")

  OUT = STDOUT.dup
  ERR = STDERR.dup
  clang_versions.each do |k,v|
    $stdout.reopen("parse_#{k}.log", "w")
    $stderr.reopen("parse_#{k}.err", "w")
    v.parse(true)
    $stdout.flush
    $stderr.flush
  end
  $stdout = OUT
  $stderr = ERR

  versions = {}

  clang_versions.each do |k,v|
    fns = v.functions
    fns.each do |f|
      if versions[f.name] && !versions[f.name].include?(k)
        versions[f.name] << k
      else
        versions[f.name] = [k]
      end
    end
  end

# Get a list of all Clang functions of system headers
clang_c = "/usr/include/clang-c/Index.h"

parser = Toolbox::SourceParser.new(clang_c)
unless parser.parse(true)
  puts "Can't parse"
  exit
end

functions = parser.functions


print_global_informations(functions, MANAGED_FUNCTIONS)
# print specific informations
sumup_module_functions(functions, MANAGED_FUNCTIONS, versions)
sumup_objects(functions, ["CXIndex",
                          "CXTranslationUnit",
                          "CXDiagnostic",
                          "CXFile",
                          "CXSourceRange",
                          "CXSourceLocation",
                          "CXCursor",
                          "CXType",
                          "CXCompletionString",
                          "CXCursorSet",
                          "CXModule"],
                          MANAGED_FUNCTIONS, versions)

print_global_informations(functions, MANAGED_FUNCTIONS)
