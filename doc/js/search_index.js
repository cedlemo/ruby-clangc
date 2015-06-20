var search_data = {"index":{"searchIndex":["clangc","diagnostic","file","index","sourcerange","translationunit","category()","category_name()","category_text()","create_translation_unit()","create_translation_unit2()","create_translation_unit_from_source_file()","default_code_complete_options()","default_diagnostic_display_options()","default_editing_translation_unit_options()","default_reparse_options()","default_save_options()","diagnostic()","diagnostics_num()","file()","format()","global_options()","global_options=()","is_equal()","is_multiple_include_guarded()","mtime()","name()","new()","null_source_range()","num_fixits()","num_ranges()","option()","parse_translation_unit()","parse_translation_unit2()","severity()","source_range()","spelling()","spelling()","version()","readme"],"longSearchIndex":["clangc","clangc::diagnostic","clangc::file","clangc::index","clangc::sourcerange","clangc::translationunit","clangc::diagnostic#category()","clangc::diagnostic#category_name()","clangc::diagnostic#category_text()","clangc::index#create_translation_unit()","clangc::index#create_translation_unit2()","clangc::index#create_translation_unit_from_source_file()","clangc::default_code_complete_options()","clangc::default_diagnostic_display_options()","clangc::default_editing_translation_unit_options()","clangc::translationunit#default_reparse_options()","clangc::translationunit#default_save_options()","clangc::translationunit#diagnostic()","clangc::translationunit#diagnostics_num()","clangc::translationunit#file()","clangc::diagnostic#format()","clangc::index#global_options()","clangc::index#global_options=()","clangc::file#is_equal()","clangc::file#is_multiple_include_guarded()","clangc::file#mtime()","clangc::file#name()","clangc::index::new()","clangc::null_source_range()","clangc::diagnostic#num_fixits()","clangc::diagnostic#num_ranges()","clangc::diagnostic#option()","clangc::index#parse_translation_unit()","clangc::index#parse_translation_unit2()","clangc::diagnostic#severity()","clangc::diagnostic#source_range()","clangc::diagnostic#spelling()","clangc::translationunit#spelling()","clangc::version()",""],"info":[["Clangc","","Clangc.html","",""],["Clangc::Diagnostic","","Clangc/Diagnostic.html","","<p>A diagnostic object, containing the diagnostic&#39;s severity, location,\ntext, source ranges, and fix-it …\n"],["Clangc::File","","Clangc/File.html","","<p>A particular source file that is part of a translation unit\n"],["Clangc::Index","","Clangc/Index.html","","<p>An “index” consists of a set of translation units that would typically be\nlinked together into …\n"],["Clangc::SourceRange","","Clangc/SourceRange.html","","<p>Identifies a half-open character range in the source code.\n<p>Use Clangc::Range#start and Clangc::Range#end …\n"],["Clangc::TranslationUnit","","Clangc/TranslationUnit.html","","<p>A single TranslationUnit which resides in an Index\n"],["category","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-category","()","<p>Retrieve the category number for this diagnostic.\n<p>Diagnostics can be categorized into groups along with …\n"],["category_name","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-category_name","()","<p>Retrieve the name of a particular diagnostic category.  This is now\ndeprecated.  Use Clangc::Diagnostic#category_text …\n"],["category_text","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-category_text","()","<p>Retrieve the diagnostic category text for a given diagnostic. Returns The\ntext of the given diagnostic …\n"],["create_translation_unit","Clangc::Index","Clangc/Index.html#method-i-create_translation_unit","(p1)","<p>Create a translation unit from an AST file name. If the creation fail, it\nreturns nil. The AST file is …\n"],["create_translation_unit2","Clangc::Index","Clangc/Index.html#method-i-create_translation_unit2","(p1)","<p>Create a translation unit from an AST file name. If the creation fail, it\nreturns an error code Clangc::ErrorCode …\n"],["create_translation_unit_from_source_file","Clangc::Index","Clangc/Index.html#method-i-create_translation_unit_from_source_file","(p1, p2)","<p>Return a TranslationUnit instance for a given source file and the provided\ncommand line arguments one …\n"],["default_code_complete_options","Clangc","Clangc.html#method-c-default_code_complete_options","()","<p>Returns a default set of code-completion options that can be passed to\nClangc::TranslationUnit#codeCompleteAt …\n"],["default_diagnostic_display_options","Clangc","Clangc.html#method-c-default_diagnostic_display_options","()","<p>Retrieve the set of display options most similar to the default behavior of\nthe clang compiler.\n<p>A set …\n"],["default_editing_translation_unit_options","Clangc","Clangc.html#method-c-default_editing_translation_unit_options","()","<p>Returns the set of flags that is suitable for parsing a translation unit\nthat is being edited. (Clangc::TranslationUnit_Flags …\n"],["default_reparse_options","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-default_reparse_options","()","<p>Returns the set of flags that is suitable for reparsing a translation unit.\n<p>The set of flags returned …\n"],["default_save_options","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-default_save_options","()","<p>Returns the set of flags that is suitable for saving a translation unit.\nThose flags should be Clangc::SaveTranslationUnit_Flags …\n"],["diagnostic","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-diagnostic","(p1)","<p>Retrieve a diagnostic associated with the given translation unit.\n<p>index the zero-based diagnostic number …\n"],["diagnostics_num","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-diagnostics_num","()","<p>Determine the number of diagnostics produced for the given translation\nunit.\n"],["file","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-file","(p1)","<p>Retrieve a file handle within the given translation unit. file_name a\nString for the name of the file. …\n"],["format","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-format","(p1)","<p>Format the given diagnostic in a manner that is suitable for display.\n<p>This routine will format the given …\n"],["global_options","Clangc::Index","Clangc/Index.html#method-i-global_options","()","<p>Gets the general options associated with an Index. A bitmask of options, a\nbitwise OR of the Clangc::GlobalOptFlags …\n"],["global_options=","Clangc::Index","Clangc/Index.html#method-i-global_options-3D","(p1)","<p>Sets general options associated with an Index instance.\n<p>options:\n\n<p>A bitmask of options, a bitwise OR of …\n"],["is_equal","Clangc::File","Clangc/File.html#method-i-is_equal","(p1)","<p>Returns true if the file1 and file2 point to the same file, or both null.\nTwo ruby objects Clangc::File …\n"],["is_multiple_include_guarded","Clangc::File","Clangc/File.html#method-i-is_multiple_include_guarded","()","<p>Determine whether the given header is guarded against multiple inclusions,\neither with the conventional …\n"],["mtime","Clangc::File","Clangc/File.html#method-i-mtime","()","<p>Retrieve the last modification time of the given file.\n"],["name","Clangc::File","Clangc/File.html#method-i-name","()","<p>Retrieve the complete file and path name of the given file.\n"],["new","Clangc::Index","Clangc/Index.html#method-c-new","(p1, p2)","<p>Provides a shared context for creating translation units.\n<p>It provides two options:\n<p>excludeDeclarationsFromPCH: …\n"],["null_source_range","Clangc","Clangc.html#method-c-null_source_range","()","<p>Retrieve a NULL (invalid) source range\n"],["num_fixits","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-num_fixits","()","<p>Determine the number of fix-it hints associated with the given diagnostic.\n"],["num_ranges","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-num_ranges","()","<p>Determine the number of source ranges associated with the given diagnostic.\n"],["option","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-option","()","<p>Retrieve the name of the command-line option that enabled this diagnostic\nsuch as (“-Wconversion” …\n"],["parse_translation_unit","Clangc::Index","Clangc/Index.html#method-i-parse_translation_unit","(p1, p2, p3)","<p>Parse the given source file and generate the translation unit corresponding\nto that file.\n<p>This routine …\n"],["parse_translation_unit2","Clangc::Index","Clangc/Index.html#method-i-parse_translation_unit2","(p1, p2, p3)","<p>Parse the given source file and generate the translation unit corresponding\nto that file. If its fails, …\n"],["severity","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-severity","()","<p>Determine the severity of the given diagnostic. It returns one of the\nconstants defined in Clangc::DiagnosticSeverity.constants …\n"],["source_range","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-source_range","(p1)","<p>Retrieve a source range associated with the diagnostic.\n<p>A diagnostic&#39;s source ranges highlight important …\n"],["spelling","Clangc::Diagnostic","Clangc/Diagnostic.html#method-i-spelling","()","<p>Retrieve the text of the given diagnostic.\n"],["spelling","Clangc::TranslationUnit","Clangc/TranslationUnit.html#method-i-spelling","()","<p>Get the original translation unit source file name.\n"],["version","Clangc","Clangc.html#method-c-version","()","<p>Return a version string, suitable for showing to a user, but not intended\nto be parsed (the format is …\n"],["README","","README_rdoc.html","","<p>Ruby bindings for libclang \n<p>This module provides bindings for the C interface of Clang. clang.llvm.org\n…\n"]]}}