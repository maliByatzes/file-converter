cflags := -c -fsanitize=address -O2 -fno-omit-frame-pointer -Wall -Wextra
cflags2 := -fsanitize=address -O2 -fno-omit-frame-pointer -Wall -Wextra
objs := epub_extractor.o html_scanner.o xml_scanner.o xhtml_scanner.o pdf_extractor.o html_scanner.o fc.o

all: f

xml_scanner.o: xml_scanner.c
	clang $(cflags) xml_scanner.c

xhtml_scanner.o: xhtml_scanner.c
	clang $(cflags) xhtml_scanner.c

pdf_extractor.o: pdf_extractor.c
	clang $(cflags) pdf_extractor.c

html_scanner.o: html_scanner.c
	clang $(cflags) html_scanner.c

epub_extractor.o: epub_extractor.c
	clang $(cflags) epub_extractor.c

fc.o: fc.c
	clang $(cflags) fc.c

main.o: main.c
	clang $(cflags) main.c

f: $(objs) main.o
	clang $(cflags2) main.o $(objs) -o f

debug:
	clang -Wall -Wextra -ggdb main.c fc.c epub_extractor.c html_scanner.c pdf_extractor.c xhtml_scanner.c xml_scanner.c -o f

clean:
	rm -rf *.o f
