# Makefile for libhexes
#
TOPDIR = ..

NEED_NCURSES = 1

ifdef TCAMAKE_DEBUG
OPT_FLAGS = -g
endif

OPT_FLAGS += -fPIC -O2
CCSHARED  += -Wl,-soname,$@
INCLUDES   = -Iinclude
LIBS=
LFLAGS=	

OBJS=	src/HexApp.o src/HexPanel.o src/HexWindow.o \
		src/HexString.o src/HexStringField.o \
		src/HexDialog.o src/HexStack.o \
		src/LineInputHandler.o src/LineOutputHandler.o

ALL_OBJS = $(OBJS)


include $(TOPDIR)/tcamake/tcamake_include


all: lib test

lib: arlib
arlib: lib/libhexes.a
solib: libhexes.so.0.2.1

lib/libhexes.a: $(OBJS)
	( $(MKDIR) lib )
	$(make-lib-rule)
	@echo

libhexes.so.0.2.1: $(OBJS)
	( $(MKDIR) lib )
	( $(RM) $@ lib/libhexes.so )
	$(make-so-rule)
	( mv $@ lib/; cd lib; ln -s $@ libhexes.so )
	@echo

documentation:
	(cd docs; ${MAKE} ${MFLAGS} ${MVARS} all )
	@echo

libclean:
	rm -rf lib
	@echo

doc-clean:
	( cd docs; $(MAKE) clean )
	@echo

.PHONY: test
test:
	( cd test; $(MAKE) all )
	@echo

clean:
	$(RM) $(ALL_OBJS) \
	*.d *.bd *.D *.o src/*.d src/*.D src/*.bd src/*.o lib/*.bd \
	test/*.o test/*.bd test/*.d test/*.D
	@echo
	
test-clean:
	( cd test; $(MAKE) distclean )
	@echo


distclean: clean libclean test-clean doc-clean
	$(RM) $(ALL_BINS)
	@echo

dist:
ifdef TCAMAKE_DISTDIR
	@echo "Sync'ing to $(TCAMAKE_DISTDIR)/libhexes"
	( $(RDIST) ./ $(TCAMAKE_DISTDIR)/libhexes/ )
	@echo
endif
	
install:
ifdef TCAMAKE_PREFIX
	@echo "Installing libhexes to $(TCAMAKE_PREFIX)/lib"
	$(MKDIR) $(TCAMAKE_PREFIX)/include/hexes
	$(MKDIR) $(TCAMAKE_PREFIX)/lib
	$(RSYNC) --delete include/ $(TCAMAKE_PREFIX)/include/hexes
	$(RSYNC) lib/ $(TCAMAKE_PREFIX)/lib/
	@echo	
endif

