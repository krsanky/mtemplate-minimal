CFLAGS+= -W -Wall -O2 -std=c99 -g
CFLAGS+= -I mtemplate
LDFLAGS+= -L mtemplate

all: tmpl

tmpl: main.c mtemplate/libmtemplate.a
	$(CC) $(CFLAGS) -o $@ main.c $(LDFLAGS) -lmtemplate

clean:
	rm -f tmpl

.PHONY: clean 

