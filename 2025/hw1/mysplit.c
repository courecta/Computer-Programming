#include "mysplit.h"

int32_t mysplit(char ***pppStrs, const char *pStr, const char *delim)
{
    if(!pppStrs || !pStr || !delim)
        return -1;
    
    if(delim[0] == '\0')
        return -1;

    int32_t count = 1;  // At least one string
    const char *tmp = pStr;
    while((tmp = strstr(tmp, delim)))
    {
        count++;
        tmp += strlen(delim);
    }

    char **ppStrs = (char**)malloc(count * sizeof(char*));
    if(!ppStrs)
        return -1;
    
    *pppStrs = ppStrs;

    if(pStr[0] == '\0')
    {
        ppStrs[0] = strdup("");
        return 1;
    }

    char *str = strdup(pStr);
    if(!str)
    {
        free(ppStrs);
        return -1;
    }

    int32_t i = 0;
    char *token = str;
    char *next = strstr(str, delim);
    
    while(next)
    {
        *next = '\0';
        ppStrs[i] = strdup(token);
        token = next + strlen(delim);
        next = strstr(token, delim);
        i++;
    }
    
    ppStrs[i] = strdup(token); // Handle last token

    free(str);
    return count;
}