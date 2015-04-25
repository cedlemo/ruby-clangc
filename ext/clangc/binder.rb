module Binder
  NEWLINE ="\n"
  O_BRACKET ="("
  C_BRACKET = ")"
  O_CURLY_BRACKET = "{"
  C_CURLY_BRACKET = "}"
  COMMA = ","
  SEMI_COLON = ";"
  
  class OutputFiles
    attr_reader :_c, :_h
    def initialize(name)
      @_c = File.new((name||'wrapper') + '.c', 'w')
      @_h = File.new((name||'wrapper') + '.h', 'w')
      @_c.sync= true
      @_c.sync= true
    end
    def close_all
      @_c.close
      @_h.close
    end
  end 

  class Ruby_C_Class_Generator
    attr_accessor :free_instructions, :args, :init_instructions 
    def initialize(class_name, data_type)
      @class_name = class_name
      @data_type = data_type
      @files = OutputFiles.new(class_name)
    end
    def generate_files
      generate_c_file
      generate_h_file
      @files.close_all
    end

    :private
    def generate_class_structure
%Q{#ifndef #{@class_name.upcase}_H
#define #{@class_name.upcase}_H
  #include <ruby/ruby.h>
  typedef #{class_name}_t {
    #{@data_type} data; 
} #{class_name}_t;
#endif //#{@class_name.upcase}_H
}
    end
    def generate_free_callback
%Q{/*#{@class_name} ruby class*/
#include "#{@class_name}.h"
static void c_#{@class_name}_struct_free(#{@class_name}_t *c)
{
  if(c)
  {
    #{@free_instructions||""}
    ruby_xfree(c);
  }
}  
}
    end
    def generate_basic_allocator
%Q{static VALUE c_#{class_name}_struct_alloc( VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, c_#{class_name}_struct_free, ruby_xmalloc(sizeof(#{class_name}_t)));
}
}
    end
    def generate_initialize
%Q{
static VALUE c_#{@class_name}_initialize(#{@args||"VALUE self"}) {
  #{init_instructions}
}
}
    end
    def generate_c_file
      @files._c.puts generate_free_callback
      @files._c.puts generate_basic_allocator
      @files._c.puts generate_initialize
    end
    def generate_h_file
      @files._h.puts generate_class_structure
    end
  end

  def self.rb_str_2_c_char_ptr(r_val_name, c_val_type, c_val_name)
        %Q{  if(TYPE(#{r_val_name}) != T_STRING)
    rb_raise(rb_eTypeError, "invalid type for input");
  #{c_val_type} #{c_val_name}=RSTRING_PTR(#{r_val_name});
}
  end
  def self.rb_boolean_2_c_boolean(r_val_name, c_val_type, c_val_name)
        %Q{  #{c_val_type} #{c_val_name}= (#{r_val_name} == Qtrue) ? TRUE : FALSE;
}
  end
  def self.rb_num_2_int8(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2int8(FIX2INT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2int8(NUM2INT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_uint8(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2uint8FIX2UINT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2uint8(NUM2UINT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_int16(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2int16(FIX2INT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2int16(NUM2INT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_uint16(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2uint16FIX2UINT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2uint16(NUM2UINT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_int32(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2int32(FIX2INT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2int32(NUM2INT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_uint32(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=int2uint32FIX2UINT(#{r_val_name}));
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=int2uint32(NUM2UINT(#{r_val_name}));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_int(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=FIX2INT(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2INT(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_uint(r_val_name, c_val_type, c_val_name)
    %Q{    #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=FIX2UINT(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2UINT(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_long(r_val_name, c_val_type, c_val_name)
    %Q{  #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=FIX2LONG(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2LONG(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_ulong(r_val_name, c_val_type, c_val_name)
    %Q{   #{c_val_type} #{c_val_name};
   if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=FIX2ULONG(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2ULONG(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_short(r_val_name, c_val_type, c_val_name)
    %Q{    #{c_val_type} #{c_val_name};
    if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=FIX2SHORT(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2SHORT(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_ushort(r_val_name, c_val_type, c_val_name)
    %Q{    #{c_val_type} #{c_val_name};
  if (TYPE(#{r_val_name}) == T_FIXNUM)
    #{c_val_name}=rb_fix2ushort(#{r_val_name});
  else if(TYPE(TYPE(#{r_val_name}) == T_BIGNUM))
    #{c_val_name}=NUM2USHORT(#{r_val_name});
  else
    rb_raise(rb_eTypeError, "invalid type for input");
}
  end
  def self.rb_num_2_llong(r_val_name, c_val_type, c_val_name)
    %Q{  if (TYPE(#{r_val_name}) != T_FIXNUM) && (TYPE(#{r_val_name}) != T_BIGNUM)
  rb_raise(rb_eTypeError, "invalid type for input");
    #{c_val_type} #{c_val_name}=NUM2LL(#{r_val_name});
}
  end
  def self.rb_num_2_ullong(r_val_name, c_val_type, c_val_name)
    %Q{  if (TYPE(#{r_val_name}) != T_FIXNUM) && (TYPE(#{r_val_name}) != T_BIGNUM)
    rb_raise(rb_eTypeError, "invalid type for input");
  #{c_val_type} #{c_val_name}=NUM2ULL(#{r_val_name});
}
  end
  def self.rb_num_2_dbl(r_val_name, c_val_type, c_val_name)
    %Q{  if ((TYPE(#{r_val_name}) != T_FIXNUM) && (TYPE(#{r_val_name}) != T_BIGNUM))
    rb_raise(rb_eTypeError, "invalid type for input");
  #{c_val_type} #{c_val_name}=NUM2DBL(#{r_val_name});
}
  end
  def self.rb_num_2_sizet(r_val_name, c_val_type, c_val_name)
    %Q{  if (TYPE(#{r_val_name}) != T_FIXNUM) && (TYPE(#{r_val_name}) != T_BIGNUM)
    rb_raise(rb_eTypeError, "invalid type for input");
  #{c_val_type} #{c_val_name}=NUMSIZET(#{r_val_name});
}
  end
  def self.rb_num_2_ssizet(r_val_name, c_val_type, c_val_name)
    %Q{  if (TYPE(#{r_val_name}) != T_FIXNUM) && (TYPE(#{r_val_name}) != T_BIGNUM)
    rb_raise(rb_eTypeError, "invalid type for input");
  #{c_val_type} #{c_val_name}=NUMSSIZET(#{r_val_name});
}
  end
  def self.c_char_ptr_2_rb_str(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = rb_str_new2(#{c_val_name});
}
  end
  def self.c_boolean_2_rb_boolean(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = #{c_val_name} == TRUE ? Qtrue : Qfalse;
}
  end
  def self.c_int_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = INT2NUM((int) #{c_val_name});
}
  end  
  def self.c_uint_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = UINT2NUM((int) #{c_val_name});
}
  end
  def self.c_long_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = LONG2NUM(#{c_val_name});
}
  end
  def self.c_ulong_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = ULONG2NUM(#{c_val_name});
}
  end
  def self.c_llong_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = LL2NUM(#{c_val_name});
}
  end
  def self.c_ullong_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = ULL2NUM(#{c_val_name});
}
  end
  def self.c_dbl_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = DBL2NUM(#{c_val_name});
}
  end
  def self.c_sizet_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = SIZET2NUM(#{c_val_name});
}
  end
  def self.c_ssizet_2_rb_num(r_val_name, c_val_name)
    %Q{  VALUE #{r_val_name} = SSIZET2NUM(#{c_val_name});
}
  end
# TODO see if needed
#  def self.c_pointer_arg_to_rb_value(r_val_name, c_val_type, c_val_name, type)
#    local_type = c_val_type.gsub(/\s*\*\s*/,"")
#    local_var = "local_" + c_val_name
#    #s = Wrapper::send("c_#{type}_2_rb_num".to_sym, r_val_name,local_var)
#    s = "  #{local_type} #{local_var};" + NEWLINE
#    s += "  #{c_val_type} #{c_val_name} = &#{local_var};" + NEWLINE
#  end

end
