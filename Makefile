# Disclaimer: This makefile was entirely made with LLMs, with only minor changes/fixes

# ------------------------------------------------------------------
# 1. USER CONFIG
# ------------------------------------------------------------------
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -MMD -MP
SRCDIR   := src
INCDIR   := include
BUILDDIR := build
TARGET   := interpreter

# ------------------------------------------------------------------
# 2. SOURCE DISCOVERY
# ------------------------------------------------------------------
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
DEPS     := $(OBJECTS:.o=.d)

# ------------------------------------------------------------------
# 3. TARGETS
# ------------------------------------------------------------------
.PHONY: all build clean
all: build

build: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

# ------------------------------------------------------------------
# 4. AUTO DEPENDENCIES
# ------------------------------------------------------------------
-include $(DEPS)
