INCFLAGS  = -I /usr/include/vecmath
INCFLAGS += -I /usr/include/GL

LINKFLAGS = -L. -lRK4 -lglut -lGL -lGLU
LINKFLAGS += -L /usr/local/lib -lvecmath

CFLAGS    = -g -Wall -no-pie
CC        = g++
SRCS      = $(wildcard *.cpp)
SRCS     += $(wildcard vecmath/src/*.cpp)
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
