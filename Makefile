
# maybe a build script would have been better but whatever.

CCpp=g++
CC=gcc
CFLAGS= -ggdb3 -I .
CppFLAGS= -ggdb3 -I . -fpermissive -w
BINDIR= ./bin

.PHONY: all inline_hook trampoline vtab

inline_hook:
	$(CC) ./$@/main.c $(CFLAGS) -o $(BINDIR)/$@
	$(BINDIR)/$@

trampoline:
	$(CC) ./$@/main.c $(CFLAGS) -o $(BINDIR)/$@
	$(BINDIR)/$@

vtab:
	$(CCpp) ./$@/main.cpp $(CppFLAGS) -o $(BINDIR)/$@
	$(BINDIR)/$@