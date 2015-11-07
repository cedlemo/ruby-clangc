#!/usr/bin/env ruby 
require "rtruckboris"

require "active_support/inflector"
PATH = File.expand_path(File.dirname(__FILE__))

require "#{PATH}/binder"

def to_snake_case(camel_cased_word)
  return camel_cased_word unless camel_cased_word =~ /[A-Z-]|::/
  word = camel_cased_word.to_s.gsub(/::/, '/')
  word.gsub!(/(?:(?<=([A-Za-z\d]))|\b)(#{ActiveSupport::Inflector.inflections.acronym_regex})(?=\b|[^a-z])/) { "#{$1 && '_'}#{$2.downcase}" }
  word.gsub!(/([A-Z\d]+)([A-Z][a-z])/,'\1_\2')
  word.gsub!(/([a-z\d])([A-Z])/,'\1_\2')
  word.tr!("-", "_")
  word
end

def rename_enum_constant(enum_name, enum_constant)
  new_name = enum_constant.gsub(/CX[^\_]*\_/,"")
  new_name = to_snake_case(new_name)
  new_name.upcase
end

def create_module_constant(main_module, enum)
  name = enum.name == "" ? enum.typedef_name : enum.name
  enum_module_name = name.gsub(/^CX\_?/,"")
  c_value_enum_module_name = "m_#{enum_module_name}"
  s="VALUE #{c_value_enum_module_name} = rb_define_module_under(#{main_module},\"#{enum_module_name}\");\n"
  enum.enum_constants.each do |c|
    s+= "rb_define_const(#{c_value_enum_module_name},\"#{rename_enum_constant(name, c.name)}\", INT2NUM(#{c.value}));\n"
  end 
  s
end

def create_module_constants(main_module, enums, id = nil )
  id = id ? "_" + id : ""
  s="void init_clang#{id}_enums_to_constants(VALUE #{main_module}){\n"
  enums.each_with_index do |e, i|
    s+="#{create_module_constant(main_module, e)}\n"
  end
  s+="}\n"
end

headerPaths = []
#get gcc include if needed
gccLibBase='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
gccLib = Dir.glob(gccLibBase ).sort.last + "/include"
headerPaths << gccLib
headerPaths << "/usr/include"
headerPaths << "/usr/include/clang-c"

clang_c = "/usr/include/clang-c/Index.h"

parser = Rtruckboris::HeaderParser.new(clang_c, headerPaths)
if(!parser.parse(true))
  puts "Whada fock??"
  exit
end

enums = parser.enums

constants = Binder::OutputFiles.new("constants")
constants._h.puts <<EOF
#include <ruby/ruby.h>
void
init_clang_enums_to_constants(VALUE);
EOF

constants._c.puts "#include \"constants.h\"\n" 
constants._c.puts create_module_constants("m_clang", enums)

clang_c = "/usr/include/clang-c/CXErrorCode.h"

parser = Rtruckboris::HeaderParser.new(clang_c, headerPaths)
if(!parser.parse(true))
  puts "Whada fock??"
  exit
end

enums = parser.enums

constants._h.puts <<EOF
void
init_clang_errors_enums_to_constants(VALUE);
EOF
constants._c.puts create_module_constants("m_clang", enums, "errors")
constants.close_all
