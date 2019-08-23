CC = clang++

INC_DIR = ../inc
GTEST_LIB_DIR = /Users/bfaber/Desktop/play/cpp/googletest/googletest/mybuild/lib

OBJ_DIR = obj

PCRE_INC = /usr/local/include/pcre.h
GTEST_INC = /Users/bfaber/Desktop/play/cpp/googletest/googletest/googletest/include

MONGO_INC=/usr/local/include/libmongoc-1.0
BSON_INC=/usr/local/include/libbson-1.0

LIBS = -lpcre -lmongoc-1.0 -lbson-1.0 -lgtest 
LIB_DIR = -L$(GTEST_LIB_DIR)

CFLAGS = -g -Wall -std=c++11 -I$(INC_DIR) -I$(PCRE_INC) -I$(GTEST_INC) -I$(MONGO_INC) \
-I$(BSON_INC)

_DEPS = logreadernew.h mongorecordprocessor.h util.h mongointerface.h mongospooler.h configitem.h matchprocessor.h matchbundle.h record.h filereader.h textparser.h logreaderexecutor.h processorexecutor.h filebundle.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = mongorecordprocessor.o logreadernew.o util.o main.o mongointerface.o mongospooler.o configitem.o matchbundle.o record.o matchbuffer.o filereader.o textparser.o logreaderexecutor.o processorexecutor.o filebundle.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))


$(OBJ_DIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

monit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LIB_DIR)

# for testing
libmonit.a: obj/mongorecordprocessor.o obj/logreadernew.o obj/util.o obj/mongospooler.o obj/mongointerface.o obj/configitem.o obj/matchbundle.o obj/record.o obj/matchbuffer.o obj/filereader.o obj/textparser.o obj/logreaderexecutor.o obj/processorexecutor.o obj/filebundle.o
	ar rcs libmonit.a obj/mongorecordprocessor.o obj/logreadernew.o obj/util.o obj/mongospooler.o obj/mongointerface.o obj/configitem.o obj/matchbundle.o obj/record.o obj/matchbuffer.o obj/filereader.o obj/textparser.o obj/logreaderexecutor.o obj/processorexecutor.o obj/filebundle.o

test: libmonit.a

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o *~ monit $(INC_DIR)/*~ 


