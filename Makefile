
all: libmmi_dnr.so wd.out wapp.out

WD_LIB_SRC = w_mmi_dnr.c ../../utils/Sched.c ../../utils/PQueue.c ../../utils/SortedL.c ../../utils/UID.c ../../utils/Task.c ../../utils/DList.c

libmmi_dnr.so: $(WD_LIB_SRC)
	gcc -ansi -pedantic-errors -Wall -g -I../../utils -fPIC -shared $(WD_LIB_SRC) -o libmmi_dnr.so -lpthread

wd.out: wd.c
	gcc -ansi -pedantic-errors -Wall -g -I../../utils wd.c -o wd.out -L. -lmmi_dnr -Wl,-rpath=.

wapp.out: wapp.c
	gcc -ansi -pedantic-errors -Wall -g -I../../utils wapp.c -o wapp.out -L. -lmmi_dnr -Wl,-rpath=.

clean:
	rm -f libmmi_dnr.so wd.out wapp.out
