CXX = g++
CXXFLAGS	:= -g -Wall -std=c++17 -fno-stack-protector
OUTPUT	:= output
BUILD 			:= build
SRC		:= src
INCLUDE	:= include
LIB		:= 
LFLAGS = -lSDL2 -lGL -lGLEW -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net



MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
BUILDDIRS := $(addprefix $(BUILD)/, $(SOURCEDIRS))
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
OBJECTS		:= $(addprefix $(BUILD)/, $(SOURCES:.cpp=.o))

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))


all: $(OUTPUT) $(BUILDDIRS) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(BUILDDIRS):
	$(MD) $(BUILDDIRS)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) $(LFLAGS) -o $(OUTPUTMAIN)

$(BUILD)/%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN) ${args}
	@echo Executing 'run: all' complete!
