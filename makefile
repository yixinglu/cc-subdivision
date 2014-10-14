sources := *.cpp
objects := $(patsubst %.cpp,%.o,$(sources))

vpath % source

CC=g++

ccsubd: $(objects)
	$(CC) -o $@ $<

clean:
	rm -f *.o ccsubd
	
.PHONY: clean


