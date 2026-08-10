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

CoreDir = $(SrcDir)/core
AppsDir = $(SrcDir)/apps

# --- Core engine source files ---
# Selects all .cpp files automatically into a list Sources (space separated)
CoreSources = $(wildcard $(CoreDir)/*.cpp)
# $(patsubst PATTERN, REPLACEMENT, TEXT) does a search and replace
# For each entry in CoreSources, it maps "source/core/filename.cpp" to "build/core/filename.o" 
CoreObjects = $(patsubst $(CoreDir)/%.cpp, $(BuildDir)/core/%.o, $(CoreSources))

# --- App objects (each has an entrypoint main() function) ---
AppSources = $(wildcard $(AppsDir)/*.cpp)
AppObjects = $(patsubst $(AppsDir)/%.cpp, $(BuildDir)/apps/%.o, $(AppSources))

AppNames = $(patsubst $(AppsDir)/%.cpp, %, $(AppSources))

# Final executable Make will try to build
AppTargets = $(patsubst %, $(BuildDir)/%, $(AppNames))

# Ensures that the "all" and "clean" targets are always executed when called
# even if a file with the same name exists
.PHONY: all clean

# Defines what happens when you run "make" with no arguments 
# Will build every app binary
all: $(AppTargets)


# Link rule: builds every app bianary
# For a target like build/draughts_gameplay, % = "draughts_gameplay"
# so the prerequisites become: build/apps/draughts_gameplay.o  +  all of $(CoreObjects)
#   $^    : Indicates all the prerequisites
#   $@    : Indicates the target (build/draughts)
$(AppTargets): $(BuildDir)/%: $(BuildDir)/apps/%.o $(CoreObjects)
	$(Compiler) $(Flags) $^ -o $@ $(LinkFlags)

# Compile rule for core engine files: source/core/*.cpp -> build/core/*.o
# This is done once per object file needed (substitutes % each time)
# "| $(BuildDir)/core" guarantees /build/core exists but touching the build/core folder timestamp does not trigger a rebuild of the .o files
#   -c    : compile to object file only (do not link)
#   $<    : Indicates the first prerequisite (source/Board.cpp)
#   $@    : Indicates the target (build/core/Board.o)
$(BuildDir)/core/%.o: $(CoreDir)/%.cpp | $(BuildDir)/core
	$(Compiler) $(Flags) -c $< -o $@

# Compile rule for app files: source/apps/*.cpp -> build/apps/*.o
$(BuildDir)/apps/%.o: $(AppsDir)/%.cpp | $(BuildDir)/apps
	$(Compiler) $(Flags) -c $< -o $@

# Creates the build directory to reflect the source directory if it does not exist
# -p flag ensures there is no error thrown if the directory already exists
$(BuildDir)/core:
	mkdir -p $(BuildDir)/core

$(BuildDir)/apps:
	mkdir -p $(BuildDir)/apps

# Defines what happens when you run "make clean"
# Removes the build direcotry and all its contents
clean:
	rm -rf $(BuildDir)