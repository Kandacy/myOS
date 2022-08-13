
#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "type.h"

void memcpy(u8 *dest, const u8 *src, u32 len);
void memset(void *dest, u8 val, u32 len);
void memmove(u8 *dest, const u8 *src, u32 len);
int memcmp(const void *ptr1, const void *ptr2, u32 num);
void bzero(void *dest, u32 len);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(const char *src);

#endif
