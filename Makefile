# ChessPuter
# Makefile
# Miles Bright


CXX = main.cpp
CXX += Engine.hpp
CXX += Engine.cpp
CXX += Position.hpp
CXX += Position_Setup.cpp
CXX += Position_MoveGen.cpp
CXX += Position_Analysis.cpp


default:
	g++ -std=c++0x ${CXX} -o ChessPuter


optimized:
	g++ -std=c++0x ${CXX} -O3 -o ChessPuter
