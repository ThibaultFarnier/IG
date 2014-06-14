# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
SOURCE      = ./src
INCFLAGS	:= -I${INCLUDES}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library
LIBEIOBJS	:= ${OBJDIR}/ei_application.o ${OBJDIR}/ei_widget.o ${OBJDIR}/ei_geometrymanager.o ${OBJDIR}/ei_widgetclass.o ${OBJDIR}/ei_event.o ${OBJDIR}/ei_linked_point.o ${OBJDIR}/ei_callbacks.o ${OBJDIR}/ei_picksurface.o ${OBJDIR}/ei_params_list.o


# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X
	
	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image -framework AppKit

else
	ifeq (${PLATFORM},Linux)

	# Building for Linux
	
	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
        LIBSDLGFX       = ${PLATDIR}/libSDL_gfx.a
        #LIBSDLGFX       = -lSDL_gfx
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf ${LIBSDLGFX} -lSDL_image

	else

	# Building for Windows
	
	PLATDIR		= _win
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL -I/usr/local/include/SDL
	LINK		= ${CC} -mwindows -mno-cygwin
	LIBEI		= ${OBJDIR}/libei.lib
	LIBEIBASE	= ${PLATDIR}/libeibase.lib
	LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a /usr/lib/SDL_image.lib -L/usr/lib -lSDL

	endif
endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal frame button buttons hello_world_absolute hello_world_relative puzzle debug_event
all : ${TARGETS}



########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# buttons

buttons : ${OBJDIR}/buttons.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o buttons ${OBJDIR}/buttons.o ${LIBEI} ${LIBS}

${OBJDIR}/buttons.o : ${TESTS}/buttons.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/buttons.c -o ${OBJDIR}/buttons.o

# toplevel

toplevel : ${OBJDIR}/toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${OBJDIR}/toplevel.o ${LIBEI} ${LIBS}
${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o

# hello_world_absolute

hello_world_absolute : ${OBJDIR}/hello_world_absolute.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world_absolute ${OBJDIR}/hello_world_absolute.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world_absolute.o : ${TESTS}/hello_world_absolute.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world_absolute.c -o ${OBJDIR}/hello_world_absolute.o

# hello_world_relative

hello_world_relative : ${OBJDIR}/hello_world_relative.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world_relative ${OBJDIR}/hello_world_relative.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world_relative.o : ${TESTS}/hello_world_relative.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world_relative.c -o ${OBJDIR}/hello_world_relative.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o

### AJOUT DE TESTS PERSO!
debug_event : ${OBJDIR}/debug_event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o debug_event ${OBJDIR}/debug_event.o ${LIBEI} ${LIBS}

${OBJDIR}/debug_event.o : ${TESTS}/debug_event.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/debug_event.c -o ${OBJDIR}/debug_event.o

filiation : ${OBJDIR}/filiation.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o filiation ${OBJDIR}/filiation.o ${LIBEI} ${LIBS}

${OBJDIR}/filiation.o : ${TESTS}/filiation.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/filiation.c -o ${OBJDIR}/filiation.o

puzzle_frame : ${OBJDIR}/puzzle_frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle_frame ${OBJDIR}/puzzle_frame.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle_frame.o : ${TESTS}/puzzle_frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle_frame.c -o ${OBJDIR}/puzzle_frame.o

# Building of the library libei
${OBJDIR}/ei_params_list.o : ${SOURCE}/ei_params_list.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_params_list.c -o ${OBJDIR}/ei_params_list.o

${OBJDIR}/ei_linked_point.o : ${SOURCE}/ei_linked_point.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_linked_point.c -o ${OBJDIR}/ei_linked_point.o

${OBJDIR}/ei_widgetclass.o : ${SOURCE}/ei_widgetclass.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_widgetclass.c -o ${OBJDIR}/ei_widgetclass.o

${OBJDIR}/ei_application.o : ${SOURCE}/ei_application.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_application.c -o ${OBJDIR}/ei_application.o

${OBJDIR}/ei_picksurface.o : ${SOURCE}/ei_picksurface.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_picksurface.c -o ${OBJDIR}/ei_picksurface.o

${OBJDIR}/ei_widget.o : ${SOURCE}/ei_widget.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_widget.c -o ${OBJDIR}/ei_widget.o

${OBJDIR}/ei_event.o : ${SOURCE}/ei_event.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_event.c -o ${OBJDIR}/ei_event.o

${OBJDIR}/ei_geometrymanager.o : ${SOURCE}/ei_geometrymanager.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_geometrymanager.c -o ${OBJDIR}/ei_geometrymanager.o

${OBJDIR}/ei_callbacks.o : ${SOURCE}/ei_callbacks.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_callbacks.c -o ${OBJDIR}/ei_callbacks.o


${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
