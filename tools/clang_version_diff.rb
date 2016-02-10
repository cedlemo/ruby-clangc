require "./toolbox.rb"

PATH = File.expand_path(File.dirname(__FILE__))

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

functions = {}

clang_versions.each do |k,v|
  fns = v.functions
  fns.each do |f|
    if functions[f.name] && !functions[f.name].include?(k)
      functions[f.name] << k
    else
      functions[f.name] = [k]
    end
  end
end

functions.each do |k,v|
  puts "#{k} #{v.inspect}"
end

puts functions.size

