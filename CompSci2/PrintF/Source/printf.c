#define __USING_C_
#include "printf.h"

char* convert(unsigned long long val, int base);

void cprintf(const char* format, ...) {
	va_list arg;
	va_start(arg,format);
	
	const char* traverse;
	const char* s;
	
	unsigned long int i;
	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while(*traverse != '\0'){
			if(*traverse == '%') break;
			putchar(*traverse);
			traverse++;
		}
		
		if(*traverse == '\0') break;
		
		traverse++;
		
		if(*traverse == 'l'){
			traverse++;
			if(*traverse == 'l'){
				traverse++;
				switch(*traverse)
				{
					case 'c': {
							putchar(va_arg(arg,int));
						}
						break;
				
					case 's': {
							s = va_arg(arg,char*);
							puts(s);
						}
						break;
						
					case 'u': {
							i = va_arg(arg,unsigned long long);
							puts(convert(i,10));
						}
						break;
						
					case 'i':
					case 'd': {
							i = va_arg(arg,unsigned long long);
							if((int)i < 0){
								i = -i;
								putchar('-');
							}
							puts(convert(i,10));
						}
						break;
						
					case 'x': {
							i = va_arg(arg,unsigned long long);
							puts(convert(i,16));
						}
						break;
				}
			}else{
				switch(*traverse)
				{
					case 'c': {
							putchar(va_arg(arg,int));
						}
						break;
				
					case 's': {
							s = va_arg(arg,char*);
							puts(s);
						}
						break;
						
					case 'u': {
							i = va_arg(arg,unsigned long int);
							puts(convert(i,10));
						}
						break;
						
					case 'i':
					case 'd': {
							i = va_arg(arg,unsigned long int);
							if((int)i < 0){
								i = -i;
								putchar('-');
							}
							puts(convert(i,10));
						}
						break;
						
					case 'x': {
							i = va_arg(arg,unsigned long int);
							puts(convert(i,16));
						}
						break;
				}
			}
		}else{
			switch(*traverse)
			{
				case 'c': {
						putchar(va_arg(arg,int));
					}
					break;
			
				case 's': {
						s = va_arg(arg,char*);
						puts(s);
					}
					break;
					
				case 'u': {
						i = va_arg(arg,unsigned int);
						puts(convert(i,10));
					}
					break;
					
				case 'i':
				case 'd': {
						i = va_arg(arg,int);
						if((int)i < 0){
							i = -i;
							putchar('-');
						}
						puts(convert(i,10));
					}
					break;
					
				case 'x': {
						i = va_arg(arg,unsigned int);
						puts(convert(i,16));
					}
					break;
			}
		}
		
		if(*traverse == '\0') break;
	}
	va_end(arg);
}

static char buffer[256] = {0};

char* convert(unsigned long long val, int base) {
	static const char numbers[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	char *b = &(buffer[255]);
	*b = 0;
	
	while(val > 0){
		b--;
		*b = numbers[val % base];
		val /= base;
	}
	
	return b;
}

int StrToInteger(const char* str) {
	int i = 0;
	int value = 0;
	
	if(*str == '-') i = 1;
	
	for(; str[i] != '\0'; i++) {
		value *= 10;
		//if(!(str[i] >= '0' && str[i] <= '9')) return -1;
		value += str[i] - '0';
	}
	
	if(*str == '-')
		value = -value;
	return value;
}
