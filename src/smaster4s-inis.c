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
  char current;
  char last = '\0';
  ini_parse_line_data line_data = {NULL, NULL, NULL, NULL};

  next_char:
  if((current = fgetc(file)) != EOF) {
    if(current == '\n') {
      if(_key_compare(line_data, section, key) && parse_state != KEY) {
        fclose(file);
        return line_data[VALUE];
      }

      parse_state = KEY;
      goto next_char;
    }

    if(last == '\\' && current == '\n')
      goto next_char;

    if(parse_state == COMMENT)
      goto next_char;

    switch(current) {
      case '=':
        parse_state = KEY;
        goto next_char;
      case '[':
        parse_state = SECTION;
        goto next_char;
      case ']':
        goto next_char;
      case '#':
        comment:
        parse_state = COMMENT;
        goto next_char;
      case ';':
        goto comment;
      default:
        break;
    }

    char* new_data;
    add_char:
    new_data = _add_str_and_char(line_data[parse_state], current);
    if(new_data == NULL) {
      fclose(file);
      _free_line_data(line_data);
      return NULL;
    }
    free(line_data[parse_state]);
    line_data[parse_state] = new_data;

    last = current;
  }
  fclose(file);
  _free_line_data(line_data);
  return NULL;
}

static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key) {
  if(key != NULL || line_data[KEY] != NULL)
    return false;
  if(strcmp(key, line_data[KEY]) == 0)
    return true;
  if(section == NULL)
    return line_data[SECTION] == NULL;

  return strcmp(section, line_data[SECTION]) == 0;
}

static char* _add_str_and_char(const char* base, const char addition) {
  if(base == NULL)
    return NULL;
  char* result = (char*) malloc(strlen(base) + sizeof(addition) + sizeof('\0'));
  if(result == NULL)
    return NULL;
  strcpy(result, base);

  *(result + strlen(base)) = addition;
  *(result + strlen(base) + sizeof(addition)) = '\0';

  return result;
}

static void _free_line_data(ini_parse_line_data line_data) {
    for(size_t i = 0; i < 4; i++) {
      if(line_data[i] == NULL)
        free(line_data[i]);
    }
}
