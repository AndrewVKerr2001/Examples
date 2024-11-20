#Directory that stores our objects, do not leave empty use '.' for current space.
OBJDIR=Objects
SRCDIR=Source
INCLUDE=Include

#Compiler and Compiler options.
CC=g++
CCFLAGS=-Wall -ansi -pedantic -I${INCLUDE}
C=gcc
CFLAGS=-I${INCLUDE}

#The target filename
TARGET=sort #Dont forget to change me!

#Grab all cpp sources and dump them into our objects list.
CPPSOURCES = ${shell echo ${SRCDIR}/*.cpp}
OBJECTS = ${CPPSOURCES:${SRCDIR}/%.cpp=${OBJDIR}/%.o}

#Grab all c sources and dump them into our objects list. [DISABLE for non C projects.]
#CSOURCES = ${shell echo ${SRCDIR}/*.c}
#OBJECTS += ${CSOURCES:${SRCDIR}/%.c=${OBJDIR}/%.o}

#Compile the target when the all rule (def rule) executes.
all: ensure_fs ${TARGET}
	@echo [FINISHED] Make command 'all' has completed!

#Compile our target from our objects.
${TARGET}: compile_obj
	@echo [LINK] Linking all object code into executeable...
	${CC} -o ${TARGET} ${OBJECTS}
	@echo [DONE] Linking has completed!
	@echo
	
ifneq ("$(wildcard base.mk)","")
  	${info [INFO] Deployed mode, Used local base.mk file!}
  	include base.mk
else
	${info [INFO] Development mode, Used global base.mk file!}
  	include ../base.mk
endif
