
all: hook.c hook.h fd_mgmt.c fd_mgmt.h
	gcc -Wall -g -o fdcheck.so -fPIC -shared hook.c fd_mgmt.c -ldl
	gcc -Wall -g -o hook.i -E hook.c

clean:
	rm -rf fdcheck.so
