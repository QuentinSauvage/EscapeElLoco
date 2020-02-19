#-Wextra  -pedantic -Wno-unused-parameter -Wall -Wextra -ansi
CFLAGS = -std=c++11 -Wall
SFML = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
EXEC = bin/app

#BOOST = -I boost

INC = -I include
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: app

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CFLAGS) $(INC) -c -g -o $@ $<

app: main.cpp $(OBJ_FILES)
	$(CXX) $(INC) $(BOOST) -o $(EXEC) $^ $(SFML)

clean:
	rm -rf obj/*.o $(EXEC)


