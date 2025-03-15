#include "mystring.h"
// TAs : We should handle null pointers, despite what the original implement was
char *mystrchr(const char *s, int c)
{
	if(s == NULL)
		return NULL;
	
	while(*s)
	{
		if(*s == (char)c)
			return (char*)s;
		s++;
	}
	
	if(c == '\0')
		return (char*)s;

	return NULL;
}

char *mystrrchr(const char *s, int c)
{
	if(s == NULL)
		return NULL;
	
	char *last_occurrence = NULL;

	while(*s)
	{
		if(*s == (char)c)
			last_occurrence = (char*)s;
		s++;
	}

	if(c == '\0')
		return (char*)s;

	return last_occurrence;
}

size_t mystrspn(const char *s, const char *accept)
{
	if(s == NULL || accept == NULL)
		return 0;

	size_t count = 0;

	while(*s)
	{
		const char *a = accept;
		int32_t found = 0;
		while(*a)
		{
			if(*s == *a)
			{
				found = 1;
				break;
			}
			a++;
		}
		if(!found)
			break;
		
		count++;
		s++;
	}
	return count;
}

size_t mystrcspn(const char *s, const char *reject)
{
	if(s == NULL || reject == NULL)
		return 0;

	size_t count = 0;

	while(*s)
	{
		const char *r = reject;
		while(*r)
		{
			if(*s == *r)
				return count;
			r++;
		}
		count++;
		s++;
	}
	return count;
}

char *mystrpbrk(const char *s, const char *accept)
{
	if(s == NULL || accept == NULL)
		return NULL;

	while(*s)
	{
		const char *a = accept;
		while(*a)
		{
			if(*s == *a)
				return (char*)s;
			a++;
		}
		s++;
	}
	return NULL;
}

char *mystrstr(const char *haystack , const char *needle)
{
	if(haystack == NULL || needle == NULL)
		return NULL;

	if(*needle == '\0')
		return (char*)haystack;

	while(*haystack)
	{
		const char *h = haystack;
		const char *n = needle;
		while(*h && *n && (*h == *n))
		{
			h++;
			n++;
		}

		if(*n == '\0')
			return (char*)haystack;

		haystack++;
	}
	return NULL;
}

char *mystrtok(char *str, const char *delim)
{
	static char *next = NULL;

	if(str == NULL)
		str = next;

	if(str == NULL)
		return NULL;

	while(*str && mystrchr(delim, *str))
		str++;

	if(*str == '\0')
		return NULL;

	char *token = str;

	while(*str && !mystrchr(delim, *str))
		str++;

	if(*str)
	{
		*str = '\0';
		next = str + 1;
	} else next = NULL;

	return token;
}