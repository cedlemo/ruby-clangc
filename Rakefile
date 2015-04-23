require "rubygems"
require "rake/extensiontask"

spec = Gem::Specification.load('clangc.gemspec')

#Gem::PackageTask.new(spec) do |pkg|
#end
Rake::ExtensionTask.new "clangc", spec do |ext|
  ext.lib_dir = "lib/clangc"
end
