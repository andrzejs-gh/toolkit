#ifndef MKTL_STR_TOOLS_H
#define MKTL_STR_TOOLS_H

#include <string.h>
#include <stddef.h>

size_t chr_replace(char* str, char chr, char replac);
size_t str_replace(char* dest, char* src, char* substr, char* replac);
char* str_reverse(char* str);

#ifdef STR_TOOLS_IMPL

size_t chr_count(char* str, char chr)
{
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
	size_t count = 0;

	while ( (str = strchr(str, chr)) )
	{
		*str = replac;
		count++;
		str++;
	}

	return count;
}

size_t str_replace(char* dest, char* src, char* substr, char* replac)
{
	if (!substr[0] || !dest || !src)
		return 0;
	
	size_t substr_len = strlen(substr);
	size_t replac_len = strlen(replac);
	size_t count = 0;

	char* ptr;
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

	return str;
}

#endif

#endif
