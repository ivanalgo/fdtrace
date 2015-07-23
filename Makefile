
all: hook.c
	gcc -Wall -g -o fdcheck.so -fPIC -shared hook.c fd_mgmt.c -ldl

clean:
	rm -rf fdcheck.so
