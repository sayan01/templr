CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -O3 -std=c17 -g

all: templr

templr: templr.c
	$(CC) $(CFLAGS) templr.c -o templr

clean *.csv:
	rm -f templr
	cat *.csv | cut -d',' -f2 | xargs -I{} rm {}.txt

