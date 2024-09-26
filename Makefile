PHONY: build

build:
	@clang -fsanitize=address -O1 -fno-omit-frame-pointer -Wall -Wextra -ggdb main.c fc.c epub_extractor.c html_parser.c pdf_extractor.c xhtml_parser.c xml_parser.c -o file-converter
