# this will compile bstsort.c files and create an executible called bstsort. It also has the option to clean the executable and object files.
all: 
	gcc bstsort.c -o bstsort
clean: 
	rm -f *.exe *.o bstsort