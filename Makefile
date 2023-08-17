
# maybe a build script would have been better but whatever.

CC=gcc
CFLAGS= -ggdb3 -I .
BINDIR= ./bin

.PHONY: all inline_hooks trampoline

all: inline_hooks trampoline

inline_hook:
	$(CC) ./$@/main.c $(CFLAGS) -o $(BINDIR)/$@
	$(BINDIR)/$@

trampoline:
	$(CC) ./$@/main.c $(CFLAGS) -o $(BINDIR)/$@
	$(BINDIR)/$@