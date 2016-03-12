#!/bin/bash

case "$CLANG_SRC_VER" in
  35)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  *)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
esac
