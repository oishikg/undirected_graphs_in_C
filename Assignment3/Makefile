#  variables 

CFLAGS =-g -Wall -Werror -ansi -pedantic
OFILES = midterm_oishik.o

#  object file to output file 
graphs: $(OFILES)
	gcc $(OFILES) -o graphs $(CFLAGS)

#  source file to object file 
midterm_oishik.o:
	gcc -c midterm_oishik.c -o midterm_oishik.o $(CFLAGS)

#  remove temp obj files 
clean:
	rm $(OFILES)






