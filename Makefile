
all: hook.c hook.h fcntl.c ioctl.c fd_mgmt.c fd_mgmt.h
	gcc -Wall -g -o hook.i -E hook.c
	indent hook.i
	gcc -Wall -g -o fcntl.i -E fcntl.c
	indent fcntl.i
	gcc -Wall -g -o ioctl.i -E ioctl.c
	indent ioctl.i
	gcc -Wall -g -o fdcheck.so -fPIC -shared hook.c fcntl.c ioctl.c fd_mgmt.c -ldl

clean:
	rm -rf fdcheck.so
