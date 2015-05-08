require "binder.rb"

index_class = Binder::Ruby_Class_Generator.new("Index", "CXIndex *")
index_class.free_instructions = %q{
  if(c->data)
    clang_disposeIndex(c->data);
}
index_class.args = "VALUE self, VALUE exclude_pch, VALUE display_diagnostic"
index_class.init_instructions = Binder::rb_num_2int("exclude_pch", 
                                                    "int",
                                                     "c_exclude_pch")
index_class.init_instructions += Binder::rb_num_2int("display_diagnostic",
                                                     "int", 
                                                     "c_display_diagnostic")
# TODO finish this
index_class.init_instructions += %Q{
clang_createIndex(c_exclude_pch, c_display_diagnostic);
}
 

