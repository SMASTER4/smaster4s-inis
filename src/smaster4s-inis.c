#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "smaster4s-inis.h"

extern int32_t ini_get_int32_t_from_str(const char* text, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str_from_str(buffer, text, section, key);
  return atoi(buffer);
}


extern int64_t ini_get_int64_t_from_str(const char* text, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str_from_str(buffer, text, section, key);
  return strtoll(buffer, NULL, 10);
}


extern uint64_t ini_get_uint64_t_from_str(const char* text, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str_from_str(buffer, text, section, key);
  return strtoull(buffer, NULL, 10);
}


extern void ini_get_str_from_str(char buffer[INI_LINE_DATA_SIZE], const char* text, const char* section, const char* key) {
  if(buffer == NULL || text == NULL || key == NULL)
    return;

  ini_parse_state parse_state = KEY;
  bool inQuotes = false;
  ini_parse_line_data line_data = {"", "", ""};

  for(size_t current = 0; current < strlen(text); current++) {
    ini_parse_success parse_context = _ini_get_str_parse_char(&parse_state, line_data, *(text + current), current > 0 ? *(text + current - 1) : '\0', &inQuotes, section, key);
    switch(parse_context) {
      case CONTINUE:
        continue;
      case FAILURE:
        return;
      case FINISHED:
        strncpy(buffer, line_data[VALUE], INI_LINE_DATA_SIZE);
        return;
    }
  }
}

extern int32_t ini_get_int32_t(const char* path, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str(buffer, path, section, key);
  return atoi(buffer);
}

extern int64_t ini_get_int64_t(const char* path, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str(buffer, path, section, key);
  return strtoll(buffer, NULL, 10);
}

extern uint64_t ini_get_uint64_t(const char* path, const char* section, const char* key) {
  char buffer[INI_LINE_DATA_SIZE];
  *buffer = '\0';
  ini_get_str(buffer, path, section, key);
  return strtoull(buffer, NULL, 10);
}


extern void ini_get_str(char buffer[INI_LINE_DATA_SIZE], const char* path, const char* section, const char* key) {
  if(buffer == NULL || path == NULL || key == NULL)
    return;

  FILE* file = fopen(path, "r");
  if(file == NULL)
    return;

  ini_parse_state parse_state = KEY;
  bool inQuotes = false;
  char current;
  char last = '\0';
  ini_parse_line_data line_data = {"", "", ""};

  while((current = fgetc(file)) != EOF) {
    ini_parse_success parse_context = _ini_get_str_parse_char(&parse_state, line_data, current, last, &inQuotes, section, key);
    switch(parse_context) {
      case CONTINUE:
        last = current;
        continue;
      case FAILURE:
        break;
      case FINISHED:
        strncpy(buffer, line_data[VALUE], INI_LINE_DATA_SIZE);
        break;
    }
    break;
  }

  fclose(file);
}

static ini_parse_success _ini_get_str_parse_char(ini_parse_state* parse_state, ini_parse_line_data line_data, char current, char last, bool* inQuotes, const char* section, const char* key) {
    if(last == '\\' && *parse_state != COMMENT)
      goto add_char;
    if(current == '\\')
      return CONTINUE;

    if(current == '\n') {
      if(_key_compare(line_data, section, key) && parse_state != KEY)
        return FINISHED;
      _set_parse_state(parse_state, line_data, KEY);
      return CONTINUE;
    }

    if(*parse_state == COMMENT)
      return CONTINUE;

    if(current == '"') {
      *inQuotes = !(*inQuotes);
      return CONTINUE;
    }
    if(*inQuotes)
      return CONTINUE;

    switch(current) {
      case '=':
        _set_parse_state(parse_state, line_data, VALUE);
        return CONTINUE;
      case '[':
        _set_parse_state(parse_state, line_data, SECTION);
        return CONTINUE;
      case ']':
        return CONTINUE;
      case '#':
        *parse_state = COMMENT;
        return CONTINUE;
      case ';':
        *parse_state = COMMENT;
        return CONTINUE;
    }

    add_char:
    _add_str_and_char(line_data[*parse_state], current);
    return CONTINUE;
}

static inline void _set_parse_state(ini_parse_state* parse_state, ini_parse_line_data line_data, const ini_parse_state new_parse_state) {
  if(parse_state == NULL)
    return;
  *parse_state = new_parse_state;
  if(line_data == NULL)
    return;
  *line_data[new_parse_state] = '\0';
}

static bool _key_compare(const ini_parse_line_data line_data, const char* section, const char* key) {
  if(key == NULL || line_data[KEY] == NULL)
    return false;
  if(strcmp(key, line_data[KEY]) != 0)
    return false;
  if(section == NULL)
    return line_data[SECTION] == NULL || *line_data[SECTION] == '\0';

  return strcmp(section, line_data[SECTION]) == 0;
}

static void _add_str_and_char(char* base, const char addition) {
  size_t base_length = base == NULL ? 0 : strlen(base);

  if(base_length + sizeof(addition) + sizeof('\0') > INI_LINE_DATA_SIZE)
    return;

  *(base + base_length) = addition;
  *(base + base_length + sizeof(addition)) = '\0';
}
