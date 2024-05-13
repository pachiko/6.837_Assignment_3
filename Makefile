INCFLAGS  = -I /usr/include/vecmath

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -lvecmath
# Local archive
LINKFLAGS += -L . -lRK4

CFLAGS    = -g -Wall -no-pie
CC        = g++
SRCS      = $(wildcard *.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a3

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)
