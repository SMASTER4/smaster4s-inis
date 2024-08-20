#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "smaster4s-inis.h"

extern void ini_get_char(char buffer[256], const char* path, const char* section, const char* key) {
  if(buffer == NULL || path == NULL || key == NULL)
    return;
  buffer[0] = '\0';
  FILE* file = fopen(path, "r");
  if(file == NULL)
    return;

  ini_parse_state parse_state = KEY;
  bool inQuotes = false;
  char current;
  char last = '\0';
  ini_parse_line_data line_data = {"", "", ""};

  next_char:
  if((current = fgetc(file)) != EOF) {
    if(last == '\\' && parse_state != COMMENT)
      goto add_char;
    if(current == '\\') {
      last = current;
      goto next_char;
    }

    if(current == '\n') {
      if(_key_compare(line_data, section, key) && parse_state != KEY) {
        fclose(file);
        strncpy(buffer, line_data[VALUE], 256 * sizeof(char));
        return;
      }
      _set_parse_state(&parse_state, line_data, KEY);
      goto next_char;
    }

    if(parse_state == COMMENT)
      goto next_char;

    if(current == '"') {
      inQuotes = !inQuotes;
      goto next_char;
    }
    if(inQuotes)
      goto add_char;

    switch(current) {
      case '=':
        _set_parse_state(&parse_state, line_data, VALUE);
        goto next_char;
      case '[':
        _set_parse_state(&parse_state, line_data, SECTION);
        goto next_char;
      case ']':
        goto next_char;
      case '#':
        parse_state = COMMENT;
        goto next_char;
      case ';':
        parse_state = COMMENT;
        goto next_char;
      default:
        break;
    }

    add_char:
    _add_str_and_char(line_data[parse_state], current);
    last = current;
    goto next_char;
  }
  fclose(file);
  return;
}

static inline void _set_parse_state(ini_parse_state* parse_state, ini_parse_line_data line_data, const ini_parse_state new_parse_state) {
  *parse_state = new_parse_state;
  *line_data[new_parse_state] = '\0';
}

static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key) {
  if(key == NULL || line_data[KEY] == NULL)
    return false;
  if(strcmp(key, line_data[KEY]) != 0)
    return false;
  if(section == NULL)
    return line_data[SECTION] == NULL;

  return strcmp(section, line_data[SECTION]) == 0;
}

static void _add_str_and_char(char* base, const char addition) {
  size_t base_length = base == NULL ? 0 : strlen(base);

  if(base_length + sizeof(addition) + sizeof('\0') >= 256)
    return;

  *(base + base_length) = addition;
  *(base + base_length + sizeof(addition)) = '\0';
}
