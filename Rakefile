require "rubygems"
require "rake/extensiontask"
require "rdoc/task"
require "yard"

PATH = File.expand_path(File.dirname(__FILE__))

spec = Gem::Specification.load('clangc.gemspec')

RDOC_FILES = FileList["README.rdoc", "ext/clangc/clangc.c", "ext/clangc/constants.c", "ext/clangc/class_Index.c", "ext/clangc/class_TranslationUnit.c", "lib/clangc.rb"]

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc"
  rd.rdoc_files.include(RDOC_FILES)
end

#YARD_FILES = FileList["ext/clangc/clangc.c", "ext/clangc/_clangc_functions.c", "ext/clangc/class_Cursor.c", "ext/clangc/class_Diagnostic.c", "ext/clangc/class_File.c", "ext/clangc/class_Index.c", "ext/clangc/class_SourceLocation.c", "ext/clangc/class_SourceRange.c", "ext/clangc/class_TranslationUnit.c", "ext/clangc/class_Type.c", "lib/clangc.rb"]
#YARD_FILES = FileList["ext/clangc/clangc.c", "ext/clangc/class_Index.c", "ext/clangc/class_TranslationUnit.c", "lib/clangc.rb"]
#files_names = %w(clangc.c  _clangc_functions.c  class_Cursor.c  class_Diagnostic.c  class_File.c  class_Index.c  class_SourceLocation.c  class_SourceRange.c  class_TranslationUnit.c  class_Type.c)

#files_names.collect!{ |n| "ext/clangc/#{n}" }
YARD_FILES = FileList.new("#{PATH}/ext/clangc/*.c") do |fl|
   fl.exclude(/constant/)
end
change_dir = proc do 
  Dir.chdir("ext/clangc")
end

reset_change_dir = proc do
  Dir.chdir(PATH)
end

YARD::Rake::YardocTask.new do |t|
  t.before  = change_dir
  t.files   = YARD_FILES   # optional
  t.options = ["-o", "#{PATH}/yard_documentation", "-r" ,"#{PATH}/README.rdoc"] # optional
  t.after   = reset_change_dir
end
Rake::ExtensionTask.new "clangc", spec do |ext|
  ext.lib_dir = "lib/clangc"
end
