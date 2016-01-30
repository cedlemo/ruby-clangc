require "rtruckboris"
require "fileutils"

module Toolbox
  class SourceParser
    def initialize(source_file, base_dir = nil)
      header_paths = []
      if base_dir && Dir.exist?(base_dir)
        base_dir = File.expand_path(base_dir)
      else
        base_dir = File.expand_path(File.dirname(source_file))
      end
      gcc_lib_base='/usr/lib/gcc/' << `llvm-config --host-target`.chomp << "/*"
      gcc_lib = Dir.glob(gcc_lib_base ).sort.last + "/include"
      header_paths << gcc_lib
      header_paths << "/usr/include"
      header_paths << base_dir

      @parser = Rtruckboris::HeaderParser.new(source_file, header_paths)
    end

    def parse(main_file_only)
      @parser.parse(main_file_only)
    end

    def functions
      @parser.functions
    end

    def structures
      @parser.structures
    end

    def unions
      @parser.unions
    end

    def enums
      @parser.enums
    end
  end
end
