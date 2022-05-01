#!/bin/bash

echo "Create source and destination directories"
mkdir ~/source ~/destination

echo "Create files to synchronisation"
mkdir ~/source/a ~/source/b ~/source/b/bb ~/source/c
touch ~/source/file1 ~/source/a/file2 ~/source/b/file3

echo "Demon start with recursive function every 30 seconds"
demon -R -s 10 ~/source ~/destination

echo "Compare source directory with destination directory"
diff ~/source ~/destination