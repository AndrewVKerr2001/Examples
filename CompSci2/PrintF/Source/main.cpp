#include <iostream>
#include "printf.h"

int main(int argc, char *argv[]){
	(void)(argc);
	cprintf("This is a test: %s\nChar: %c\nS Integer: %d\nU Integer: %u\nHex: 0x%x\nL Integer: %llx\n", argv[0], 'A', -120, -120, 120, (unsigned long long)(~0));
	
	std::cout << StrToInteger("-1080") << std::endl;
	return 0;
}
