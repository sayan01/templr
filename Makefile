CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -O3 -std=c17 -g

all: templr

templr: templr.c
	$(CC) $(CFLAGS) templr.c -o templr

clean *.csv:
	rm -f templr
	ls *.txt | grep -v template.txt | xargs -I{} rm -f {}

compress: *
	rm templr.tar.gz
	tar -czvf templr.tar.gz *

