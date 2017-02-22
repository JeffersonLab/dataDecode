#
# File:
#    Makefile
#
# Description:
#    Makefile for compilation of dataDecode program
#
#
DEBUG	?= 1
QUIET	?= 1
#
ifeq ($(QUIET),1)
        Q = @
else
        Q =
endif

CROSS_COMPILE		=
CC			= $(CROSS_COMPILE)gcc
AR                      = ar
RANLIB                  = ranlib
INCS			= -I.
CFLAGS			= -Wall -g  \
			  -L. 
SRC			= $(wildcard *.c)
SRC2			= $(filter-out dataDecode.c,${SRC})
DEPS			= $(SRC2:.c=.d)
PROGS			= $(SRC2:.c=)

all: $(PROGS)

clean distclean:
	@rm -f $(PROGS) *~ $(OBJS) $(DEPS)

%: %.c
	@echo " CC     $@"
	${Q}$(CC) $(CFLAGS) $(INCS) -DDATADECODE=$(@:dec=DataDecode) -o $@ $< dataDecode.c 

%.d: %.c
	@echo " DEP    $@"
	@set -e; rm -f $@; \
	$(CC) -MM -shared $(INCS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEPS)

.PHONY: all clean distclean
