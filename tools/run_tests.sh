#!/usr/bin/env bash
for N in "Clangc" "Index" "TranslationUnit" "Diagnostic" "File" "SourceRange" \
  "SourceLocation" "Cursor" "Type" "CursorSet" "CompletionString" "Module"\
  "CodeCompleteResults" "CompletionResult"; do
  echo ""
  echo "############> ${N}_tests.rb <###############"
  ruby "test/${N}_tests.rb"
  echo ""
done

