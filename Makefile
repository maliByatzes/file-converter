PHONY: build

build:
	@gcc -Wall -Wextra -ggdb main.c fc.c epub_extractor.c html_parser.c pdf_extractor.c xhtml_parser.c xml_parser.c -o file-converter
