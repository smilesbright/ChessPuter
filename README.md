# ChessPuter
ChessPuter (chess pyoo-ter) is a UCI (Universal Chess Interface) compatible** command line chess playing program written in C++.

ChessPuter can be compiled on Linux with its Makefile. It can be installed and played with best on PyChess (https://github.com/pychess/pychess).

** ChessPuter is not yet fully UCI compatible. To be fully compatible, the following changes still need to be made:
  - Event handling using threads must be implemented to allow the program to immediately respond to commands such as "stop", "isready", and others
  - The command string process should be made to be case-insensitive
