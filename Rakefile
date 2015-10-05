require "rubygems"
require "rake/extensiontask"
require "rdoc/task"
require "yard"

spec = Gem::Specification.load('clangc.gemspec')

RDOC_FILES = FileList["README.rdoc", "ext/clangc/clangc.c", "ext/clangc/constants.c", "ext/clangc/class_Index.c", "ext/clangc/class_TranslationUnit.c", "lib/clangc.rb"]

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc"
  rd.rdoc_files.include(RDOC_FILES)
end

YARD_FILES = FileList["ext/clangc/clangc.c", "ext/clangc/class_Index.c", "ext/clangc/class_TranslationUnit.c", "lib/clangc.rb"]

YARD::Rake::YardocTask.new do |t|
  t.files   = ['lib/**/*.rb', YARD_FILES]   # optional
  t.options = %w(-o yard_documentation --readme README.rdoc) # optional
end

Rake::ExtensionTask.new "clangc", spec do |ext|
  ext.lib_dir = "lib/clangc"
end
