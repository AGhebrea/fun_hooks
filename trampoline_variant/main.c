
/*
	This is almost the same to the inline_hook but it is faster
	since we're not constantly rewriting memory addresses.

	The fact that you are clobbering the EAX register here does
	not matter.
*/

#include <util_macros.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>

#define PATCHBYTESLEN 12

unsigned char patchBytes[12 * PATCHBYTESLEN] = {
    /* mov rax, 0xcccccccccccccccc */
    0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,

    /* jmp rax */
    0xFF, 0xE0};

unsigned char trampBytes[2 * PATCHBYTESLEN] = {0};

fun_ptr_t trampFunPtr;

void installPatch();
void updatePatch(unsigned char *, void *, size_t);
void hookDisplayMessage();
void displayMessage(char *);

int main()
{
	TODO(
	    "Take this code out in a lib and"
	    "\ninject it into the actual target ELF file"
	    "\n"
	    "\nThere is also a weird timing thing. Does"
	    "\nit persist if we write() to stdout?");
	installPatch();
	while (true)
	{
		displayMessage("SAMPLE TEXT");
		sleep(2);
	}
	return 0;
}

void hookDisplayMessage()
{
	printf("\nDo whatever. . .");
	trampFunPtr("\nhooked");
	return;
}

void displayMessage(char *message)
{
	char *timestr;
	time_t t = time(NULL);
	timestr = ctime(&t);
	printf("\n%s %.*s", message, strlen(timestr) - 1, timestr);
	return;
}

void updatePatch(unsigned char *dest, void *targetAddr, size_t poz)
{
	memcpy(&dest[poz + 2 * sizeof(unsigned char)], &targetAddr, sizeof(void *));
	return;
}

void installPatch()
{
	size_t pageStart;
	trampFunPtr = (fun_ptr_t)trampBytes;

	unsigned char trampoline[12] = {
	    /* same as hook */
	    0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
	    0xFF, 0xE0};

	pageStart = (size_t)displayMessage;
	pageStart -= pageStart % 0x1000;
	mprotect((void *)pageStart, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);

	pageStart = (size_t)trampBytes;
	pageStart -= pageStart % 0x1000;
	mprotect((void *)pageStart, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);

	memcpy(&trampBytes, &displayMessage, PATCHBYTESLEN);
	memcpy(&(trampBytes[PATCHBYTESLEN]), &trampoline, PATCHBYTESLEN);
	updatePatch(trampBytes, displayMessage + PATCHBYTESLEN, 12);

	updatePatch(patchBytes, hookDisplayMessage, 0);
	memcpy(&displayMessage, &patchBytes, PATCHBYTESLEN);

	return;
}