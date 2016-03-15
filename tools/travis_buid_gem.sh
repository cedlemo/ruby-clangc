#!/usr/bin/env bash
W_DIR=$(pwd)
cd ext/clangc
ruby extconf.rb
cd $W_DIR
gem uninstall clangc
gem build $W_DIR/clangc.gemspec

