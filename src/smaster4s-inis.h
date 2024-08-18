#pragma once

typedef enum {KEY, VALUE, SECTION, COMMENT} ini_parse_state;
typedef char* ini_parse_line_data[4];

// Parses the INI file path is pointing to
// The return value is owned by the caller and needs to be freed by the caller
// The return value will be NULL on failure
// The return value will be NULL if the key is empty
extern const char* ini_get_char(const char* path, const char* section, const char* key);

// Compares line_data with section and key
static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key);

// Adds addition to base
// The return value is owned by the called and needs to be freed by the caller
// The return value will be NULL on failure
static char* _add_str_and_char(const char* base, const char addition);

// Frees line_data
static void _free_line_data(ini_parse_line_data line_data);
