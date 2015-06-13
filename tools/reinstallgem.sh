#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
W_DIR=$(pwd)
gem uninstall clangc
gem build $DIR/../clangc.gemspec
gem install --debug --verbose $W_DIR/clangc-0.0.1.gem -- --enable-debug
ruby $DIR/../test/Clangc_tests.rb --pride
ruby $DIR/../test/Index_tests.rb --pride
ruby $DIR/../test/TranslationUnit_tests.rb --pride
ruby $DIR/../test/Diagnostic_tests.rb --pride
ruby $DIR/../test/File_tests.rb --pride
rm $W_DIR/clangc-0.0.1.gem
