#
# NCurses stuff makefile
# WRITE A HACK SCREEN

# Path to include folder -> header files
INC_DIR = inc/
SRC_DIR = src/
BUILDS_DIR = builds/

# List of all source files
OBJECTS = $(BUILDS_DIR)main.o $(BUILDS_DIR)hacks.o $(BUILDS_DIR)write.o $(BUILDS_DIR)about.o

# Processor settings
CFLAGS = -ggdb -Wall -O0 -I$(INC_DIR)
LIBS = -lmenu -lncurses -lform

# all: main

$(BUILDS_DIR)main: $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)


# $@ = filename of the target
# $< = filename of the first prerequisite
# $^ = filenames of all prerequisites separated by spaces
$(BUILDS_DIR)main.o : $(SRC_DIR)main.c
	gcc -o $@ -c $^ $(CFLAGS)

$(BUILDS_DIR)hacks.o : $(SRC_DIR)hacks.c
	gcc -o $@ -c $^ $(CFLAGS)

$(BUILDS_DIR)write.o : $(SRC_DIR)write.c
	gcc -o $@ -c $^ $(CFLAGS)

$(BUILDS_DIR)about.o : $(SRC_DIR)about.c
	gcc -o $@ -c $^ $(CFLAGS)


run: $(BUILDS_DIR)main
	# gnome-terminal --full-screen --profile LoRes_profile -x ./write
	# gnome-terminal --window --maximize -x ./write
	gnome-terminal  --geometry=180x50 -x ./$(BUILDS_DIR)main
	# gnome-terminal  --geometry=80x40 -x ./write
	# gnome-terminal -x ./write

.PHONY: clean
clean:
	rm -rf $(BUILDS_DIR)*.o
	rm $(BUILDS_DIR)main