obj_dir = obj
src_dir = src
lib_dir = lib
obj_dir = obj

INCLUDE_OPTS = -I $(lib_dir)

COMPILER_FLAGS = -O3 -s -DNDEBUG -std=c++17 -pthread
LINKER_FLAGS = -pthread

COMPILER_OPTS = $(COMPILER_FLAGS) $(INCLUDE_OPTS)
LINKER_OPTS = $(LINKER_FLAGS)

objects = main.o
object_files = $(obj_dir)/main.o

all: $(objects)
	$(CXX) $(LINKER_OPTS) -o lab $(object_files)
    
main.o: $(src_dir)/main.cpp
	$(CXX) $(COMPILER_OPTS) -c $(src_dir)/main.cpp -o $(obj_dir)/main.o

clean :
	rm $(object_files)