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

def rename_enum_constant(enum_constant)
  new_name = enum_constant.gsub(/CX[^\_]*\_/,"")
  new_name = to_snake_case(new_name)
  new_name.upcase
end

constants_names = []

def get_constants_names(enum_constant)
  {:old => enum_constant.gsub(/CX[^\_]*\_/,"").capitalize, :new => rename_enum_constant(enum_constant)}
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
enums.each do |e|
  e.enum_constants.each do |c|
    constants_names << get_constants_names(c.name)
  end
end

clang_c = "/usr/include/clang-c/CXErrorCode.h"

parser = Rtruckboris::HeaderParser.new(clang_c, headerPaths)
if(!parser.parse(true))
  puts "Whada fock??"
  exit
end

enums = parser.enums

enums.each do |e|
  e.enum_constants.each do |c|
    constants_names << get_constants_names(c.name)
  end
end

files = Dir.glob("#{PATH}/../test/*.rb")

files.each do |f|
  constants_names.each do |c|
    `/usr/bin/sed -i 's/\:#{c[:old]}$/\:#{c[:new]}/g' #{f}`
    `/usr/bin/sed -i 's/\:#{c[:old]}\ /\:#{c[:new]}\ /g' #{f}`
    `/usr/bin/sed -i 's/\:#{c[:old]}\|/\:#{c[:new]}\|/g' #{f}`
    `/usr/bin/sed -i 's/\:#{c[:old]}\)/\:#{c[:new]}\)/g' #{f}`
    `/usr/bin/sed -i 's/\:#{c[:old]}\,/\:#{c[:new]}\,/g' #{f}`
    `/usr/bin/sed -i 's/Clangc::TYPE/Clangc::Type/g' #{f}`
  end
end
