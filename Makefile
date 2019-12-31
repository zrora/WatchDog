
all: libmmi_dnr.so wd.out wapp.out

WD_LIB_SRC = w_mmi_dnr.c Sched.c PQueue.c SortedL.c UID.c Task.c DList.c

libmmi_dnr.so: $(WD_LIB_SRC)
	gcc -ansi -pedantic-errors -Wall -g  -I ./ -fPIC -shared $(WD_LIB_SRC) -o libmmi_dnr.so -lpthread

wd.out: wd.c
	gcc -ansi -pedantic-errors -Wall -g  -I ./ wd.c -o wd.out -L. -lmmi_dnr -Wl,-rpath=.

wapp.out: wapp.c
	gcc -ansi -pedantic-errors -Wall -g -I ./ wapp.c -o wapp.out -L. -lmmi_dnr -Wl,-rpath=.

clean:
	rm -f libmmi_dnr.so wd.out wapp.out
