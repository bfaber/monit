CC = clang++

INC_DIR = ../inc
GTEST_LIB_DIR = /Users/bfaber/Desktop/play/cpp/googletest/googletest/mybuild/lib

OBJ_DIR = obj

PCRE_INC = /usr/local/include/pcre.h
GTEST_INC = /Users/bfaber/Desktop/play/cpp/googletest/googletest/googletest/include

LIBS = -lpcre -lgtest
LIB_DIR = -L$(GTEST_LIB_DIR)

CFLAGS = -g -Wall -std=c++11 -I$(INC_DIR) -I$(PCRE_INC) -I$(GTEST_INC)

_DEPS = logreader.h csvprocessor.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = csvprocessor.o logreader.o main.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))


$(OBJ_DIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

monit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_DIR)

# for testing
libmonit.a: obj/csvprocessor.o obj/logreader.o
	ar rcs libmonit.a obj/csvprocessor.o obj/logreader.o

test: libmonit.a

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o *~ monit $(INC_DIR)/*~ 


