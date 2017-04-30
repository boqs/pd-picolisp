# Makefile for 

lib.name = counter

common.sources = embeddableMiniPicoLisp/src/apply.c \
embeddableMiniPicoLisp/src/math.c \
embeddableMiniPicoLisp/src/flow.c \
embeddableMiniPicoLisp/src/subr.c \
embeddableMiniPicoLisp/src/gc.c \
embeddableMiniPicoLisp/src/sym.c \
embeddableMiniPicoLisp/src/io.c \
embeddableMiniPicoLisp/src/tab.c \
embeddableMiniPicoLisp/src/main.c


cflags+= -IembeddableMiniPicoLisp/src/

class.sources = counter.c

#datafiles = myclass1-help.pd myclass2-help.pd README.txt LICENSE.txt

PDINCLUDEDIR=/usr/local/include/pd

include ../pd-lib-builder/Makefile.pdlibbuilder

# snosh :
# 	echo $(common.sources)

tags:
	find . ~/git_checkouts/pure-data -name '*.[ch]' | etags -

test:
	cp counter.pd_linux ~/pd-externals/; pd ./counter.pd
