CC = clang++

INC_DIR = ../inc
GTEST_LIB_DIR = /Users/bfaber/Desktop/play/cpp/googletest/googletest/mybuild/lib

OBJ_DIR = obj

PCRE_INC = /usr/local/include/pcre.h
GTEST_INC = /Users/bfaber/Desktop/play/cpp/googletest/googletest/googletest/include

MONGO_INC=/usr/local/include/libmongoc-1.0
BSON_INC=/usr/local/include/libbson-1.0

MONIT_LIB_DIR = /Users/bfaber/Desktop/play/cpp/monit/src

LIBS = -lpcre -lgtest -lmonit -lmongoc-1.0 -lbson-1.0
LIB_DIR = -L$(GTEST_LIB_DIR) -L$(MONIT_LIB_DIR)

CFLAGS = -g -Wall -std=c++14 -I$(INC_DIR) -I$(PCRE_INC) -I$(GTEST_INC) -I$(MONGO_INC) -I$(BSON_INC)

_OBJ = configitemtest.o recordtest.o logreadertest.o testspooler.o testprocessor.o testmongointerface.o filereadertest.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testmonit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o *~ testmonit $(INC_DIR)/*~ 
