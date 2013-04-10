#/*
# *
# *	Copyright (C) 2012 Thomas MAURICE
# *
# *	This program is free software; you can redistribute it and/or modify
# *	it under the terms of the GNU General Public License as published by
# *	the Free Software Foundation; either version 2 of the License, or
# *	(at your option) any later version.
# *
# *	This program is distributed in the hope that it will be useful,
# *	but WITHOUT ANY WARRANTY; without even the implied warranty of
# *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# *	GNU General Public License for more details.
# *
# *	You should have received a copy of the GNU General Public License along
# *	with this program; if not, write to the Free Software Foundation, Inc.,
# *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# */

CPP=g++
AR=ar
ARFLAGS=-q
CXXFLAGS=-W -Wall -Wmain -O3 -Wno-deprecated -fPIC
# Détermination de l'extension des binaires
ifneq ($(shell uname -s),Linux)
LDFLAGS=-rdynamic -fPIC -lwsock32 ../bin-win32/libplugin.dll ../bin-win32/libtsocket.dll ../bin-win32/libssl32.dll ../bin-win32/libeay32.dll ../bin-win32/ssleay32.dll -lwininet
EXT=.dll
else
LDFLAGS=../bin-linux/libtsocket.a ../bin-linux/libplugin.a -ldl -rdynamic -fPIC -lssl -lcrypto
EXT=.so
endif
EXEC=../Yarilo
LIB=./lib/libyarilo.a
DYNLIB=./lib/libyarilo$(EXT)
SRCDIR=./
SRC_LIB=$(SRCDIR)/*.cpp
ifneq ($(shell uname -s),Linux)
OBJ_LIB= $(SRC_LIB:.cpp=.o) sha1/sha1.o regex/regex.o ../bin-win32/*.dll
REGEX_FLAGS=-DSTDC_HEADERS=1 -DHAVE_STRING_H=1
else
OBJ_LIB= $(SRC_LIB:.cpp=.o) sha1/sha1.o regex/regex.o
REGEX_FLAGS=-DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DHAVE_ALLOCA_H=1
endif
SRC= $(wildcard $(SRCDIR)*.cpp)
OBJ= $(SRC:.cpp=.o) sha1/sha1.o regex/regex.o
HEADERDIR=../include/ -I ../include/libtsocket -I ..
API_HEADER=./lib/bot_api.h

all: bin api

regex/regex.o: regex/regex.c
	gcc $(REGEX_FLAGS) -fPIC -I ../include/ -c regex/regex.c -o regex/regex.o

bin: $(OBJ) $(MOD) 
	@echo "Création de l'executable..."
	$(CPP) -o $(EXEC) $^ $(LDFLAGS)

api: $(OBJ_LIB) $(MOD)
	@echo "Création de la lib statique..."
	@rm -rf $(LIB)
	@rm -rf $(DYNLIB)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ_LIB)
	@echo "Creation de la lib dynamique..."
	$(CPP) -shared -fPIC $(OBJ_LIB) -o $(DYNLIB)
	@echo "Création du header de l'API..."
	@echo "#ifndef YARILO_BOT_API" > $(API_HEADER)
	@echo "#define YARILO_BOT_API" >> $(API_HEADER)
	@echo "class Bot;" >> $(API_HEADER)
	@echo "class Server;" >> $(API_HEADER)
	@echo "class User;" >> $(API_HEADER)
	@echo "class Event;" >> $(API_HEADER)
	@cat ../include/*.h >> $(API_HEADER)
	@echo "#endif" >> $(API_HEADER)

%.o: %.cpp
	$(CPP) -o $@ -c $< -I $(HEADERDIR) $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	@if ! [ -z "`ls $(SRCDIR)*.o 2> /dev/null`" ] ; then \
		echo -n "Nettoyage des sources "; \
		touch src.nettoyage.ok ; \
	fi;
	@for i in `ls $(SRCDIR)*.o 2> /dev/null` ; do \
		echo -n "." ; \
		rm $$i; \
	done;
	@if [ -e src.nettoyage.ok ] ; then \
		echo ""; \
		rm -f src.nettoyage.ok ; \
	fi;
	@$(MAKE) -C sha1 clean
	@ rm -rf regex/*.o
	@rm -rf lib/libyarilo.*

mrproper: clean
	@echo "Suppression de l'executable..."
	@rm -rf $(EXEC)
	@echo "Suppression de la lib statique..."
	@rm -rf $(LIB)
	@echo "Suppression du header de l'API..."
	@rm -rf $(API_HEADER)
	
