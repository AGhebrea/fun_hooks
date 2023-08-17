
#include <util_macros.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>

#define PATCHBYTESLEN 12

unsigned char patchBytes[PATCHBYTESLEN] = {
    /* mov rax, 0xcccccccccccccccc */
    0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,

    /* jmp rax */
    0xFF, 0xE0};

unsigned char origBytes[PATCHBYTESLEN] = {0};
int ORIG_BYTES_VALID = 0;

void uninstallPatch();
void installPatch();
void updatePatch(void *);
void hookDisplayMessage();
void displayMessage(char *);

int main()
{
	TODO(
	    "Take this code out in a lib and"
	    "\ninject it into the actual target ELF file");
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
	uninstallPatch();
	displayMessage("hooked");
	installPatch();
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

void updatePatch(void *targetAddr)
{
	memcpy(&patchBytes[2], &targetAddr, sizeof(void *));
	return;
}

void installPatch()
{
	size_t addrPageStart;
	addrPageStart = (size_t)displayMessage;
	addrPageStart -= addrPageStart % 0x1000;

	mprotect((void *)addrPageStart, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);

	if (!ORIG_BYTES_VALID)
	{
		updatePatch(hookDisplayMessage);
		memcpy(&origBytes, &displayMessage, PATCHBYTESLEN);
	}
	memcpy(&displayMessage, &patchBytes, PATCHBYTESLEN);
	return;
}

void uninstallPatch()
{
	memcpy(&displayMessage, &origBytes, PATCHBYTESLEN);
	return;
}