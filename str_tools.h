#ifndef MKTL_STR_TOOLS_H
#define MKTL_STR_TOOLS_H

#include <string.h>
#include <stddef.h>

size_t chr_count(const char* str, char chr);
size_t chr_replace(char* str, char chr, char replac);
size_t str_count(const char* str, const char* substr);
size_t str_replace(char* dest, const char* src, const char* substr, const char* replac);
char* str_reverse(char* str);

#ifdef STR_TOOLS_IMPL

size_t chr_count(const char* str, char chr)
{
	if (!str || !chr)
		return 0;

	size_t count = 0;

	while ( (str = strchr(str, chr)) )
	{
		count++;
		str++;
	}

	return count;
}

size_t chr_replace(char* str, char chr, char replac)
{
	if (!str || !chr)
		return 0;

	size_t count = 0;

	while ( (str = strchr(str, chr)) )
	{
		*str = replac;
		count++;
		str++;
	}

	return count;
}

size_t str_count(const char* str, const char* substr)
{
	if (!str || !substr || !str[0] || !substr[0])
		return 0;

	size_t substr_len = strlen(substr);
	size_t count = 0;

	while ( (str = strstr(str, substr)) )
	{
		count++;
		str += substr_len;
	}

	return count;
}

size_t str_replace(char* dest, const char* src, const char* substr, const char* replac)
{
	if (!dest || !src || !substr || !replac || !src[0] || !substr[0])
		return 0;
	
	size_t substr_len = strlen(substr);
	size_t replac_len = strlen(replac);
	size_t count = 0;

	const char* ptr;
	while ( (ptr = strstr(src, substr)) )
	{
		memcpy(dest, src, ptr - src);
		dest += ptr - src;
		src += ptr - src;
		
		memcpy(dest, replac, replac_len);
		dest += replac_len;
		src += substr_len;
		
		count++;
	}
	strcpy(dest, src);
	
	return count;
}

char* str_reverse(char* str)
{
	if (!str || !str[0])
		return str;

	char* str_addr = str;
	char* str_end = str + strlen(str) - 1;
	char temp;

	while ( str < str_end )
	{
		temp = *str_end;
		*str_end-- = *str;
		*str++ = temp;
	}

	return str_addr;
}

#endif

#endif
