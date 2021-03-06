# Detect toolchain or OS by different methods
# First check if PATH contains any ';' (that's the seperator on Windows ; afaik nobody sane uses it on any other OS for any purpose so if it contains any of them we're assuming this is Windows)
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
	# Then we get the name through uname
    detected_OS := $(shell uname 2>/dev/null || echo __UnknownOS)
	# And then detect some common Unix-on-Windows toolchains
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MINGW,$(detected_OS))
endif

# If we're on MINGW, we can use native Windows stuff
ifeq ($(detected_OS),MINGW)
	detected_OS := Windows
endif

# Warning options fed to the compiler
WARNINGS := -pedantic -Wall -Wextra -Walloc-zero -Wbool-compare -Wcast-align -Wcast-qual -Wchar-subscripts -Wchkp -Wdangling-else -Wdisabled-optimization -Wduplicated-branches -Wduplicated-cond -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winvalid-pch -Wlogical-not-parentheses -Wlogical-op -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wnoexcept -Wnoexcept-type -Wnormalized=nfc -Woverloaded-virtual -Wpointer-arith -Wregister -Wrestrict -Wsign-promo -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing=3 -Wstrict-null-sentinel -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-override -Wswitch-bool -Wundef -Wunreachable-code -Wunused -Wunused-local-typedefs -Wuseless-cast -Wvariadic-macros -Wwrite-strings -Wzero-as-null-pointer-constant -Wno-multichar -Wno-unused-parameter

# Optimisation options fed to the compiler
OPTIMISATIONS := -O3 -frename-registers
# OPTIMISATIONS += -flto
# Link-time optimizing makes the linking super long so it's unactivated by default

# Base command-line when calling the compiler
# `sdl2-config --cflags` to get the flags needed to compile with SDL2
# -std=c++17 to activate C++17 features
# -MMD -MP -MF $@.d to make the compiler generate dependency files
# -c so the compiler makes a simple compile into an object file
CXXFLAGS = $(CXX) $(OPTIMISATIONS) $(WARNINGS) `sdl2-config --cflags` -std=c++17 -c -MMD -MP -MF $@.d
LDFLAGS := $(CXX) $(OPTIMISATIONS) $(WARNINGS) -s

ifeq ($(detected_OS),Windows)
# We can use native icons
	CXXFLAGS += -DUSE_ICONS_WINDOWS
# --static-libs is windows-exclusive. Also static linking to avoid having to distribute DLLs with the exe
	LDFLAGS += -static `sdl2-config --static-libs`
else
# Normal linker SDL2 flags for Unix
	LDFLAGS += `sdl2-config --libs`
endif

# Main source file
MAIN := main
# States the game can be in
MAIN += game inventory mapSystem stageSelect
# Random utilities
MAIN += filesystem flags input loadConfig log mathUtils script stage stdUtils
# Collision handling
MAIN += bossCollision bulletCollision npcCollision playerCollision
# Drawing-related
MAIN += fade flash hud render
# External Libraries
MAIN += lodepng/lodepng
# Sound-related
MAIN +=	org pxt sound
# Classes
MAIN += boss bullet caret npc player weapons valueview
# Weapon behavior
MAIN += bladeThrow bubblePew fireballShoot machineGunShoot missileShoot nemesisShoot polarStarShoot snakeShoot spurShoot
# Bullet behavior
MAIN += blade bubbler fireball missile polarStar machineGun misc nemesis snake spur
# NPC behavior
MAIN += npcAct npc000 npc020 npc040 npc060 npc080 npc100 npc120 npc140 npc160 npc180 npc200 npc220 npc240 npc260 npc280 npc300 npc320 npc340 npc360
# Boss behavior
MAIN += balfrog ballos heavyPress monsterX omega core

OBJS := $(addprefix obj/, $(addsuffix .o, $(MAIN)))
ifeq ($(detected_OS),Windows)
	# Embed icons as resources, and load them natively
	OBJS += obj/icon.o
endif
DEPS := $(addsuffix .d, $(OBJS))

# Default target
all: bin/CaveStoryRecoded

# CaveStoryRecoded binary
bin/CaveStoryRecoded: $(OBJS)
	@mkdir -p $(@D)
	$(LDFLAGS) $(OBJS) -o $@

# Generic source compile
obj/%.o: source/%.cpp
	@mkdir -p $(@D)
	$(CXXFLAGS) $< -o $@

# Compile for lodepng (avoid warnings and also some other stuff)
obj/lodepng/lodepng.o: source/lodepng/lodepng.cpp
	@mkdir -p $(@D)
	$(CXXFLAGS) $< -o $@ -DLODEPNG_NO_COMPILE_ENCODER -DLODEPNG_NO_CXXFLAGS -Wno-zero-as-null-pointer-constant -Wno-suggest-attribute=pure -Wno-suggest-attribute=const -Wno-alloc-zero -Wno-useless-cast -Wno-cast-qual
	
ifeq ($(detected_OS),Windows)
# Compile icon resource
obj/icon.o: res/icon.rc res/icon_mini.ico
	@mkdir -p $(@D)
	@windres $< $@
else
# Compile icon from file
source/icon_mini.h: res/icon_mini.png
	@xxd -i $< $@
endif

# Include dependencies
include $(wildcard $(DEPS))

# Remove all objects files and the binary
clean:
	@rm -rf obj bin/CaveStoryRecoded
