#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifndef INI_LINE_DATA_SIZE
#define INI_LINE_DATA_SIZE 256 // Remember that this is stack allocated
#endif

typedef enum {KEY, VALUE, SECTION, COMMENT} ini_parse_state;
typedef char ini_parse_line_data[3][INI_LINE_DATA_SIZE];
typedef enum {CONTINUE /* Continue calling the function */, FINISHED /* The function has finished. Don't call it again. */, FAILURE /* Something went wrong. Your input was wrong, the file is corrupted, whatever. */} ini_parse_success;

extern int32_t ini_get_int32_t_from_str(const char* text, const char* section, const char* key);

extern int64_t ini_get_int64_t_from_str(const char* text, const char* section, const char* key);

extern uint64_t ini_get_uint64_t_from_str(const char* text, const char* section, const char* key);

// Parses text
// The buffer wont be changed if the function fails
extern void ini_get_str_from_str(char buffer[INI_LINE_DATA_SIZE], const char* text, const char* section, const char* key);

extern int32_t ini_get_int32_t(const char* path, const char* section, const char* key);

extern int64_t ini_get_int64_t(const char* path, const char* section, const char* key);

extern uint64_t ini_get_uint64_t(const char* path, const char* section, const char* key);

// Parses the INI file the path is pointing to
// The buffer wont be changed if the function fails
extern void ini_get_str(char buffer[INI_LINE_DATA_SIZE], const char* path, const char* section, const char* key);

static inline ini_parse_success _ini_get_char_parse_char(ini_parse_state* parse_state, ini_parse_line_data line_data, char current, char last, bool* inQuotes, const char* section, const char* key);

static inline void _set_parse_state(ini_parse_state* parse_state, ini_parse_line_data line_data, const ini_parse_state new_parse_state);

// Compares line_data with section and key
static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key);

// Adds addition to base
// The return value is owned by the called and needs to be freed by the caller
// The return value will be NULL on failure
static void _add_str_and_char(char* base, const char addition);
