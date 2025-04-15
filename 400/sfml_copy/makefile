BIN := main
PATH_SRC := src
PATH_TEST := test
PATH_BUILD := build

CXX := g++
OPT := -O0
DEPFLAGS := -MP -MD
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CXXFLAGS := -g -Wall -std=c++17 -fpermissive $(OPT) $(DEPFLAGS)

FILES_MAIN := $(wildcard $(PATH_SRC)/*.cpp)
FILES_TEST := $(wildcard $(PATH_TEST)/*.cpp)

OBJECTS_MAIN := $(patsubst $(PATH_SRC)/%.cpp,$(PATH_BUILD)/%.o,$(FILES_MAIN))
OBJECTS_TEST := $(patsubst $(PATH_TEST)/%.cpp,$(PATH_BUILD)/%.o,$(FILES_TEST))

DEPS_MAIN := $(patsubst $(PATH_SRC)/%.cpp,$(PATH_BUILD)/%.d,$(FILES_MAIN))
DEPS_TEST := $(patsubst $(PATH_TEST)/%.cpp,$(PATH_BUILD)/%.d,$(FILES_TEST))

ifeq ($(OS),Windows_NT)
	RM := rmdir /s /q
	MKDIR := if not exist "$(PATH_BUILD)" mkdir "$(PATH_BUILD)"
	RUN := $(PATH_BUILD)\$(BIN).exe
else
	RM := rm -rf
	MKDIR := mkdir -p $(PATH_BUILD)
	RUN := ./$(PATH_BUILD)/$(BIN)
endif

all: $(PATH_BUILD)/$(BIN)

$(PATH_BUILD)/$(BIN): $(OBJECTS_MAIN) $(OBJECTS_TEST)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(PATH_BUILD)/%.o: $(PATH_SRC)/%.cpp
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PATH_BUILD)/%.o: $(PATH_TEST)/%.cpp
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	$(RUN)

clean:
	$(RM) $(PATH_BUILD)

-include $(DEPS_MAIN) $(DEPS_TEST)

.PHONY: all run clean
