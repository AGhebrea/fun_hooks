
#include <iostream>
#include <sys/mman.h>
#include <models.h>
#include <util_macros.h>

void intruder()
{
	std::cout << "1337" << std::endl;

	return;
}

int main(int argc, char **argv)
{
	fun_ptr_t funPtr;

	BaseClass *base = new BaseClass();
	BaseClass *derived = new DerivedClass();

	TODO(
		"This is jank in the sense that we don't"
		"\nalways find the pointers to member"
		"\nfunctions. Fix that."
	);

	std::cout << "Before injection:" << std::endl;
	std::cout << "[BaseClass] -> "; base->Hello();
	std::cout << "[DerivedClass] -> "; derived->Hello(); std::cout << std::endl;

	auto **vtabPtr = reinterpret_cast<void**>(*reinterpret_cast<void**>(derived));

	printf("\n[0x%x] vtabPtr[0]", reinterpret_cast<size_t>(*vtabPtr));
	funPtr = reinterpret_cast<void*>(&(derived->Hello));
	// printf("\n[0x%x] derived->Hello", funPtr);	
	// funPtr = reinterpret_cast<void*>(&(derived->Name));
	// printf("\n[0x%x] derived->Name", funPtr);
	// funPtr = reinterpret_cast<void*>(&(derived->Order));
	// printf("\n[0x%x] derived->Order (searching)", funPtr);

	for(int i = 0; i < 4; ++i){
		void *vtabEntryPtr{vtabPtr[i]};
		printf("\n[0x%x]", reinterpret_cast<size_t>(vtabEntryPtr));
		if(reinterpret_cast<size_t>(vtabEntryPtr) == reinterpret_cast<size_t>(funPtr)){
			std::cout << " Found a target" << std::endl;
			mprotect(((size_t)vtabPtr - ((size_t)vtabPtr % 0x1000)), 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);
			vtabPtr[i] = intruder;
		}
	}

	std::cout << std::endl << "Calling derived->Hello : ";
	derived->Hello();
	std::cout << std::endl;


	return 0;
}