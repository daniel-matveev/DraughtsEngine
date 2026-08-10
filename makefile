# Compiler to use for every compile and link step
Compiler = clang++

# Flags used when compiling each .cpp file into a .o file:
#   -std=c++17      : Use the C++17 standard
#   -g              : Include debugging information
#   -Wall           : Enable all warning messages
#   -Iinclude       : Allows the compiler to find header files in the include directory
Flags = -std=c++17 -g -Wall -Iinclude

# Flags used when linking the object files into the final executable
LinkFlags = 

# Folder name variables
SrcDir = source
IncludeDir = include
BuildDir = build

# Selects all .cpp files automatically into a list Sources (space separated)
Sources = $(wildcard $(SrcDir)/*.cpp)

# $(patsubst PATTERN, REPLACEMENT, TEXT) does a search and replace
# For each entry in Sources, it maps "source/filename.cpp" to "build/filename.o" 
Objects = $(patsubst $(SrcDir)/%.cpp, $(BuildDir)/%.o, $(Sources))

# Final executable Make will try to build
Target = $(BuildDir)/draughts

# Ensures that the "all" and "clean" targets are always executed when called
# even if a file with the same name exists
.PHONY: all clean

# Defines what happens when you run "make" with no arguments. In this case, it will build the final executable.
all: $(Target)

# Link rule: builds the final executable from all the object files
# Lists $(Objects) as prerequisites
#   $^    : Indicates all the prerequisites
#   $@    : Indicates the target (build/draughts)
$(Target): $(Objects)
	$(Compiler) $(Flags) $^ -o $@ $(LinkFlags)

# Pattern rule: builds each .o file from its corresponding .cpp file
# This is done once per object file needed (substitutes % each time)
# "| $(BuildDir)" guarantees /build exists but touching the build/ folder timestamp does not trigger a rebuild of the .o files
#   -c    : compile to object file only (do not link)
#   $<    : Indicates the first prerequisite (source/Board.cpp)
#   $@    : Indicates the target (build/Board.o)
$(BuildDir)/%.o: $(SrcDir)/%.cpp | $(BuildDir)
	$(Compiler) $(Flags) -c $< -o $@

# Creates the build directory if it does not exist
# -p flag ensures there is no error thrown if the directory already exists
$(BuildDir):
	mkdir -p $(BuildDir)

# Defines what happens when you run "make clean"
# Removes the build direcotry and all its contents
clean:
	rm -rf $(BuildDir)