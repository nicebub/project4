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
#CC = /usr/local/bin/gcc 
CC = /usr/bin/gcc
#BISON = /usr/local/bin/bison
BISON = /usr/bin/bison
#FLEX = /usr/local/bin/flex
FLEX = /usr/bin/flex

MYUCC = "./ucc"


CFLAGS = -g  -D DEBUGON

#CFLAGS = -g 



LDFLAGS =  -g

FLOP =
BISOP = -vtd
LIBS = -lfl -lm

all: ucc


example: symtab.o example.c List.o 
	gcc -o example example.c symtab.o List.o $(CFLAGS)


main.c : ucc.tab.c main.h
	

trans.o: trans.c trans.h
	gcc -c trans.c $(CFLAGS)

ucc.tab.c : ucc.y
	$(BISON) $(BISOP) ucc.y

lex.yy.c : ucc.l ucc.tab.c
	$(FLEX) $(FLOP) ucc.l

symtab.o: symtab.h symtab.c
	gcc -c symtab.c $(CFLAGS)

List.o: List.c
	gcc -c List.c $(CFLAGS)

command.o: command.c command.h
	gcc -c command.c $(CFLAGS)

MAIN = main.c

SRCS  = lex.yy.c \
	ucc.tab.c \
	main.c \
	symtab.c \
	List.c  \
	command.c \
	trans.c

OBJS  = lex.yy.o  \
	ucc.tab.o \
	symtab.o \
	List.o \
	command.o \
	trans.o


sample$(NUM) : sample$(NUM).c
	$(MYUCC) sample$(NUM).c

clean :
	rm *.o lex.yy.c ucc.tab.* ucc.output ucc example core test *.out *.asm


ucc : $(OBJS) $(MAIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGETDIR)/$@ $(OBJS) $(MAIN) $(LIBS)

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
	sed -e "/^#=====DO NOT DELETE THIS LINE=====/q" Makefile > Makefile.new
	gcc $(CFLAGS) $(DEPEND) $(SRCS) >> Makefile.new
	mv Makefile.new Makefile

#=====DO NOT DELETE THIS LINE=====
lex.yy.o: lex.yy.c List.h type.h expr.h symtab.h data.h trans.h ucc.tab.h \
  ucc.l.h
ucc.tab.o: ucc.tab.c List.h type.h expr.h symtab.h data.h trans.h
main.o: main.c data.h symtab.h List.h type.h expr.h trans.h ucc.tab.h \
  command.h main.h
symtab.o: symtab.c symtab.h List.h type.h expr.h data.h trans.h
List.o: List.c List.h type.h expr.h
command.o: command.c command.h
trans.o: trans.c trans.h
