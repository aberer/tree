AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CXXFLAGS= -std=c++11 -Wall -pedantic -Wextra -Weffc++ -I../src/

bin_PROGRAMS = test 
test_SOURCES = TreeTest.cpp ../src/Tree.cpp ../src/Tree.hpp
test_LDFLAGS=  -pthread
test_LDADD = /usr/src/gtest/libgtest.a
