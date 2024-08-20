#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "smaster4s-inis.h"

extern char* ini_get_char(const char* path, const char* section, const char* key) {
  if(path == NULL || key == NULL)
    return NULL;
  FILE* file = fopen(path, "r");
  if(file == NULL)
    return NULL;

  ini_parse_state parse_state = KEY;
  bool inQuotes = false;
  char current;
  char last = '\0';
  ini_parse_line_data line_data = {NULL, NULL, NULL, NULL};

  next_char:
  if((current = fgetc(file)) != EOF) {
    if(last == '\\' && current == '\n')
      goto next_char;

    if(current == '\n') {
      if(_key_compare(line_data, section, key) && parse_state != KEY) {
        fclose(file);
        char* value = malloc(strlen(line_data[VALUE]) + sizeof('\0'));
        strcpy(value, line_data[VALUE]);
        _free_line_data(line_data);
        return value;
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
        _set_parse_state(&parse_state, line_data, COMMENT);
        goto next_char;
      case ';':
        _set_parse_state(&parse_state, line_data, COMMENT);
        goto next_char;
      default:
        break;
    }

    char* new_data; // You can't declare a variable behind a lable in C11
    add_char:
    new_data = _add_str_and_char(line_data[parse_state], current);
    if(new_data == NULL) {
      fclose(file);
      _free_line_data(line_data);
      return NULL;
    }
    if(line_data[parse_state] != NULL)
      free(line_data[parse_state]);
    line_data[parse_state] = new_data;

    last = current;
    goto next_char;
  }
  fclose(file);
  _free_line_data(line_data);
  return NULL;
}

static inline void _set_parse_state(ini_parse_state* parse_state, ini_parse_line_data line_data, const ini_parse_state new_parse_state) {
  *parse_state = new_parse_state;
  free(line_data[new_parse_state]);
  line_data[new_parse_state] = NULL;
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

static char* _add_str_and_char(const char* base, const char addition) {
  size_t base_length = base == NULL ? 0 : strlen(base);
  char* result = (char*) malloc(base_length + sizeof(addition) + sizeof('\0'));
  if(result == NULL)
    return NULL;

  if(base != NULL)
    strcpy(result, base);

  *(result + base_length) = addition;
  *(result + base_length + sizeof(addition)) = '\0';

  return result;
}

static void _free_line_data(ini_parse_line_data line_data) {
  for(size_t i = 0; i < 4; i++) {
    if(line_data[i] != NULL) {
      free(line_data[i]);
      line_data[i] = NULL;
    }
  }
}
