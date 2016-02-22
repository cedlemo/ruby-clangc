PATH = File.expand_path(File.dirname(__FILE__)) 
module ClangcUtils
  # Good C test file
  SOURCE_FILE = "#{PATH}/source1.c"
   # Good C test file with gnu-binary-literal option warning
  SOURCE_FILE_OPTION_WARNINGS = "#{PATH}/source_option_warnings.c"
 # C source code with one error
  SOURCE_FILE_ONE_ERROR = "#{PATH}/source2.c"
  # C source code with two error
  SOURCE_FILE_TWO_ERRORS = "#{PATH}/source3.c"
  # C source code with three error
  SOURCE_FILE_THREE_ERRORS = "#{PATH}/source4.c"
  # C source file with pointer
  SOURCE_FILE_POINTER = "#{PATH}/source5.c"
  # C source file with qualified type
  SOURCE_FILE_QUALIFIED = "#{PATH}/source6.c"
  # C source file with only one function
  SOURCE_FILE_FUNCTION = "#{PATH}/source7.c"
  # C source file with only one array 
  SOURCE_FILE_ARRAY= "#{PATH}/source8.c"
  # C++ source file with one non POD and one POD
  SOURCE_FILE_POD = "#{PATH}/source9.cpp"
  # C source with macro
  SOURCE_FILE_MACRO =  "#{PATH}/source10.c"
  # C with enum
  SOURCE_FILE_ENUM = "#{PATH}/source11.c"
  # C with struct and bitfield 
  SOURCE_FILE_STRUCT_BITFIELD = "#{PATH}/source12.c"
  # C++ source file with function template example
  SOURCE_FILE_FUNCTION_TEMPLATE = "#{PATH}/source13.cpp"
  # Objective-C source file
  SOURCE_FILE_OBJECTC = "#{PATH}/source14.m"
  # C with anonymous structure
  SOURCE_FILE_ANON_DECLS = "#{PATH}/source15.cpp"
  # C++ source with classes and a virtual class
  SOURCE_FILE_VIRT_BASE_CLASS = "#{PATH}/source16.cpp"
  # C++ source with function overloading 
  SOURCE_FILE_OVERL_FUNC = "#{PATH}/source17.cpp"
  # Objective-C source file with IBOutlet collection
  SOURCE_FILE_IBOUTLET = "#{PATH}/source18.m"
  # Header with C function and its comments
  SOURCE_FILE_COMMENTS = "#{PATH}/source19.h"
  # second C++ source file with function template example
  SOURCE_FILE_FUNCTION_TEMPLATE_2 = "#{PATH}/source20.cpp"
  # C++ source file with method override example
  SOURCE_FILE_METHOD_OVERRIDE = "#{PATH}/source21.cpp"
  # Source file with module import
  SOURCE_FILE_MODULE_IMPORT = "#{PATH}/source22.cpp"
  # Source file with variadic function
  SOURCE_FILE_VARIADIC_FN = "#{PATH}/source23.cpp"
  # Source file with variadic type 
  SOURCE_FILE_VARIADIC_TYPE = "#{PATH}/source24.cpp"
  # Source file alignof sizeof 
  SOURCE_FILE_ALIGN_OF = "#{PATH}/source25.cpp"
  # Source file member pointer 
  SOURCE_FILE_MEMBER_PTR = "#{PATH}/source26.cpp"
  # Source file type offset 
  SOURCE_FILE_TYPE_OFFSET_OF = "#{PATH}/source27.cpp"
  # Source file class template specialisation 
  SOURCE_FILE_CLASS_TEMPLATE_SPECIALISATION = "#{PATH}/source28.cpp"
  # Source file cxx ref qualifier 
  SOURCE_FILE_CXX_REF_QUALIFIER = "#{PATH}/source29.cpp"
  # Source code for code completion
  SOURCE_FILE_COMPLETION_STRING = "#{PATH}/source30.c"
  SOURCE_FILE_WITH_INCLUDE_GUARD = "#{PATH}/include_guarded_header.h"
  # Inexistant file
  BAD_FILE = "#{PATH}/qsdfqsdf.c"
  AST_FILE = "#{PATH}/source1.ast"
  CLANG_HEADERS_PATH = Dir.glob("/usr/lib/clang/*/include").collect {|x| "-I#{x}"}
end
