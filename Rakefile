require "rubygems"
require "rake/extensiontask"
require 'rdoc/task'

spec = Gem::Specification.load('clangc.gemspec')

#Gem::PackageTask.new(spec) do |pkg|
#end
RDOC_FILES = FileList["README.rdoc", "ext/clangc/clangc.c", "ext/clangc/constants.c", "ext/clangc/class_Index.c", "ext/clangc/class_TranslationUnit.c", "lib/clangc.rb"]

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc"
  rd.rdoc_files.include(RDOC_FILES)
end
Rake::ExtensionTask.new "clangc", spec do |ext|
  ext.lib_dir = "lib/clangc"
end
