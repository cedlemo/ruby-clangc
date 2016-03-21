#!/bin/bash

case $CLANG_SRC_VER in
  35)
    echo "$CLANG_SRC_VER"
    apt-get update -qq
    echo "install the libs for $CLANG_SRC_VER"
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    echo "add symbolic link for $CLANG_SRC_VER"
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  36)
    echo "${CLANG_SRC_VER}"
    apt-get update -qq
    echo "install the libs for $CLANG_SRC_VER"
    apt-get install -qq llvm-dev libclang-3.6-dev libclang1-3.6 libclang-common-3.6-dev build-essential ruby-dev
    echo "add symbolic link for $CLANG_SRC_VER"
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.6.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  37)
    echo "$CLANG_SRC_VER"
    apt-get update -qq
    echo "install the libs for $CLANG_SRC_VER"
    apt-get install -qq llvm-dev libclang-3.7-dev libclang1-3.7 libclang-common-3.7-dev build-essential ruby-dev
    echo "add symbolic link for $CLANG_SRC_VER"
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.7.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  38)
    echo "$CLANG_SRC_VER"
    apt-get update -qq
    echo "install the libs for $CLANG_SRC_VER"
    apt-get install -qq llvm-dev libclang-3.8-dev libclang1-3.8 libclang-common-3.8-dev build-essential ruby-dev
    echo "add symbolic link for $CLANG_SRC_VER"
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.8.so /usr/lib/x86_64-linux-gnu/libclang.so;;
  *)
    echo "$CLANG_SRC_VER"
    apt-get update -qq
    echo "install the libs for $CLANG_SRC_VER"
    apt-get install -qq llvm-dev libclang-3.5-dev libclang1-3.5 libclang-common-3.5-dev build-essential ruby-dev
    echo "add symbolic link for $CLANG_SRC_VER"
    ln -s /usr/lib/x86_64-linux-gnu/libclang-3.5.so /usr/lib/x86_64-linux-gnu/libclang.so;;
esac
