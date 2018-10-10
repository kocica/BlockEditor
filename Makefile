###############################################################
# @file    Makefile                                           #
# @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>          #
# @date    01/03/2018                                         #
#                                                             #
# ICP project Makefile                                        #
###############################################################

################## Build constants ##############
BIN_NAME   = blockeditor

DOC        =
README     = README.txt
QMAKE      = qmake

CC         = g++
CFLAGS     = -std=c++14

LDFLAGS    =
LIBS       =
DOXYGEN    = doxygen

SRC        = src
GUI        = $(SRC)/gui

HEADERS    = $(wildcard $(SRC)/*.hpp)
SOURCES    = $(wildcard $(SRC)/*.cpp)
OBJS       = $(patsubst %.cpp, %.o, $(SOURCES))

GUI_SOURCES = $(wildcard $(GUI)/*.cpp)
GUI_HEADERS = $(wildcard $(GUI)/*.hpp)
GUI_OBJS    = $(patsubst %.cpp, %.o, $(GUI_SOURCES))

################## Compilation ##################

all: $(BIN_NAME)

$(BIN_NAME): $(HEADERS) $(SOURCES) $(OBJS) $(GUI_SOURCES) $(GUI_HEADERS)

	@cd $(GUI) && $(QMAKE) && make
	@mv $(shell pwd)/$(GUI)/$(BIN_NAME) .

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $< -o $@

################## Pack/Clean ##################

.PHONY: clean

doxygen:
	$(DOXYGEN) $(SRC)/doxyConf

pack:
	zip -r xkocic01-xvasic25.zip Makefile $(README) ./src/* ./doc/ ./examples/*

clean:
	-@cd $(GUI) && make clean && rm -f moc_*
	rm -f $(BIN_NAME) $(SRC)/*.o $(GUI)/*.o
	rm -rf doc/*

run:
	./$(BIN_NAME)
