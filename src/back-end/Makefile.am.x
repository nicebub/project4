#BUILT_SOURCES =  cg.tab.h 


noinst_LIBRARIES = libcg.a


libcg_a_SOURCES = cg.l cg.y cg.tab.h cg.tab.c symtabcg.c symtabcg.h \
	 			Listcg.h Listcg.c commandcg.h commandcg.c \
				transcg.h transcg.c datacg.h exprcg.h typecg.h \
				cg.l.h



bin_PROGRAMS = cg


cg_SOURCES = main.c main.h


cg_DEPENDENCIES = libcg.a


LIBS = libcg.a


#cg_LDADD = -ll


AM_LFLAGS = -olex.yy.c


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
