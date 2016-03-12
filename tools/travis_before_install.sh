#!/bin/bash

case "$CLANG_SRC_VER" in
  35)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  36)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.6-dev libclang1-3.6 libclang-common-3.6-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.6.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  37)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.7-dev libclang1-3.7 libclang-common-3.7-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.7.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  *)
    apt-get update -qq
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
esac
