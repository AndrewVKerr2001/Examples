#Directory that stores our objects, do not leave empty use '.' for current space.
OBJDIR=Objects
SRCDIR=Source
INCLUDE=Include

#Compiler and Compiler options.
CC=g++
CCFLAGS=-Wall -ansi -pedantic -I${INCLUDE}

#The target filename
TARGET=Imaginary #Dont forget to change me!

#Grab all cpp sources and dump them into our objects list.
CSOURCES=${shell echo ${SRCDIR}/*.cpp}
OBJECTS=${CSOURCES:${SRCDIR}/%.cpp=${OBJDIR}/%.o}

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
