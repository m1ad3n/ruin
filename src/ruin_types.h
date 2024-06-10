#ifndef RUIN_TYPES_HEADER
#define RUIN_TYPES_HEADER

#include <stdbool.h>
#include <stdio.h>

/* Error handling */
typedef enum {
	
	RUIN_FAILURE = 0x0,
	RUIN_SUCCESS,

	RUIN_PASSED_NULL = 0x10,

} RUIN_STATUS_CODE;

/* ERROR HANDLING */
#define RUIN_Error(error_message) fprintf(stderr, error_message);
#define RUIN_Errorf(error_message, format) fprintf(stderr, error_message, format);
#define RUIN_ErrorReturn(error_message, return_value) fprintf(stderr, error_message); return return_value;
#define RUIN_ErrorfReturn(error_message, format, return_value) fprintf(stderr, error_message, format); return return_value;

#define RUIN_SDL_Error(error_message) fprintf(stderr, error_message, SDL_GetError());
#define RUIN_SDL_ErrorReturn(error_message, return_value) fprintf(stderr, error_message, SDL_GetError()); return return_value;

/* Basic data types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef void RUIN_Void;

typedef bool RUIN_Bool;
#define RUIN_TRUE true
#define RUIN_FALSE false

#define RUIN_PRIVATE static
#define RUIN_API __stdcall

/* string */
typedef struct {

	char* data;
	u64 capacity;
	u64 size;

} RUIN_String;

RUIN_String* RUIN_CreateString();
RUIN_String* RUIN_CreateStringFrom(char* __data);
RUIN_Bool RUIN_IsStringEmpty(RUIN_String* __str);
RUIN_STATUS_CODE RUIN_StringAppendChar(RUIN_String* __str, char __ch);
RUIN_STATUS_CODE RUIN_StringAppendString(RUIN_String* __str, char* __src);
RUIN_STATUS_CODE RUIN_StringAppendRuinString(RUIN_String* __str, RUIN_String* __src);
char* RUIN_StringData(RUIN_String* __str);
u64 RUIN_StringSize(RUIN_String* __str);
RUIN_STATUS_CODE RUIN_DestroyString(RUIN_String* __str);

#endif