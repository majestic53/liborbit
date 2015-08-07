# liborbit
# Copyright (C) 2015 David Jolly
# ----------------------
#
# liborbit is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# liborbit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -pthread -std=gnu++11 -O3 -Wall -Werror -fPIC
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
LIB=liborbit.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)orbit.o $(DIR_BUILD)orbit_exception.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: orbit.o orbit_exception.o

orbit.o: $(DIR_SRC)orbit.cpp $(DIR_INC)orbit.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)orbit.cpp -o $(DIR_BUILD)orbit.o

orbit_exception.o: $(DIR_SRC)orbit_exception.cpp $(DIR_INC)orbit_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)orbit_exception.cpp -o $(DIR_BUILD)orbit_exception.o

# COMPONENTS

# LANGUAGE
