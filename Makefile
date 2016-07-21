# ChessPuter
# Makefile
# Miles Bright

CXX = main.cpp
CXX += Computer.hpp
CXX += Computer.cpp
CXX += Position.hpp
CXX += Position.cpp

default:
	g++ -std=c++0x ${CXX} -o ChessPuter

optimized:
	g++ -std=c++0x ${CXX} -O3 -o ChessPuter
