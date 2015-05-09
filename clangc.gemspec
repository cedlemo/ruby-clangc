Gem::Specification.new do |s|
  s.name        = 'clangc'
  s.version     = '0.0.1'
  s.date        = '2015-04-22'
  s.summary     = "Ruby bindings for clang C API"
  s.description = "Ruby bindings for clang C API"
  s.authors     = ["cedlemo"]
  s.email       = 'cedlemo@gmx.com'
  require "rake"
  s.files       = FileList['lib/*.rb', 'ext/*/*.{rb,c,h}']
  s.extensions = %w(ext/clangc/extconf.rb)
  s.add_development_dependency 'rake-compiler', '~> 0' 
  s.homepage    =     'https://github.com/cedlemo/ruby-clangc'
  s.license       = 'GNU GPL 3'
end
