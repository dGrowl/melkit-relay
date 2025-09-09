CC = gcc
CXX = g++

EXE = relay

LIB_DIR = ./lib
SOURCE_DIR = ./src
OBJ_DIR = ./build

IMGUI_DIR = $(LIB_DIR)/imgui
LIBUIOHOOK_DIR = $(LIB_DIR)/libuiohook

UNAME_S := $(shell uname -s)

CC_FLAGS = -I$(LIBUIOHOOK_DIR)
CXXFLAGS = -std=c++20 -I$(LIB_DIR) -I$(IMGUI_DIR) -Iinc
LIBS = -lmongoose

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -ldl `pkg-config sdl3 --libs`

	CXXFLAGS += `pkg-config sdl3 --cflags`
	CFLAGS = $(CXXFLAGS)

	OS_DIR = x11
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework Cocoa -framework IOKit -framework CoreVideo `pkg-config --libs sdl3`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `pkg-config sdl3 --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)

	OS_DIR = darwin
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -limm32 `pkg-config --static --libs sdl3`

	CXXFLAGS += `pkg-config --cflags sdl3`
	CFLAGS = $(CXXFLAGS)

	OS_DIR = windows
endif

APP_HEADERS = $(shell find ./inc -name "*.hpp")
APP_SOURCES = $(shell find $(SOURCE_DIR) -name "*.cpp")
IMGUI_SOURCES = $(shell find $(IMGUI_DIR) -name "*.cpp")
LIBUIOHOOK_SOURCES = $(shell find $(LIBUIOHOOK_DIR)/$(OS_DIR) -name "*.c")
LIBUIOHOOK_SOURCES += $(LIBUIOHOOK_DIR)/logger.c

APP_OBJS = $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJ_DIR)/app/%.o,$(APP_SOURCES))
IMGUI_OBJS = $(patsubst $(IMGUI_DIR)/%.cpp,$(OBJ_DIR)/imgui/%.o,$(IMGUI_SOURCES))
LIBUIOHOOK_OBJS = $(patsubst $(LIBUIOHOOK_DIR)/%.c,$(OBJ_DIR)/libuiohook/%.o,$(LIBUIOHOOK_SOURCES))

OBJS = $(APP_OBJS) $(IMGUI_OBJS) $(LIBUIOHOOK_OBJS)

vpath %.cpp $(SOURCE_DIR)
vpath %.cpp $(SOURCE_DIR)/core
vpath %.cpp $(SOURCE_DIR)/gui
vpath %.cpp $(SOURCE_DIR)/vts
vpath %.cpp $(SOURCE_DIR)/ws
vpath %.cpp $(IMGUI_DIR)
vpath %.cpp $(IMGUI_DIR)/backends
vpath %.c $(LIBUIOHOOK_DIR)
vpath %.c $(LIBUIOHOOK_DIR)/$(OS_DIR)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(OBJ_DIR)/app/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/imgui/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/libuiohook/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c -o $@ $<

debug: CXXFLAGS += -g -Wall -Wextra -pedantic -O0 -DSDL_ASSERT_LEVEL=2
debug: all

release: CXXFLAGS += -s -Ofast -DNDEBUG -fno-rtti -static-libgcc -static-libstdc++
release: all

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

format:
	clang-format -i $(APP_SOURCES) $(APP_HEADERS)

loc:
	find inc src -regex ".*\.\(hpp\|cpp\)$$" | xargs wc -l

clean:
	rm -f $(EXE)
	rm -rf $(OBJ_DIR)

clean-libs:
	rm -rf $(OBJ_DIR)/imgui $(OBJ_DIR)/libuiohook

clean-app:
	rm -rf $(OBJ_DIR)/app
