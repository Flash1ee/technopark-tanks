TARGET = game
СOMPILER = g++

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = ./include

SRCS = ./src/*.cpp \
		game.cpp

.PHONY: all clean

all: $(SRCS)
	$(СOMPILER) $(CFLAGS) -I$(HDRS) $(SRCS) -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -rf $(TARGET)
