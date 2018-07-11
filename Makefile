#
# NCurses stuff makefile
# WRITE A HACK SCREEN

# Path to include folder -> header files
INC_DIR = inc/
SRC_DIR = src

# List of all source files
OBJECTS = main.o hacks.o

# Processor settings
CFLAGS = -ggdb -Wall -O0 -I$(INC_DIR)
LIBS = -lmenu -lncurses -lform

all: main

main: $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)


# $@ = filename of the target
# $< = filename of the first prerequisite
# $^ = filenames of all prerequisites separated by spaces
main.o : $(SRC_DIR)/main.c
	gcc -c $^ $(CFLAGS)

hacks.o : $(SRC_DIR)/hacks.c
	gcc -c $^ $(CFLAGS)

run: main
	# gnome-terminal --full-screen --profile LoRes_profile -x ./write
	# gnome-terminal --window --maximize -x ./write
	gnome-terminal  --geometry=180x50 -x ./main
	# gnome-terminal  --geometry=80x40 -x ./write
	# gnome-terminal -x ./write

.PHONY: clean
clean:
	rm -rf *.o
	rm main