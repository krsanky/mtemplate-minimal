CFLAGS+= -W -Wall -O2 -std=c99 -g
CFLAGS+= -I/usr/local/include
LDFLAGS+= -L/usr/local/lib
LDFLAGS+= -ljson-c

all: tmpl

tmpl: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${.TARGET} main.c

clean:
	rm -f tmpl

.PHONY: clean 

