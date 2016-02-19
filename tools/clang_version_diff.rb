require "./toolbox.rb"

clang_versions = {}
clang_versions[:v34] = Toolbox::SourceParser.new("#{PATH}/clang-3.4/Index.h")
clang_versions[:v35] = Toolbox::SourceParser.new("#{PATH}/clang-3.5/Index.h")
clang_versions[:v36] = Toolbox::SourceParser.new("#{PATH}/clang-3.6/Index.h")
clang_versions[:v37] = Toolbox::SourceParser.new("#{PATH}/clang-3.7/Index.h")

OUT = STDOUT.dup
ERR = STDERR.dup
clang_versions.each do |k, v|
  $stdout.reopen("parse_#{k}.log", "w")
  $stderr.reopen("parse_#{k}.err", "w")
  v.parse(true)
  $stdout.flush
  $stderr.flush
end

$stdout = OUT
$stderr = ERR

functions = {}

clang_versions.each do |k, v|
  fns = v.functions
  fns.each do |f|
    if functions[f.name] && !functions[f.name].include?(k)
      functions[f.name] << k
    else
      functions[f.name] = [k]
    end
  end
end

clang_versions.each do |k, v|
  puts "versions #{k} fns number : #{v.functions.size}"
end

def filter_functions_to_print(current_version, versions)
  versions.sort
  if versions.include?(current_version)
    if current_version == :v34
      return true if versions.size == 1
    else
      return true if versions.first == current_version
    end
  else
    false
  end
end

clang_versions.each do |k, _v|
  puts "functions that are only in #{k}"
  functions.each do |name, versions|
    puts "\t #{name}" if filter_functions_to_print(k, versions)
  end
end
