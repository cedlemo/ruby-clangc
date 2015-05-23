#!/usr/bin/env ruby

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

require "rtruckboris"

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
require "term/ansicolor"

color = Term::ANSIColor
count = 0
functions.each do |f|
  if MANAGED_FUNCTIONS.include?(f.name)
    print color.green, f.name, color.clear, "\n"
    count += 1
  else
    print color.black, f.name, color.clear, "\n"
  end 
end

print color.green, count.to_s, color.clear, "/", color.black, functions.size, color.clear, " functions wrapped\n"

print color.yellow, (count/(functions.size*1.00)) * 100, color.clear, "%\n"
