TARGET = game
СOMPILER = g++

# XXX: Don't forget backslash at the end of any line except the last one
# HDRS = 
	#    project/include

SRCS = game.cpp

.PHONY: all clean

all: $(SRCS)
	$(СOMPILER) $(CFLAGS) $(SRCS) -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -rf $(TARGET)
