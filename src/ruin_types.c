#include "ruin_types.h"
#include <stdlib.h>

RUIN_String* RUIN_CreateString() {
	RUIN_String* __tmp = (RUIN_String*)malloc(sizeof(RUIN_String));
	__tmp->capacity = 2;
	__tmp->data = NULL;
	__tmp->size = 0;
	return __tmp;
}

RUIN_String* RUIN_CreateStringFrom(char* __str) {
	RUIN_String* __tmp = RUIN_CreateString();
	while (*__str != '\0') {
		RUIN_StringAppendChar(__tmp, *__str);
		++__str;
	}
	return __tmp;
}

RUIN_STATUS_CODE RUIN_StringAppendString(RUIN_String* __str, char* __src) {
	if (__str == NULL || __src == NULL) return RUIN_PASSED_NULL;

	while (*__src != '\0') {
		if (RUIN_StringAppendChar(__str, *__src) != RUIN_SUCCESS)
			return RUIN_FAILURE;
		__src++;
	}

	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_StringAppendRuinString(RUIN_String* __str, RUIN_String* __src) {
	if (__str == NULL || __src == NULL) return RUIN_PASSED_NULL;
	RUIN_StringAppendString(__str, RUIN_StringData(__src));
	return RUIN_SUCCESS;
}

RUIN_STATUS_CODE RUIN_StringAppendChar(RUIN_String* __str, char __ch) {
	if (__str == NULL) return RUIN_PASSED_NULL;
	if (__str->data == NULL) {
		__str->data = (char*)malloc(__str->capacity);
		__str->data[__str->capacity] = '\0';
	}

	if (__str->size >= __str->capacity - 1) {
		__str->capacity *= 2;
		__str->data = (char*)realloc(__str->data, __str->capacity);
		if (__str->data == NULL) {
			RUIN_ErrorfReturn("%s: RUIN_AddCharToString => realloc returned NULL\n", __FILE__, RUIN_FAILURE);
		}
		__str->data[__str->capacity] = '\0';
	}

	__str->data[__str->size++] = __ch;
	return RUIN_SUCCESS;
}

char* RUIN_StringData(RUIN_String* __str) {
	if (__str == NULL || __str->size == 0) return NULL;
	return __str->data;
}

u64 RUIN_StringSize(RUIN_String* __str) {
	if (__str == NULL) return 0;
	return __str->size;
}

RUIN_Bool RUIN_IsStringEmpty(RUIN_String* __str) {
	if (__str == NULL) return RUIN_TRUE;
	return (__str->size) ? RUIN_FALSE : RUIN_TRUE;
}

RUIN_STATUS_CODE RUIN_DestroyString(RUIN_String* __str) {
	if (__str == NULL) return RUIN_PASSED_NULL;
	free(__str->data);
	free(__str);
	return RUIN_SUCCESS;
}