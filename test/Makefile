#Configurations and setup
CC = cc
CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -Werror -g -I../headers -Isrc
LINKFLAGS = $(CFLAGS)
LIBS = ../lib/wolkykim-qdecoder-63888fc/src/libqdecoder.a

OBJECTS := $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
TARGETS := $(patsubst src/%.c,bin/%.out,$(wildcard src/*.c))

#internals are non cgi scripts that we need
INTERNAL := $(patsubst src/internal/%.c,obj/%.o,$(wildcard ../src/internal/*.c))

#Commands to help test and run programs:	
valgrind = valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes

all: copyobjects ${TARGETS} 

$(TARGETS): $(OBJECTS) ../obj/*.o
	${CC} ${LINKFLAGS} -g -o $@ $(patsubst bin/%.out, obj/%.o, $@ ) $(INTERNAL) ${LIBS}

clean:
	rm -f obj/*.o ${TARGETS}

copyobjects:
	rm -f obj/*.o 
	cd ../ && make && cd test
	cp ../obj/*.o obj/

$(OBJECTS): obj/%.o : src/%.c
	${CC} ${CFLAGS} -c -o $@ $<

test: $(TARGETS)
	-@rm /tmp/data-test || true #remove any test data created so far
	$(valgrind) ./$<