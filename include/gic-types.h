#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned int virt_addr_t;
typedef unsigned int virt_size_t;
typedef unsigned long long phys_addr_t;
typedef unsigned long long phys_size_t;

/* signed number */
typedef signed int s32;
typedef signed long long s64;

#define TRUE            1
#define FALSE           0
#define NULL            ((void *)0)

#endif /* __TYPES_H__ */
