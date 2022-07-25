#include "conversion.h"


/* itoa:  convert int n to char array s */
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do
	{
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

/* reverse symbols in char array*/
void reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void reverse(char s[], char ssize)
{
	int i, j;
	char c;

	for (i = 0, j = ssize - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void u16toHexString (unsigned int n, char s[]){

unsigned int a = 0xF;
unsigned char b = 0;
	
	for (unsigned char i = 0; i < 4; i++)
	{
		char temp = ((n & a) >> b);
		char symb = (temp < 10) ? temp + 48 : temp + 55;
		s[i] = symb;
		a *= 0x10;
		b += 4;
	}
	s[4] = 'x';
	s[5] = '0';
	s[6] = '\0';

	reverse(s, 6);

}

void u32toHexString (unsigned int n, char s[]){
	
unsigned int a = 0xF;
	unsigned char b = 0;

	for (unsigned char i = 0; i < 8; i++)
	{
		char temp = ((n & a) >> b);
		char symb = (temp < 10) ? temp + 48 : temp + 55;
		s[i] = symb;
		a *= 0x10;
		b += 4;
	}
	s[8] = 'x';
	s[9] = '0';
	s[10] = '\0';

	reverse(s, 10);

}
