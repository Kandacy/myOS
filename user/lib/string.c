
#include "lib/string.h"


int strlen(const char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++) {
    }
    return i;
}

void memcpy(u8 *dest, const u8 *src, u32 len)
{
    for (; len != 0; len--) {
        *dest++ = *src++;
    }
}

int memcmp(const void *ptr1, const void *ptr2, u32 num)
{
    for (int i = 0; i < num; ++i) {
        if (((char*)ptr1)[i] != ((char*)ptr2)[i]) {
            return ((char*)ptr1)[i] - ((char*)ptr2)[i];
        }
    }
    return 0;
}

void memmove(u8 *dest, const u8 *src, u32 len)
{
    u8 temp[len];
    i32 i;
    for (i = 0; len != 0; len--) {
        temp[i] = *src++;
    }
    for (; len != 0; len--) {
        *dest++ = temp[i];
    }
}

void memset(void *dest, u8 val, u32 len)
{
    u8 *dst = (u8 *)dest;

    for (; len != 0; len--) {
        *dst++ = val;
    }
}

void bzero(void *dest, u32 len) { memset(dest, 0, len); }

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1) - (*str2);
        }
        ++str1;
        ++str2;
    }
    return (*str1) - (*str2);
}

char *strcpy(char *dest, const char *src)
{
    char *tmp = dest;

    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}

char *strcat(char *dest, const char *src)
{
    char *tmp = dest;

    while (*dest != '\0') { dest++; }
    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}