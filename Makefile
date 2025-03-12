CC = g++
CFLAGS = --std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
# Your .hpp files
DEPS = CelestialBody.hpp Universe.hpp
# Your compiled .o files
OBJECTS = CelestialBody.o Universe.o
LIBRARY = NBody.a
TEST_EXEC = test
PROGRAM = NBody
# The name of your program

.PHONY: all clean lint


all: $(PROGRAM) $(TEST_EXEC) $(LIBRARY)

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): main.o $(OBJECTS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(TEST_EXEC): test.o $(OBJECTS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARY) $(LIB)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm -f *.o *.d $(PROGRAM) $(TEST_EXEC) $(LIBRARY)

lint:
	cpplint *.cpp *.hpp

-include $(OBJECTS:.o=.d)