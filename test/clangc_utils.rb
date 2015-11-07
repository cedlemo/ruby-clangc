module ClangcUtils
  # Good C test file
  SOURCE_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.c"
   # Good C test file with gnu-binary-literal option warning
  SOURCE_FILE_OPTION_WARNINGS = "#{File.expand_path(File.dirname(__FILE__))}/source_option_warnings.c"
 # C source code with one error
  SOURCE_FILE_ONE_ERROR = "#{File.expand_path(File.dirname(__FILE__))}/source2.c"
  # C source code with two error
  SOURCE_FILE_TWO_ERRORS = "#{File.expand_path(File.dirname(__FILE__))}/source3.c"
  # C source code with three error
  SOURCE_FILE_THREE_ERRORS = "#{File.expand_path(File.dirname(__FILE__))}/source4.c"
  # C source file with pointer
  SOURCE_FILE_POINTER = "#{File.expand_path(File.dirname(__FILE__))}/source5.c"
  # C source file with qualified type
  SOURCE_FILE_QUALIFIED = "#{File.expand_path(File.dirname(__FILE__))}/source6.c"
  # C source file with only one function
  SOURCE_FILE_FUNCTION = "#{File.expand_path(File.dirname(__FILE__))}/source7.c"
  # C source file with only one array 
  SOURCE_FILE_ARRAY= "#{File.expand_path(File.dirname(__FILE__))}/source8.c"
  # C++ source file with one non POD and one POD
  SOURCE_FILE_POD = "#{File.expand_path(File.dirname(__FILE__))}/source9.cpp"
  # C source with macro
  SOURCE_FILE_MACRO =  "#{File.expand_path(File.dirname(__FILE__))}/source10.c"
  # C with enum
  SOURCE_FILE_ENUM = "#{File.expand_path(File.dirname(__FILE__))}/source11.c"
  # C with struct and bitfield 
  SOURCE_FILE_STRUCT_BITFIELD = "#{File.expand_path(File.dirname(__FILE__))}/source12.c"
  # C++ source file with function template example
  SOURCE_FILE_FUNCTION_TEMPLATE = "#{File.expand_path(File.dirname(__FILE__))}/source13.cpp"
  # Objective-C source file
  SOURCE_FILE_OBJECTC = "#{File.expand_path(File.dirname(__FILE__))}/source14.m"
  # C with anonymous structure
  SOURCE_FILE_ANON_DECLS = "#{File.expand_path(File.dirname(__FILE__))}/source15.c"
  SOURCE_FILE_WITH_INCLUDE_GUARD = "#{File.expand_path(File.dirname(__FILE__))}/include_guarded_header.h"
  # Inexistant file
  BAD_FILE = "#{File.expand_path(File.dirname(__FILE__))}/qsdfqsdf.c"
  AST_FILE = "#{File.expand_path(File.dirname(__FILE__))}/source1.ast"
  CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
end
