#!/usr/bin/env bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
W_DIR=$(pwd)
cd ext/clangc
ruby extconf.rb
cd $W_DIR
gem uninstall clangc
gem build $DIR/../clangc.gemspec
gem install --user-install --debug --verbose --local clangc-0.0.1.gem

for N in "Clangc" "Index" "TranslationUnit" "Diagnostic" "File" "SourceRange" \
  "SourceLocation" "Cursor" "Type" "CursorSet" "CompletionString" "Module"\
  "CodeCompleteResults" "CompletionResult"; do
  echo ""
  echo "############> ${N}_tests.rb <###############"
  ruby "${DIR}/../test/${N}_tests.rb" --pride
  echo ""
done


