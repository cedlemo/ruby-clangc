# Files overview:

# Wrapping helpers:

## The clang-c lib headers for each versions
*    clang-3.5/
*    clang-3.6/
*    clang-3.7/
*    clang-3.8/

## Scripts that help to wrapp, or analyse the headers:
*    toolbox.rb
*    binder.rb
*    clangc_classes_generator.rb
*    clang_version_diff.rb
*    classes.rb
*    enums.rb
*    update_constants_in_other_files.rb
*    update_constants_in_tests_files.rb

Note: code sucks for most of them but that not the point

## Scripts to reinstall/test and visualize status:
*    reinstallgem.sh
*    status.rb

## Scripts used in travis
*    travis_before_install.sh
*    travis_build_gem.sh
*    travis_run_tests.sh
