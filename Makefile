.RECIPEPREFIX +=
.PHONY: $(filter-out compile, $(MAKECMDGOALS))
.DEFAULT_GOAL := build

NAME          = uamqpphpbinding
INI_DIR       = /etc/php/7.2/mods-available/
EXTENSION_DIR = $(shell php-config --extension-dir)
EXTENSION     = ${NAME}.so
INI           = ${NAME}.ini

COMPILER            = g++
LINKER              = g++
COMPILER_FLAGS      = -Wall -c -O2 -std=c++11 -fpic -o
LINKER_FLAGS        = -shared
LINKER_DEPENDENCIES = -lphpcpp -luamqp -laziotsharedutil

RM =   rm -f
CP =   cp -f

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

# =-=-=
# Tasks
# =-=-=

compile: ${OBJECTS} ${EXTENSION} ## compile extension where u're running make

${EXTENSION}: ${OBJECTS}
              ${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
    ${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

help: ## shows help
    @echo "\033[33mUsage:\033[0m\n  make [target] [arg=\"val\"...]\n\n\033[33mTargets:\033[0m"
    @grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[32m%-15s\033[0m %s\n", $$1, $$2}'

image: ## build docker image with everything we need to compile this extension
    @docker build -t ${NAME} .

build: ## compiles the extension in the builded docker image
    @docker run --rm -v ${CURDIR}:${CURDIR} -w ${CURDIR} ${NAME} sh -c 'make clean && make compile'

run-examples: ## run image
    @docker-compose up -d
    @docker exec -it ${NAME} bash

install: ## install extension ini and so files in the proper locations
    ${CP} ${EXTENSION} ${EXTENSION_DIR}
    ${CP} ${INI} ${INI_DIR}

clean: ## remove object files
    ${RM} ${EXTENSION} ${OBJECTS}
