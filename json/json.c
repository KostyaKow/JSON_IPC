#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "json.h"

#define error(s) \
   printf("%s", s);

inline bool isNum(char c) {
   return (c >= 48 && c <=57) || c == '-' || c == '.';
}

JSON* new_json(enum JSON_TYPE type, void* element) {
   JSON* j = (JSON*)malloc(sizeof(JSON));
   j->type = type; //json_dict
   j->p = (void*)element;
   return j;
}

JSON_DICT* new_dict(void) {
   JSON_DICT* d = (JSON_DICT*)malloc(sizeof(JSON_DICT));
   d->num_entries = d->size_entries = 0;
   d->keys = d->entries = 0;
   return d;
}

void json_dict_add_entry(JSON* j, cstring key, JSON* entry) {
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


LEXEME* lex_str(cstring s, int* num_lexemes_, int* size_lexemes_) {
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
      case '"': case '\'':
         lexemes[num_lexemes].type = l_str;
         int j = 1;
         for (; s[j+i] != '"' && s[j+i] != '\''; j++)
            assert(j + i + 1 < len); //TODO: +1 ??
         char* str = malloc(j); //was malloc(j+1);
         strncpy(str, s+i+1, j-1);
         str[j-1] = '\0'; //added -1 here but double check
         lexemes[num_lexemes].str_value = str;
         num_lexemes++;
         i += j;
         break;
      default:
         if (isNum(s[i])) { //handle num
            lexemes[num_lexemes].type = l_num;
            //char digits[32]; //extra character for '\n' so we can only have 31-digit numbers
            int j = 0;
            for (; isNum(s[i+j]); j++) {
               //assert(j < 30 && j+i+1 < len); //TODO: +1 ??
               assert(j+i < len);
               //digits[j] = s[i+j];
            }
            //digits[j+1] = '\0';

            char* end; //= s + i + j;
            lexemes[num_lexemes].num_value = strtold(s + i, &end);

            i += j;
            num_lexemes++;
         }
         break;
      }
   }

   *num_lexemes_ = num_lexemes;
   *size_lexemes_ = size_lexemes;
   return lexemes;
}


JSON* parse(LEXEME* lexemes, int num_lexemes) {
   JSON* ret = NULL;

   int num_lexemes, size_lexemes;
   //LEXEME* lexemes = lex_str(s, &num_lexemes, &size_lexemes);

   int i = 0;
   for (; i < num_lexemes; i++) {
      switch (lexemes[i].type) {


      }

   }

}
