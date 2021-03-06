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

.PHONY: clean 

all: ${TARGETS} 

$(TARGETS): copyobjects $(OBJECTS) ../headers/config.h
	-@mv ../headers/config.h ../headers/config.h.bak
	-@mv ../headers/test-config.h ../headers/config.h
	${CC} ${LINKFLAGS} -g -o $@ $(patsubst bin/%.out, obj/%.o, $@ ) $(INTERNAL) ${LIBS} 
	-@mv ../headers/config.h ../headers/test-config.h
	-@mv ../headers/config.h.bak ../headers/config.h

clean:
	rm -f obj/*.o ${TARGETS}
	rm -f ../obj/*.o

copyobjects:
	-@mv ../headers/config.h ../headers/config.h.bak
	-@mv ../headers/test-config.h ../headers/config.h
	rm -f obj/*.o 
	cd ../ && make clean all && cd test
	cp ../obj/*.o obj/
	-@mv ../headers/config.h ../headers/test-config.h
	-@mv ../headers/config.h.bak ../headers/config.h

$(OBJECTS): obj/%.o : src/%.c ../headers/config.h
	${CC} ${CFLAGS} -c -o $@ $<

test: $(patsubst bin/%.out, %, $(TARGETS))

$(patsubst bin/%.out, %, $(TARGETS)): $(TARGETS)
	-@rm /tmp/data-test -r || true #remove any test data created so far
	@echo "Running Test bin/$@.out"
	$(valgrind) ./bin/$@.out #ignore error so that we place config back	