
#ifndef __TYPE_H
#define __TYPE_H


#if 0
typedef int __attribute__((__mode__(QI))) i8;
typedef unsigned __attribute__((__mode__(QI))) u8;
typedef int      __attribute__((__mode__(HI))) i16;
typedef unsigned __attribute__((__mode__(HI))) u16;
typedef int      __attribute__((__mode__(SI))) i32;
typedef unsigned __attribute__((__mode__(SI))) u32;
typedef int      __attribute__((__mode__(DI))) i64;
typedef unsigned __attribute__((__mode__(DI))) u64;
#else
typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
#endif



#define NULL  0


#endif /* __TYPE_H */
