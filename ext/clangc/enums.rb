#!/usr/bin/env ruby 
require "rtruckboris"


clang_c = "/usr/include/clang-c/Index.h"
headerPaths = []
#get gcc include if needed
gccLibBase='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
gccLib = Dir.glob(gccLibBase ).sort.last + "/include"
headerPaths << gccLib
headerPaths << "/usr/include"
headerPaths << "/usr/include/clang-c"

parser = Rtruckboris::HeaderParser.new(clang_c, headerPaths)
if(!parser.parse(true))
  puts "Whada fock??"
  exit
end
def create_module_constant(main_module, enum)
  name = enum.name == "" ? enum.typedef_name : enum.name
  enum_module_name = name.gsub(/CX/,"")
  c_value_enum_module_name = "m_#{enum_module_name}"
  s="VALUE #{c_value_enum_module_name} = rb_define_module_under(#{main_module},\"#{enum_module_name}\");\n"
  enum.enum_constants.each do |c|
    s+= "rb_define_const(#{c_value_enum_module_name},\"#{c.name.gsub(/CX[^\_]*\_/,"").capitalize}\", INT2NUM(#{c.value}));\n"
  end 
  s
end
def create_module_constants(main_module, enums)
  s="#include \"constants.h\"\n" 
  s+="void init_clang_enums_to_constants(VALUE #{main_module}){\n"
  enums.each_with_index do |e, i|
    s+="#{create_module_constant(main_module, e)}\n"
  end
  s+="}\n"
end
enums = parser.enums
puts enums.size
class OutputFiles
  attr_reader :_c, :_h
  def initialize(name)
    @_c = File.new((name||'wrapper') + '.c', 'w')
    @_h = File.new((name||'wrapper') + '.h', 'w')
    @_c.sync= true
    @_c.sync= true
  end
  def close_all
    @_c.close
    @_h.close
  end
end 
constants = OutputFiles.new("constants")
constants._h.puts <<EOF
#include <ruby/ruby.h>
void init_clang_enums_to_constants(VALUE);
EOF
constants._c.puts create_module_constants("m_clang", enums)
constants.close_all

NEWLINE ="\n"
O_BRACKET ="("
C_BRACKET = ")"
O_CURLY_BRACKET = "{"
C_CURLY_BRACKET = "}"
COMMA = ","
SEMI_COLON = ";"


