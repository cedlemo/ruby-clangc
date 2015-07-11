#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
W_DIR=$(pwd)
gem uninstall clangc
gem build $DIR/../clangc.gemspec
gem install --debug --verbose $W_DIR/clangc-0.0.1.gem -- --enable-debug
for N in "Clangc" "Index" "TranslationUnit" "Diagnostic" "File" "SourceRange" "SourceLocation" "Cursor"; do
  echo "############> ${N}_tests.rb <###############"
  ruby "${DIR}/../test/${N}_tests.rb" --pride
done
rm $W_DIR/clangc-0.0.1.gem
