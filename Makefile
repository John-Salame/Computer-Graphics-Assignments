# HW5
EXE=hw5
SCENE_DEPS=myCSCI5229.h candyCane.h scenes.h

# Main target
all: $(EXE)

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Main file dependencies
hw5.o: hw5.c $(SCENE_DEPS)

# myCSCI5229.a dependencies
fatal.o: fatal.c myCSCI5229.h
errcheck.o: errcheck.c myCSCI5229.h
print.o: print.c myCSCI5229.h

# scene dependencies
candyCane.o: candyCane.h myCSCI5229.h
snow.o: snow.c snow.h myCSCI5229.h
scene0.o: scene0.c candyCane.h snow.h myCSCI5229.h

#  Create archive
myCSCI5229.a:fatal.o errcheck.o print.o
	ar -rcs $@ $^
	
scenes.a: candyCane.o snow.o scene0.o 
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
hw5:hw5.o scenes.a myCSCI5229.a
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
