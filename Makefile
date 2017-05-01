# Makefile for 

lib.name = pil

common.sources = embeddableMiniPicoLisp/src/apply.c \
embeddableMiniPicoLisp/src/math.c \
embeddableMiniPicoLisp/src/flow.c \
embeddableMiniPicoLisp/src/subr.c \
embeddableMiniPicoLisp/src/gc.c \
embeddableMiniPicoLisp/src/sym.c \
embeddableMiniPicoLisp/src/io.c \
embeddableMiniPicoLisp/src/tab.c \
embeddableMiniPicoLisp/src/main.c


cflags+= -IembeddableMiniPicoLisp/src/ -g

class.sources = pil.c

#datafiles = myclass1-help.pd myclass2-help.pd README.txt LICENSE.txt

PDINCLUDEDIR=/usr/local/include/pd

include Makefile.pdlibbuilder

tags:
	find . ~/git_checkouts/pure-data -name '*.[ch]' | etags -

test:
	cp pil.pd_linux ~/pd-externals/; pd ./pil.pd
