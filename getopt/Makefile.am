CC = $(REAL_CC)
noinst_LIBRARIES = libgetopt.a
libgetopt_a_SOURCES = getopt.c getopt1.c

getopt.o: getopt.c
	$(CC) -o $@ -c $<
	cp $@ ../src
getopt1.o: getopt1.c
	$(CC) -o $@ -c $<
	cp $@ ../src

