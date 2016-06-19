# IOOP/M Björnen

## Projektuppgift: (2016-01-13) 0aa6259

## Folders
* [src](src) 	   - Source files
* [tests](tests)   - Test files
* [obj](obj) 	   - Object files
* [bin](bin) 	   - Full (linked) binaries (including test-binaries)
* [doc](doc) 	   - Doxygen configuration & Documentation PDF:s
* [report](report) - Report PDF & Tex-files (Log books within report)

### Report
The report is located [here](report) as a PDF-document. This document is 'living' and
will be maintained until the end of the project.

## Compiling
Compiling files, tests or the entire project is done using `make` from the root
directory of the repository. For help with available make-targets, simply type
`make help` in your terminal or gaze upon the mighty list of make-targets at the
end of the README.

### Prerequisits
The GC requires **C-standard 99** or newer i order to compile.

### Tests
All binaries, including the tests, will end up in the [bin](/bin) folder. To
compile and run all tests, simply type in `make test` when standing in the
root directory of the repository.

## MAKE TARGETS
A "living" list of targets and their use will also be added below.
``` make

    make             # Compiles static GC Library (*libgc.a*) into [bin](bin)

    make help        # Displays available Make-targets

    make test        # Compiles- and runs all tests (NOTE: test, not tests)

    make list_perf   # Compiles- and runs list performance test program

    make clean       # Clean out all temporary- and binary files

    make clean_docs  # Cleans all generated documentation files (requires sudo password)

    make docs        # Clear & re-make documentation in /doc
	
    make objs        # Compiles all source files in src/ to object files in obj/.

    obj/%.o          # Compiles src/%.c to object file obj/%.o
```

## USING THE GC LIBRARY
To use the GC library you have to compile your program in one of two ways:
###Link in libgc.a from a standard path
**Include statement**
``` c
#include <libgc.h>
```
**Compile command**
```
gcc myprogram.c -lgc
```
### Link in libgc.a from a custom path:
**Include statement**
``` c
#include "libgc.h"
```
**Compile command**
```
gcc myprogram.c -I /my/custom/path -L /my/custom/path -lgc
```
* `-I` denotes custom search/include path for headers
* `-L` denotes custom path to library files
* `-lgc` specifies use of libgc.a

