#pragma once

#include <stdbool.h>

#ifndef INI_LINE_DATA_SIZE
#define INI_LINE_DATA_SIZE 256 // Remember that this is stack allocated
#endif

typedef enum {KEY, VALUE, SECTION, COMMENT} ini_parse_state;
typedef char ini_parse_line_data[3][INI_LINE_DATA_SIZE];
typedef enum {CONTINUE /* Continue calling the function */, FINISHED /* The function has finished. Don't call it again. */, FAILURE /* Something went wrong. Your input was wrong the file is corrupted, whatever. */} ini_parse_success;

// Parses the INI file path is pointing to
// The return value is owned by the caller and needs to be freed by the caller
// The return value will be NULL on failure
// The return value will be NULL if the key is empty
extern void ini_get_str(char buffer[INI_LINE_DATA_SIZE], const char* path, const char* section, const char* key);

static inline ini_parse_success _ini_get_char_parse_char(ini_parse_state* parse_state, ini_parse_line_data line_data, char current, char last, bool* inQuotes, const char* section, const char* key);

static inline void _set_parse_state(ini_parse_state* parse_state, ini_parse_line_data line_data, const ini_parse_state new_parse_state);

// Compares line_data with section and key
static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key);

// Adds addition to base
// The return value is owned by the called and needs to be freed by the caller
// The return value will be NULL on failure
static void _add_str_and_char(char* base, const char addition);
