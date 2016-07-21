# ChessPuter
ChessPuter (chess pyoo-ter) is a UCI (Universal Chess Interface) compatible** command line chess playing program written in C++.

ChessPuter can be compiled on Linux with its Makefile. It can be installed and played with best on PyChess (https://github.com/pychess/pychess).

** ChessPuter is not yet fully UCI compatible. To be fully compatible, threads must be implemented to allow for pausing a game while the program is thinking, to allow it to respond to a "isready" command as well as immediately respond to a stop command while thinking.
