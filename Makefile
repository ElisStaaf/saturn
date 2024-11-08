CC=gcc
LD=ld

PRGNAME=/usr/bin/saturn

EN_DEBUG ?= 1

MAJOR	:= 1
MINOR	:= 0
VERSION := $(MAJOR).$(MINOR)

DESTDIR ?= /usr/local/bin

WARNINGS  = -Wall -Wextra -Wdouble-promotion -Wformat -Wnull-dereference
WARNINGS += -Wmissing-include-dirs -Wswitch-enum -Wsync-nand
WARNINGS += -Wunused -Wuninitialized -Winit-self -Winvalid-memory-model
WARNINGS += -Wmaybe-uninitialized  -Wstrict-aliasing -Wstrict-overflow=4
WARNINGS += -Wstringop-overflow=2 -Wstringop-truncation 
WARNINGS += -Wsuggest-attribute=noreturn -Wsuggest-attribute=const 
WARNINGS += -Wsuggest-attribute=malloc -Wsuggest-attribute=format 
WARNINGS += -Wmissing-format-attribute -Wduplicated-branches
WARNINGS += -Wduplicated-cond -Wtautological-compare -Wtrampolines -Wfloat-equal
WARNINGS += -Wundef -Wunused-macros -Wcast-align -Wconversion
WARNINGS += -Wsizeof-pointer-div -Wsizeof-pointer-memaccess -Wstrict-prototypes
WARNINGS += -Wmissing-prototypes -Wmissing-declarations 
WARNINGS += -Wmissing-field-initializers -Wpacked -Wredundant-decls
WARNINGS += -Winline -Wdisabled-optimization -Wunsuffixed-float-constants

DEBUG=  -ggdb3 -fvar-tracking -fvar-tracking-assignments -ginline-points \
		-gstatement-frontiers # -fprofile-arcs -ftest-coverage


INC  = -I./
INC	+= -I./include

CFLAGS   = -O2
CFLAGS	+= $(INC)
ifeq ( $(EN_DEBUG), 1 )
CFLAGS += $(DEBUG)
endif

CFLAGS += $(OPTIONS)
CFLAGS += $(WARNINGS)
LDFLAGS= -lyaml

SRC_DIR	 = ./src
SRC		 = $(SRC_DIR)/parser.c
OBJ		 = $(SRC:.c=.o)
DEP		 = $(OBJ:.o=.d)


all: $(PRGNAME)

$(PRGNAME): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PRGNAME) $^

-include $(DEP)

valgrind: $(PRGNAME)
	valgrind --leak-check=yes --track-origins=yes --show-reachable=yes --log-file="valgrind.log" $(PRGNAME)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@


.PHONY: clean
clean:
	-rm -f $(OBJ)
	-rm -f $(DEP)
	-rm -f $(PRGNAME)
	-rm -f $(SRC_DIR)/*.gcda
	-rm -f $(SRC_DIR)/*.gcno
	-rm -f *.gcov
