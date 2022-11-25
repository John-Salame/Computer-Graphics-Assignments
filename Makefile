# project
EXE=project
SCENE_DEPS=myCSCI5229.h objects.h scenes.h
OBJECTS=candyCane.o snow.o ball.o

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
project.o: project.c $(SCENE_DEPS)

# myCSCI5229.a dependencies
fatal.o: fatal.c myCSCI5229.h
errcheck.o: errcheck.c myCSCI5229.h
print.o: print.c myCSCI5229.h
loadtexbmp.o: loadtexbmp.c myCSCI5229.h
loadProgram.o: loadProgram.c myCSCI5229.h

# objects.a dependencies
candyCane.o: candyCane.c objects.h myCSCI5229.h
snow.o: snow.c objects.h myCSCI5229.h
ball.0: ball.c objects.h myCSCI5229.h

# scenes.a dependencies
scene0.o: scene0.c objects.h myCSCI5229.h

#  Create archives
myCSCI5229.a:fatal.o errcheck.o print.o loadtexbmp.o loadProgram.o
	ar -rcs $@ $^	

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
project:project.o scene0.o $(OBJECTS) myCSCI5229.a 
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
