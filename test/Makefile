NEED_NCURSES = 1
NEED_HEXES = 1

ifdef TNMS_DEBUG
OPT_FLAGS= 	-g
endif

LFLAGS=	
INCLUDES=   -I. -I../include

BIN=		hextest
OBJS=		hextest.o TestHexApp.o

ALL_OBJS=	$(OBJS)
ALL_BINS=	$(BIN)


ifeq ($(TCAMAKE_HOME),)
    TCAMAKE_HOME := $(realpath ../../tcamake)
endif

include $(TCAMAKE_HOME)/tcamake_include


all: hextest


hextest: $(OBJS)
	$(make-cxxbin-rule)

clean:
	$(RM) $(ALL_OBJS) \
	*.d *.D *.o src/*.d src/*.D src/*.bd src/*.o

distclean: clean
	$(RM) $(ALL_BINS)

install:
