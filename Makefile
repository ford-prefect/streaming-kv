PROGRAMS = test-server \
	   test-client

OBJECTS = skv-server.o  \
	  skv-client.o  \
	  test-server.o \
	  test-client.o

LIBS = -lpthread

%.o: %.cc
	$(CXX) -g -c $^ -o $@

test-server: test-server.o skv-server.o
	$(CXX) $^ -o $@ $(LIBS)

test-client: test-client.o skv-client.o
	$(CXX) $^ -o $@ $(LIBS)

all: $(PROGRAMS)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS) $(PROGRAMS)
