.RECIPEPREFIX +=
.PHONY: $(filter-out compile, $(MAKECMDGOALS))

NAME          = uamqpphpbinding
INI_DIR       = $(shell php -i | grep -i "additional .ini files" | head -n 1 | cut -f2 -d'>' | xargs)
EXTENSION_DIR = $(shell php-config --extension-dir)
EXTENSION     = ${NAME}.so
INI           = ${NAME}.ini

COMPILER            = g++
LINKER              = g++
COMPILER_FLAGS      = -Wall -c -O2 -std=c++11 -fpic -o
LINKER_FLAGS        = -shared
LINKER_DEPENDENCIES = -lphpcpp -luamqp -laziotsharedutil -luuid

RM =   rm -f
CP =   cp -f

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

# =-=-=
# Tasks
# =-=-=

all: ${OBJECTS} ${EXTENSION} ## compile extension where u're running make

${EXTENSION}: ${OBJECTS}
              ${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
    ${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

help: ## shows help
    @echo "\033[33mUsage:\033[0m\n  make [target] [arg=\"val\"...]\n\n\033[33mTargets:\033[0m"
    @grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[32m%-15s\033[0m %s\n", $$1, $$2}'

install: ## install extension ini and so files in the proper locations
    ${CP} ${EXTENSION} ${EXTENSION_DIR}
    ${CP} ${INI} ${INI_DIR}

clean: ## remove object files
    ${RM} ${EXTENSION} ${OBJECTS}
