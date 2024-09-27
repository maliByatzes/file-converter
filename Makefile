PHONY: build debug

build:
	@clang -fsanitize=address -O1 -fno-omit-frame-pointer -Wall -Wextra main.c fc.c epub_extractor.c html_parser.c pdf_extractor.c xhtml_parser.c xml_parser.c -o f

debug:
	@clang -Wall -Wextra -ggdb main.c fc.c epub_extractor.c html_parser.c pdf_extractor.c xhtml_parser.c xml_parser.c -o f
