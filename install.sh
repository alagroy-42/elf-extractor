#!/bin/bash

install_dir=$HOME/scripts
bin_name="elf-extractor"
SHELL="/bin/zsh"

mkdir -p $install_dir
gcc -Wall -Werror -Wextra -g -fsanitize=address extractor.c -o $install_dir/$bin_name
echo export PATH=$install_dir:'$PATH' >> $HOME/.$(basename $SHELL)rc
