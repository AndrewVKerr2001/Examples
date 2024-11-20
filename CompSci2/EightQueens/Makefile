#Compiler and Compiler options.
CC=g++
CCFLAGS=-Wall -ansi -pedantic

#The target filename.
TARGET=qSolver

#Directory that stores our objects, do not leave empty use '.' for current space.
OBJDIR=obj

#Grab all cpp sources and dump them into our objects list.
CSOURCES=${shell echo *.cpp}
OBJECTS=${CSOURCES:%.cpp=${OBJDIR}/%.o}

#Compile the target when the all rule (def rule) executes.
all: dirstruct ${TARGET}

#Compile our target from our objects.
${TARGET}: ${OBJECTS}
	${CC} -o ${TARGET} ${OBJECTS}
	
include ../base.mk
