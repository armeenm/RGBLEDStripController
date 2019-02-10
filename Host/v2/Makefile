TARGET_EXEC ?= sendcolors

SRC_DIRS ?= ./src
BUILD_DIR ?= ./build

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXX ?= g++
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
CXXFLAGS ?= -g -Wpedantic -Wall -std=c++17
LDFLAGS ?= -lpthread -lboost_system

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

print-%: ; @echo $* = $($*)
