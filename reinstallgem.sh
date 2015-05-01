#!/bin/bash
gem uninstall clangc
gem build clangc.gemspec
gem install --debug --verbose clangc-0.0.1.gem -- --enable-debug
ruby test/Index_tests.rb
