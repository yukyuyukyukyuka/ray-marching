# src file
SRCROOT = .
SRCDIRS = $(shell find $(SRCROOT) -type d)
SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

# obj file
OBJROOT = .
OBJDIRS = $(addprefix $(OBJROOT)/, $(SRCDIRS))
OBJECTS = $(addprefix $(OBJROOT)/, $(SOURCES:.cpp=.o))

# exe file
TARGETS   = ray-marching
TARGETDIR = ./bin
CMDARGS   = 

# library
LDFLAGS = -lglfw -lGLEW -framework OpenGL

# include
INCLUDE = -I./inc -I./inc/imgui

# compiler
COMPILER = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra

# generate exe files
$(TARGETS): $(OBJECTS)
	$(COMPILER) -o $(TARGETDIR)/$@ $^ $(LDFLAGS)

# generate obj files
$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $^

# execute exe files
.PHONY: run
run: $(TARGETS)
	cd $(TARGETDIR); ./$(TARGETS) $(CMDARGS); cd -

# delete files
.PHONY: clean
clean: 
	rm -f $(OBJECTS) $(TARGETDIR)/$(TARGETS)