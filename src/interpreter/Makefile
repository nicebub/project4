# Used for project1 by Scott Lorberbaum
#
# This makefile includes an entry "depend" which will automatically
# generate complete include file dependencies (including nested
# includes.  To update dependencies, simply update the SRCS variable
# (and OBJS) and type "make depend"
#
TARGETDIR = .
#
# Use gcc for Motif compilations.
CC = /usr/local/bin/gcc-8
#CC = /usr/bin/gcc
BISON = /usr/local/opt/bison/bin/bison
#BISON = /usr/bin/bison
FLEX = /usr/local/opt/flex/bin/flex
#FLEX = /usr/bin/flex



MYCG = "./CG"


#CFLAGS = -g  -O0 -ggdb -I/usr/local/opt/bison/include
CFLAGS = -g  -O0 -ggdb -D DEBUGON -I/usr/local/opt/bison/include

#CFLAGS = -g -O0 -D DEBUGON
#CFLAGS = -g -O0 -D DEBUGOFF



#LDFLAGS =  -L/usr/local/opt/bison/lib -lc -L/usr/local/opt/flex/lib
#LDFLAGS =  -g -L/usr/local/opt/bison/lib -lc 

FLOP = -v -p -p -B
BISOP = -vtd
#LIBS = -lfl -lm
#LIBS = -ll -lm

all: cg memlib

memlib: memlib_main.c memlib.c memlib.h debuglib.o Listcg.o
	$(CC) -o memlib memlib.c memlib_main.c debuglib.o Listcg.o $(CFLAGS)

run:
#	exec "for FILE in `ls *.asm`; do ./cg $(FILE); done"
	for FILE in `ls *.asm` ; do ./cg $(FILE); done

main.c : cg.tab.c main.h


transcg.o: transcg.c transcg.h
	$(CC) -c transcg.c $(CFLAGS)

cg.tab.c : cg.y
	$(BISON) $(BISOP) cg.y

lex.yy.c : cg.l cg.tab.c
	$(FLEX) $(FLOP) cg.l

#symtabcg.o: symtabcg.h symtabcg.c
#	$(CC) -c symtabcg.c $(CFLAGS)

Listcg.o: Listcg.c
	$(CC) -c Listcg.c $(CFLAGS)

commandcg.o: commandcg.c commandcg.h
	$(CC) -c commandcg.c $(CFLAGS)

debuglib.o: debuglib.c debuglib.h
	$(CC) -c debuglib.c $(CFLAGS)

memlib.o: memlib.c memlib.h
	$(CC) -c memlib.c $(CFLAGS)

MAIN = main.c

SRCS  = lex.yy.c \
	cg.tab.c \
	main.c \
	Listcg.c  \
	commandcg.c \
	transcg.c \
	debuglib.c \
	memlib.c

OBJS  = lex.yy.o  \
	cg.tab.o \
	Listcg.o \
	commandcg.o \
	transcg.o \
	debuglib.o \
	memlib.o


sample$(NUM) : sample$(NUM).c
	$(MYUCC) sample$(NUM).c

clean :
	rm *.o lex.yy.c cg.tab.* cg.output cg example core test *.out *.s; rm -rf cg.dSYM debug.output memlib memlib.dSYM;


cg : $(OBJS) $(MAIN)
	$(CC) $(LDFLAGS) -o $(TARGETDIR)/$@ $(OBJS) $(MAIN) $(LIBS) $(CFLAGS)

example: $(OBJS) $(MAIN) symtab.o example.c List.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGETDIR)/$@ $(OBJS) example.c $(LIBS)


#	if (/bin/test -w $@) then (rm $@) else (:) fi
#
# make depend
#
# The depend switch varies (unfortunately) for different C compilers.
#        -Em is used by DEC,
#	 -M by Sun
#DEPEND = -Em
#DEPEND = -M
DEPEND = -MM
depend:
	sed -e "/^#=====DO NOT DELETE THIS LINE=====/q" Makefile.old > Makefile.new
	$(CC) $(CFLAGS) $(DEPEND) $(SRCS) >> Makefile.new
#	gcc $(CFLAGS) $(DEPEND) $(SRCS) >> Makefile.new
	mv Makefile.new Makefile.old

#=====DO NOT DELETE THIS LINE=====
lex.yy.o: lex.yy.c debuglib.h typecg.h Listcg.h exprcg.h symtabcg.h \
  cg.l.h main.h datacg.h transcg.h cg.tab.h
cg.tab.o: cg.tab.c debuglib.h typecg.h Listcg.h exprcg.h datacg.h \
  symtabcg.h cg.l.h main.h transcg.h
main.o: main.c debuglib.h typecg.h Listcg.h exprcg.h datacg.h symtabcg.h \
  cg.l.h main.h transcg.h cg.tab.h commandcg.h
Listcg.o: Listcg.c debuglib.h typecg.h Listcg.h exprcg.h cg.l.h
commandcg.o: commandcg.c commandcg.h
transcg.o: transcg.c transcg.h cg.tab.h
debuglib.o: debuglib.c debuglib.h typecg.h Listcg.h exprcg.h
memlib.o: memlib.c memlib.h typecg.h Listcg.h exprcg.h debuglib.h
