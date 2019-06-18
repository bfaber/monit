CC = clang++

INC_DIR = ../inc
GTEST_LIB_DIR = /Users/bfaber/Desktop/play/cpp/googletest/googletest/mybuild/lib

OBJ_DIR = obj

PCRE_INC = /usr/local/include/pcre.h
GTEST_INC = /Users/bfaber/Desktop/play/cpp/googletest/googletest/googletest/include

MONIT_LIB_DIR = /Users/bfaber/Desktop/play/cpp/monit/src

LIBS = -lpcre -lgtest -lmonit
LIB_DIR = -L$(GTEST_LIB_DIR) -L$(MONIT_LIB_DIR)

CFLAGS = -g -Wall -std=c++11 -I$(INC_DIR) -I$(PCRE_INC) -I$(GTEST_INC)

_OBJ = logreadertest.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testmonit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o *~ testmonit $(INC_DIR)/*~ 
