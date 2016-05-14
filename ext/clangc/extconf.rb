require "mkmf"

$CFLAGS += " -I#{`llvm-config --prefix`.chomp}/include"
#$CFLAGS += " -std=c89 -pedantic -Wall"
#$CFLAGS += " -Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2"
#$CFLAGS += " -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith"
#$CFLAGS += " -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline"
#$CFLAGS += " -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code"
#$CFLAGS += " -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls"
#$CFLAGS += " -Wold-style-definition -Werror"
#$CFLAGS += " -ggdb3"
#$CFLAGS += " -O0"
#$CFLAGS += " -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing"
#$CFLAGS += " -lm"
#$LIBS += " #{`llvm-config --libs`.chomp}"
# When the libclang.so is in a custom directory:
# llvm-config --libdir => /usr/lib64/llvm in Fedora
# llvm-config --libdir => /usr/lib in Archlinux so not needed 
$LDFLAGS << " -L#{`llvm-config --libdir`.chomp}"
# override normal build configuration to build debug friendly library
# if installed via 'gem install clangc -- --enable-debug'
if enable_config('debug')
  puts '[INFO] enabling debug library build configuration.'
  if RUBY_VERSION < '1.9'
    $CFLAGS = CONFIG['CFLAGS'].gsub(/\s\-O\d?\s/, ' -O0 ')
    $CFLAGS.gsub!(/\s?\-g\w*\s/, ' -ggdb3 ')
    CONFIG['LDSHARED'] = CONFIG['LDSHARED'].gsub(/\s\-s(\s|\z)/, ' ')
  else
    CONFIG['debugflags'] << ' -ggdb3 -O0 -std=c89 -pedantic -Wall'
  end
end
have_library("clang", "clang_createIndex")

create_makefile("clangc/clangc")
