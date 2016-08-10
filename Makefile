PROGRAMS = test-server \
	   test-client

OBJECTS = skv-server.o  \
	  skv-client.o  \
	  test-server.o \
	  test-client.o

CFLAGS = -std=c++14 -g
LIBS = -lpthread

all: $(PROGRAMS)

%.o: %.cc
	$(CXX) $(CFLAGS) -c $^ -o $@

test-server: test-server.o skv-server.o
	$(CXX) $^ -o $@ $(LIBS)

test-client: test-client.o skv-client.o
	$(CXX) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS) $(PROGRAMS)
