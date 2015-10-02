libsearch.so: src/hash.h src/hash.c src/type.h src/html.h src/html.c src/seg.c src/seg.h src/utils.h src/utils.c src/index.h src/index.c src/search.h src/search.c
	gcc -shared -o libsearch.so -fPIC src/search.c src/hash.c src/html.c src/seg.c src/utils.c src/index.c

genindex: src/utils.h src/utils.c src/index.h src/index.c src/html.h src/html.c src/genindex.c src/hash.h src/hash.c src/type.h src/seg.h src/seg.c
	gcc -o genindex src/genindex.c src/utils.c src/index.c src/html.c src/hash.c src/seg.c `pkg-config --cflags --libs gumbo` -lgumbo

cli: libsearch.so src/cli.c
	gcc src/cli.c -L$(CURDIR) -lsearch `pkg-config --cflags --libs gumbo` -lgumbo -o cli

httpd: libsearch.so src/httpd.go
	LIBRARY_PATH=$(CURDIR) go build -o httpd src/httpd.go


.DEFAULT_GOAL := all
.PHONY: all clean
all: libsearch.so cli httpd genindex
clean:
	rm libsearch.so cli httpd genindex
