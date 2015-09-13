
all: hook.c hook.h fd_mgmt.c fd_mgmt.h
	gcc -Wall -g -o hook.i -E hook.c
	indent hook.i
	gcc -Wall -g -o fdcheck.so -fPIC -shared hook.c fd_mgmt.c -ldl

clean:
	rm -rf fdcheck.so
