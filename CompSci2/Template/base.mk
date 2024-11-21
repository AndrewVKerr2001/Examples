#Default some of our options.
OBJDIR ?= Objects
SRCDIR ?= Sources
INCLUDE ?= Include
SUBMITDIR ?= Submit

DEFAULTNAME = a
TARGET ?= ${DEFAULTNAME}

DEPLOY ?= ${TARGET}


#Directory structure, make it in case we dont have it.
ensure_fs:
	@echo [SETUP] Ensuring file structure is up to date...
	mkdir -p ${SRCDIR}
	mkdir -p ${INCLUDE}
	mkdir -p ${OBJDIR}
	mkdir -p ${SUBMITDIR}
	@echo [DONE] File structure is up to date!
	@echo
ifeq ("$(TARGET)", "$(DEFAULTNAME)") 
	@echo [WARN] Still using TARGET value of a, dont forget to rename the TARGET executeable.
	@echo
endif

compile_obj: echo_build_obj ${OBJECTS}
	@echo [DONE] All objects are up to date!
	@echo

.PHONY: echo_build_obj
echo_build_obj: 
	@echo [BUILD] Building objects...

${OBJDIR}/%.o: ${SRCDIR}/%.c
	${C} -c ${CFLAGS} -o $@ $<

#Compile any obj code from cpp code.
${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	${CC} -c ${CCFLAGS} -o $@ $<

#Clean the current directory.
clean:
	@echo [CLEAN] Cleaning up all object code and executables.
	rm -f ${OBJDIR}/*.o ${TARGET}
	rm -rf ${OBJDIR}
	rm -rf ${SUBMITDIR}/*
	@echo [DONE] File sturcture has been fully cleaned of all object code/executables.
	@echo
	
deploy:
	@echo [DEPLOY] Creating the deployable zip file for submission...
	zip -rv ../${DEPLOY}.zip ${SUBMITDIR}
	@echo [DONE] Deployable zip file has been made!
	@echo
	
submit: ensure_fs clean
	@echo [SUBMIT] Creating user submission...
	cp -r ${SRCDIR}/* ${SUBMITDIR}/
	cp -r ${INCLUDE}/* ${SUBMITDIR}/
	cp -r read.me ${SUBMITDIR}/
	@echo [DONE] User submission has been made...
	@echo
	
full_clean: clean
	@echo [FULL CLEAN] Cleaning up everything...
	rm -rf ${SUBMITDIR}
	@echo [DONE] Cleaned up everything...
	@echo
