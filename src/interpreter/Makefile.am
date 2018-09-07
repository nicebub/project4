BUILT_SOURCES =  cg.tab.h 
bin_PROGRAMS = cg
cg_SOURCES = main.c main.h symtabcg.c symtabcg.h Listcg.c Listcg.h commandcg.c commandcg.h transcg.c transcg.h datacg.h exprcg.h typecg.h cg.l.h cg.l cg.y
cg_LDADD = -ll
AM_YFLAGS = -vt -d -b cg

#main.c : cg.tab.c main.h


#transcg.o: transcg.c transcg.h
#	$(COMPILE) -c transcg.c $(CFLAGS)

#cg.tab.c : 
#	$(YACC) $(AM_YFLAGS) cg.y

#lex.yy.c : cg.tab.c
#	$(LEX) $(AM_LFLAGS) cg.l

#symtabcg.o: symtabcg.h symtabcg.c
#	$(COMPILE) -c symtabcg.c $(CFLAGS)

#Listcg.o: Listcg.c
#	$(COMPILE) -c Listcg.c $(CFLAGS)

#commandcg.o: commandcg.c commandcg.h
#	$(COMPILE) -c commandcg.c $(CFLAGS)
