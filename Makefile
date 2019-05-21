
dirSrc:=src
dirOut:=build
dirTests:=tests
#dirTestsPerf:=tests_performance [check] add folder for performance check

# File Makefile.cfg must be written to fit the user's sytem
# One can use Makefile.cfg.template as a template.
CCFLAGS:=
LKFLAGS:=
MACROS:=
MACROS +=-D 'UNUSED(x)=x __attribute__((__unused__))' #[check] remove -D from syntax

# IMPORT DATA FROM Makefile.cfg
include Makefile.cfg
CCFLAGS+=$(MACROS)
CCFLAGS+=--std=c++11
INCL_ALL := -I $(dirSrc) $(INCL_HDF5) $(INCL_GTEST)
LINK_ALL := $(LINK_HDF5) $(LINK_GTEST)

# FIND SOURCE FILES
fileSrc:=$(shell find $(dirSrc) -iname "*.cpp" ! -iname "main.cpp")
fileObj:=$(subst .cpp,.o,$(fileSrc))
fileObj:=$(subst src/,build/,$(fileObj))

fileTestsSrc:=$(shell find $(dirTests) -iname "*.cpp" ! -iname "main.cpp")
fileTestsObj:=$(subst .cpp,.o,$(fileTestsSrc))
fileTestsObj:=$(subst tests/,build/tests/,$(fileTestsObj))

.PHONY: compilationMessage

# BUILD SOURCE FILES

all: compilationMessage build/main.o $(fileObj)
	@printf "Linking into $(dirOut)/prgm.run...\n"
	@$(CC) $(LKFLAGS) "build/main.o" $(fileObj) $(LINK_ALL) -o "$(dirOut)/prgm.run"
	@printf "Done!\n"

build/%.o:src/%.cpp
	@printf "Compiling $<...\n"
	@#Make output directory if it doesn't exist
	@outDir=$$(dirname "$@") ; if [ ! -d $$outDir ] ; then mkdir --parents $$outDir ; fi
	@$(CC) -c $(CCFLAGS) $(INCL_ALL) $< -o $@


# BUILD UNIT TESTS

tests: compilationMessage build/tests/main.o $(fileObj) $(fileTestsObj)
	@printf "Linking into $(dirOut)/prgm.run...\n"
	@$(CC) $(LKFLAGS) "build/tests/main.o" $(fileObj) $(fileTestsObj) $(LINK_ALL) -o "$(dirOut)/tests/prgm.run"
	@printf "Running tests...\n"
	./$(dirOut)/tests/prgm.run
	@printf "Done!\n"

build/tests/%.o:tests/%.cpp
	@printf "Compiling $<...\n"
	@#Make output directory if it doesn't exist
	@outDir=$$(dirname "$@") ; if [ ! -d $$outDir ] ; then mkdir --parents $$outDir ; fi
	@$(CC) -c $(CCFLAGS) $(INCL_ALL) $< -o $@


# OTHER

clean:
	@printf "Deleting output directory $(dirOut)/...\n"
	@rm -rf "$(dirOut)/"

compilationMessage:
	@printf "Compilation will use $(CC) $(CCFLAGS)\n"

