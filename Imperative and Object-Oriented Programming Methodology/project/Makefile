# Flags
CC        :=  gcc
DEBUG     :=  -ggdb
WARNINGS  :=  -Wall #-Wextra
TEST	  :=  -lcunit

CFLAGS    += $(DEBUG) $(WARNINGS) -std=c99

# Directories
SRCDIR   :=  src
OBJDIR   :=  obj
BINDIR   :=  bin
TESTDIR  :=  tests

# File list (NO EXTENSION)
_FILES    := list h_init object gc

## Add paths and suffixes
FILES     := $(patsubst %,$(SRCDIR)/%,$(addsuffix .c, $(_FILES)))
OBJFILES  := $(patsubst %,$(OBJDIR)/%,$(addsuffix .o, $(_FILES)))

OUT	  := libgc

###################################################################
# OS Detection in Makefile, taken from stackoverflow:
# http://stackoverflow.com/questions/714100/os-detecting-makefile
#
# Modded to work with target systems
###################################################################
ifeq ($(OS),Windows_NT)
    # NOT SUPPORTED
else
    # Get OS
    UNAME_S := $(shell uname -s)
    # Get architecture
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_S),Linux)
        ifeq ($(UNAME_P),x86_64)
            CFLAGS += -D LINUX_64
        endif
        ifeq ($(UNAME_P),sparc)
            CFLAGS += -D LINUX_SPARC
        endif
        ifneq ($(filter %86,$(UNAME_P)),)
            CFLAGS += -D LINUX_32
        endif
    endif
    ifeq ($(UNAME_S), SunOS)
        ifeq ($(UNAME_P),sparc)
            CFLAGS += -D SPARC
        endif
        ifneq ($(filter %86,$(UNAME_P)),)
            CFLAGS += -D SOLARIS_32
        endif
    endif
    ifeq ($(UNAME_S),Darwin) # OS X
        CFLAGS += -D LINUX_64
    endif
endif

# Text formatting for Linux & OSX
# skip on SunOS
ifeq ($(UNAME_S), SunOS)
    TEXT_RED     := 
    TEXT_GREEN   := 
    TEXT_YELLOW  := 
    TEXT_BLUE    := 
    TEXT_BOLD    := 
    TEXT_RESET   := 
else
    TEXT_RED     := $$(tput setaf 1)
    TEXT_GREEN   := $$(tput setaf 2)
    TEXT_YELLOW  := $$(tput setaf 3)
    TEXT_BLUE    := $$(tput setaf 4)
    TEXT_BOLD    := $$(tput bold)
    TEXT_RESET   := $$(tput sgr0)
endif

# Compilation to library
all: lib

lib: objs
	@echo "Creating $(TEXT_BOLD)$(BINDIR)/$(OUT).a$(TEXT_RESET)"
	@ar rcs $(BINDIR)/$(OUT).a $(OBJFILES)
	@cp $(SRCDIR)/gc.h $(BINDIR)/$(OUT).h
	@echo "$(TEXT_GREEN)OK$(TEXT_RESET)"

# OBJECT COMPILATION
objs: $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Compiling $(TEXT_BOLD)$@$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "$(TEXT_GREEN)OK$(TEXT_RESET)"

# PERFORANCE PROGRAMS
list_perf: prog/list_perf.c lib
	@echo "Compiling $(TEXT_BOLD)$@$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $< -I $(BINDIR) -L $(BINDIR) -lgc 
	@echo "Running $@..."
	@$(BINDIR)/$@


# TEST
test: test_h_init test_is_mem_addr test_page test_object_internal test_object test_gc

test_h_init: $(TESTDIR)/test_h_init.c $(SRCDIR)/h_init.c $(SRCDIR)/h_init.h
	@echo "$(TEXT_BOLD)h_init tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $< $(TEST)
	@$(BINDIR)/$@

test_is_mem_addr: $(TESTDIR)/test_is_mem_addr.c $(SRCDIR)/h_init.c $(SRCDIR)/h_init.h
	@echo "$(TEXT_BOLD)is_mem_addr tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $< $(TEST)
	@$(BINDIR)/$@

test_page: $(TESTDIR)/test_page.c $(SRCDIR)/h_init.c $(SRCDIR)/h_init.h
	@echo "$(TEXT_BOLD)Page tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $< $(TEST)
	@$(BINDIR)/$@

test_object_internal: $(TESTDIR)/test_object_internal.c $(OBJDIR)/h_init.o
	@echo "$(TEXT_BOLD)Internal object tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@  $(TEST)
	@$(BINDIR)/$@

test_object: $(TESTDIR)/test_object.c $(OBJDIR)/h_init.o $(OBJDIR)/object.o
	@echo "$(TEXT_BOLD)Object interface tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@  $(TEST)
	@$(BINDIR)/$@

test_gc: $(TESTDIR)/test_gc.c $(OBJDIR)/list.o $(OBJDIR)/object.o
	@echo "$(TEXT_BOLD)GC tests$(TEXT_RESET)"
	@$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@ $(TEST)
	@$(BINDIR)/$@

# DOCUMENTATION
docs:
	@find doc -maxdepth 1 -not -name '*.md' -not -name '*conf' -not -name 'doc' | xargs sudo rm -rf
	@sudo doxygen doc/public_docs_conf
	@sudo doxygen doc/internal_docs_conf
	@cd doc/internal/latex && sudo make pdf && cp refman.pdf ../../internal_docs.pdf && cd ../../../
	@cd doc/PUBLIC/latex && sudo make pdf && cp refman.pdf ../../public_docs.pdf && cd ../../../

# Cleaning rules
clean_docs:
	@find doc -maxdepth 1 -not -name '*.md' -not -name '*conf' -not -name 'doc' | xargs sudo rm -rf

clean:
	@rm -rf obj/*.o
	@find bin -type f -not -name '*.md' | xargs rm -rf

# Help text 
help:
	@echo ""
	@echo "    Available make-commands are:\n"
	@echo "    $(TEXT_BOLD)make$(TEXT_RESET)"
	@echo "        Compiles static GC Library ($(TEXT_GREEN)$(OUT).a$(TEXT_RESET)) to ./$(BINDIR)."
	@echo ""
	@echo "    $(TEXT_BOLD)make help$(TEXT_RESET)"
	@echo "        Prints this help text."
	@echo ""
	@echo "    $(TEXT_BOLD)make test$(TEXT_RESET)"
	@echo "        Compiles- and runs all tests."
	@echo "        $(TEXT_RED)NOTE:$(TEXT_RESET) $(TEXT_GREEN)test$(TEXT_RESET), not tests"
	@echo ""
	@echo "    $(TEXT_BOLD)make list_perf$(TEXT_RESET)"
	@echo "        Compiles- and runs list performance test program."
	@echo ""
	@echo "    $(TEXT_BOLD)make clean$(TEXT_RESET)"
	@echo "        Cleans out all binaries and temporary files."
	@echo ""
	@echo "    $(TEXT_BOLD)make clean_docs$(TEXT_RESET)"
	@echo "        Cleans out all generated documentation."
	@echo ""
	@echo "    $(TEXT_BOLD)make docs$(TEXT_RESET)"
	@echo "        Clear & re-make documentation in /doc"
	@echo ""
	@echo "    $(TEXT_BOLD)make objs$(TEXT_RESET)"
	@echo "        Compiles all source files in /src to object files in /obj."
	@echo ""
	@echo "    $(TEXT_BOLD)make $(OBJDIR)/%.o$(TEXT_RESET)"
	@echo "        Compiles $(SRCDIR)/%.c to object file $(OBJDIR)/%.o"
	@echo ""
