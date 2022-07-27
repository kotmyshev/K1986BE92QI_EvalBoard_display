#include <string.h>

void itoa(int n, char s[]); // itoa:  convert int n to char array s
void reverse(char s[]); // reverse symbols in char array
void reverse(char s[], char ssize); // reverse ssize symbols in char array

void u16toHexStr (unsigned int n, char s[]); // convert uint16 to array s in hex style with 0x -> 0x0000
void u32toHexStr (unsigned int n, char s[]); // convert uint32 to array s in hex style with 0x -> 0x00000000

void DoubleToStr (double n, int tolerance, char s[]); // convert double to array s with tolerance (from https://prog-cpp.ru/double-to-str/)
