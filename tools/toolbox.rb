require "fileutils"

unless Module.constants.include?(:PATH)
  PATH = File.expand_path(File.dirname(__FILE__))
end

begin
  require "clangc"
  module Toolbox
    class SourceParser
      attr_reader :index, :source_file, :base_dir,
                  :translation_unit, :diagnostics,
                  :functions
      def initialize(source_file, base_dir = nil, lang = "c")
        @source_file = source_file
        @base_dir = base_dir
        include_libs = build_default_include_libs
        args = ["-x", lang] + include_libs
        @index = Clangc::Index.new(false, false)
        @functions = []
        @translation_unit = @index.create_translation_unit(source: source_file,
                                                           args: args)
      end

      def parse(only_main_file)
        @diagnostics = @translation_unit.diagnostics if @translation_unit
        return false unless @translation_unit
        @only_main_file = only_main_file
        cursor = @translation_unit.cursor
        Clangc.visit_children(cursor: cursor) do |cxcursor, parent|
          if cursor_in_main_file?(cxcursor)
            case cxcursor.kind
            when Clangc::CursorKind::FUNCTION_DECL
              name = cxcursor.spelling
              ret = SourceType.new(cxcursor.result_type.spelling)
              fn = SourceFunction.new(name, ret)
              cxcursor.arguments.each do |param|
                p = SourceParameter.new(param.spelling, param.type.spelling)
                fn.add_param(p)
              end
              @functions << fn
            end
          end
          Clangc::ChildVisitResult::RECURSE
        end
        return true if @translation_unit
      end

      # Check if the cursor given in argument focus on
      # the file we want to parse and not on included
      # headers
      def cursor_in_main_file?(cursor)
        return true if !@only_main_file 
        # if we dont care that the cursor go in other
        # file we do like it is always in the main file
        cursor_file = cursor.location.spelling[0]
        main_file = @translation_unit.file(@source_file)
        cursor_file.is_equal(main_file)
      end

      private

      # Add the directories where the default headers files
      # for the standards libs can be found
      def build_default_include_libs
        header_paths = []
        if @base_dir && Dir.exist?(@base_dir)
          @base_dir = File.expand_path(@base_dir)
        else
          @base_dir = File.expand_path(File.dirname(@source_file))
        end
        gcc_lib_base = "/usr/lib/gcc/" << `llvm-config --host-target`.chomp << "/*"
        last_gcc_lib_base = Dir.glob(gcc_lib_base ).sort.last
        if last_gcc_lib_base
          gcc_lib = last_gcc_lib_base + "/include"
          header_paths << gcc_lib
        end
        header_paths << "/usr/include"
        header_paths << @base_dir
        header_paths.collect {|h| "-I#{h}"}
      end
    end

      class SourceFunction
      attr_reader :name, :parameters
      attr_accessor :return_type

      def initialize(name, return_type = nil, parameters = [])
        @name = name
        @parameters = parameters
        @return_type = return_type
      end

      def add_param(param)
        @parameters << param if param.class == SourceParameter
      end

      def parameters_num
        @parameters.size
      end
    end

    class SourceType
      attr_reader :name
      def initialize(name)
        @name = name
      end
    end

    class SourceParameter
      attr_reader :name, :type
      def initialize(name, type)
        @name = name
        if type.class == String
          @type = SourceType.new(type)
        elsif type.class == SourceType
          @type = type
        end
      end
    end
  end

rescue LoadError
  puts "No Clangc module, using rtuckboris"
  require "rtruckboris"
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
end
