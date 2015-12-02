#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "json.h"

#define error(s) \
   printf("%s", s);

JSON_DICT* new_dict(void) {
   JSON_DICT* d = (JSON_DICT*)malloc(sizeof(JSON_DICT));
   d->num_entries = d->size_entries = 0;
   d->keys = d->entries = 0;
   return d;
}

JSON* new_json(enum JSON_TYPE type, void* element) {
   JSON* j = (JSON*)malloc(sizeof(JSON));
   j->type = type; //json_dict
   j->p = (void*)element;
   return j;
}

void json_dict_add_entry(JSON* j, string key, JSON* entry) {
   if (j->type != json_dict)
      exit(-1);
   JSON_DICT* d = (JSON_DICT*)j->p;

   if (d->num_entries >= d->size_entries + 1) {
      d->size_entries = d->size_entries * 2 + 1;
      d->keys = realloc(d->keys, d->size_entries * sizeof(char*));
      d->entries = realloc(d->keys, d->size_entries * sizeof(JSON*));
   }

   d->keys[d->num_entries] = key;
   d->entries[d->num_entries] = entry;
   d->num_entries += 1;
}


/*JSON* parse_string1(string s, int start, int end) {
   JSON* j = NULL; //= (JSON*)malloc(sizeof(JSON));
   //assert(strlen(s) > )
   while (start < end) {
      if (s[i] == '{') {
         JSON_DICT* d = new_dict;
         j = new_json(json_dict, d);

      }
      start++;
   }
}*/




LEXEME* lex_str(string s, int* num_lexemes_, int* size_lexemes_) {
   LEXEME* lexemes = NULL;
   uint32_t num_lexemes = 0, size_lexemes = 0;

   int len = strlen(s), i;
   for (i = 0; i < len; i++) {
      if (num_lexemes >= size_lexemes) {
         size_lexemes = size_lexemes * 2 + 1;
         lexemes = realloc(lexemes, size_lexemes * sizeof(LEXEME));
      }
      switch (s[i]) {
      case '{':
         lexemes[num_lexemes++].type = l_open_dict;
         break;
      case '}':
         lexemes[num_lexemes++].type = l_close_dict;
         break;
      case '[':
         lexemes[num_lexemes++].type = l_open_lst;
         break;
      case ']':
         lexemes[num_lexemes++].type = l_close_lst;
         break;
      case ':':
         lexemes[num_lexemes++].type = l_semi;
         break;
      case ',':
         lexemes[num_lexemes++].type = l_comma;
         break;
      case '"':
         lexemes[num_lexemes].type = l_str;
         int j = 0;
         for (; s[j+i] != '"'; j++)
            assert(j + i + 1 < len);
         char* str = malloc(j + 1);
         strncpy(str, s+i, j);
         lexemes[num_lexemes].value = str;
         num_lexemes++;
         i += j;
         break;
      default:
         break;
      }
   }

   *num_lexemes_ = num_lexemes;
   *size_lexemes_ = size_lexemes;
   return lexemes;
}
