CC = gcc
CXX = g++

EXE = relay

LIB_DIR = ./lib
SOURCE_DIR = ./src
OBJ_DIR = ./build

IMGUI_DIR = $(LIB_DIR)/imgui
LIBUIOHOOK_DIR = $(LIB_DIR)/libuiohook

APP_HEADERS = $(shell find ./inc -name "*.hpp")
APP_SOURCES = $(shell find ./src -name "*.cpp")

SOURCES = $(APP_SOURCES)
SOURCES += $(IMGUI_DIR)/imgui.cpp \
 $(IMGUI_DIR)/imgui_draw.cpp \
 $(IMGUI_DIR)/imgui_tables.cpp \
 $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl3.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_sdlgpu3.cpp

UNAME_S := $(shell uname -s)

CC_FLAGS = -I$(LIBUIOHOOK_DIR)
CXXFLAGS = -std=c++11 -I$(LIB_DIR) -I$(IMGUI_DIR) -Iinc
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

SOURCES += $(LIBUIOHOOK_DIR)/logger.c \
	$(LIBUIOHOOK_DIR)/$(OS_DIR)/input_helper.c \
	$(LIBUIOHOOK_DIR)/$(OS_DIR)/input_hook.c \
	$(LIBUIOHOOK_DIR)/$(OS_DIR)/post_event.c \
	$(LIBUIOHOOK_DIR)/$(OS_DIR)/system_properties.c

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJ_DIR)/%.o:$(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SOURCE_DIR)/vts/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SOURCE_DIR)/ws/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(LIBUIOHOOK_DIR)/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(LIBUIOHOOK_DIR)/$(OS_DIR)/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

debug: CXXFLAGS += -g -Wall -Wextra -pedantic
debug: all

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

format:
	clang-format -i $(APP_SOURCES) $(APP_HEADERS)

clean:
	rm -f $(EXE) $(OBJS)
