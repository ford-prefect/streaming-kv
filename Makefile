PROGRAMS = test-server \
	   test-client

OBJECTS = skv-server.o  \
	  skv-client.o  \
	  test-server.o \
	  test-client.o

%.o: %.cc
	$(CXX) -c $^ -o $@

test-server: test-server.o skv-server.o
	$(CXX) $^ -o $@

test-client: test-client.o skv-client.o
	$(CXX) $^ -o $@

all: $(PROGRAMS)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS) $(PROGRAMS)
