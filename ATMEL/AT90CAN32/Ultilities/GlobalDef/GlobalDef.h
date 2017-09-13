//***********************************************
//	GlobalDef.h
//***********************************************

#ifndef _GLOBALDEF_H_
#define _GLOBALDEF_H_

/* ******************************************************************
 *	TYPES
 * ******************************************************************/

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef unsigned long long U64;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef signed long long S64;

typedef enum {FALSE = 0, TRUE = 1} bool;

typedef enum {RESET = 0, SET = 1} FlagState, BitState, BitAction;

typedef enum {DISABLE = 0, ENABLE = 1} EnableState;

typedef enum {FAIL = 0, PASS = 1} ResultState;

#endif /* _GLOBALDEF_H_*/
