SUBDIRS = cg
BUILT_SOURCES = ucc.y ucc.l 
bin_PROGRAMS = ucc
ucc_SOURCES = main.c main.h symtab.c symtab.h List.c List.h command.c command.h trans.c trans.h data.h expr.h type.h ucc.l.h ucc.l ucc.y
ucc_LDADD = -ll
AM_YFLAGS = -vt -d -b ucc

#main.c : ucc.tab.c main.h


#trans.o: trans.c trans.h
#	$(COMPILE) -c trans.c $(CFLAGS)

#ucc.tab.h : 
#	$(YACC) $(AM_YFLAGS) ucc.y

#lex.yy.h : ucc.tab.h
#	$(LEX) $(AM_LFLAGS) ucc.l

#symtab.o: symtab.h symtab.c
#	$(COMPILE) -c symtab.c $(CFLAGS)

#List.o: List.c
#	$(COMPILE) -c List.c $(CFLAGS)

#command.o: command.c command.h
#	$(COMPILE) -c command.c $(CFLAGS)
