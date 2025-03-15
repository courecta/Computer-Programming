#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT (8192)
#define MAX_OUTPUT (16384)

void convert_json_to_toml(const char *json, char *toml);
void convert_toml_to_json(const char *toml, char *json);
void parse_json_object(const char **p, char *toml, const char *parent_prefix);
char *trim_whitespace(char *str);
void convert_inline_table_to_json(const char *inline_table, char *output);