#ifndef MKTL_STR_TOOLS_H
#define MKTL_STR_TOOLS_H

#include <string.h>
#include <stddef.h>

size_t chr_count(const char* str, char chr);
size_t chr_replace(char* str, char chr, char replac);
size_t str_count(const char* str, const char* substr);
size_t str_replace(char* dest, const char* src, const char* substr, const char* replac);
char* str_reverse(char* str);
char* str_lstrip(char* dest, const char* src, const char* to_strip);
char* str_rstrip(char* dest, const char* src, const char* to_strip);

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

char* str_lstrip(char* dest, const char* src, const char* to_strip)
{
	if (!dest || !src || !to_strip)
		return dest;

	char chr;
	while ( (chr = *src) )
	{
		if ( !strchr(to_strip, chr) )
			break;
		src++;
	}

	strcpy(dest, src);

	return dest;
}

char* str_rstrip(char* dest, const char* src, const char* to_strip)
{
	if ( !dest || !src || !to_strip )
		return dest;
	if ( !src[0] )
	{
		*dest = '\0';
		return dest;
	}

	size_t src_len = strlen(src);
	const char* last_chr = src + src_len - 1;

	char chr;
	while ( (chr = *(last_chr--)) )
	{
		if ( !strchr(to_strip, chr) )
			break;

		if ( --src_len == 0 )
			break;
	}

	memcpy(dest, src, src_len);
	*(dest + src_len) = '\0';

	return dest;
}

#endif

#endif
