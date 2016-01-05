#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef const char* cstring;

///LEX
enum LEXEME_TYPE {
   l_open_dict, l_close_dict,
   l_open_lst, l_close_lst,
   l_semi, l_comma,
   l_str, l_num
};

typedef struct LEXEME {
   enum LEXEME_TYPE type;
   union {
      char* str_value;
      long double num_value;
   };
} LEXEME;

LEXEME* lex_str(cstring s, int* num_lexemes_, int* size_lexemes_);
///END LEX

///LEX TREE
struct LexTree;
typedef struct LexTree {
   struct _lex_tree_elem {
      union {
         struct LexTree* t;
         LEXEME* l;
      };
      bool atom;
   } *elems;
   uint32_t num, size;
} LexTree;
///END LEX TREE

///JSON PARSE
//to get a thing out of JSON, cast p to it

enum JSON_TYPE {
   json_dict,
   json_lst,
   json_str,
   json_num //best precision by default
};

typedef struct JSON {
   void* p;
   enum JSON_TYPE type;
} JSON;

typedef struct JSON_DICT {
   struct {
      char* key;
      JSON* entry;
   } *items;

   uint32_t num, size;
} JSON_DICT;

typedef struct JSON_LST {
   JSON* items;
   int num, size;
} JSON_LST;

JSON* new_json(enum JSON_TYPE type, void* element);
JSON_DICT* new_dict(void);
JSON_LST* new_lst(void);

JSON* parse(LexTree* tree);

///END JSON PARSE

///OLD STUFF
/*
typedef struct JSON_DICT {
   char** keys;
   JSON** entries;
   uint32_t num_entries;
   uint32_t size_entries;
} JSON_DICT;

//cstring json_get_str(const JSON* item);
//float json_get_float(const JSON* item);
//const JSON_LST* json_get_lst(const JSON* item);
//const JSON_DICT* json_get_dict(const JSON* item);

//creates JSON with elemnt of type type
JSON* new_json(enum JSON_TYPE type, void* element);
void free_json(JSON* j);
void json_dict_add_entry(JSON* j, cstring key, JSON* entry);

JSON* parse(LEXEME* lexemes, int num_lexemes);
*/
///END OLD STUFF

///PRINTERS
char* lex_to_str(LEXEME* l);
///END PRINTERS

#endif // JSON_H_INCLUDED

/* TODO:
   long double = %L[e|f|g]
      e scientific notation
      f = floating point
      g = no crap

   new file for printer-functions
   maybe make a new phase where we don't parse stuff into json, but create lexeme trees?

   =============================old/maybe come back later
   different keys other than string
      //int (*cmp)(void* a, void* b);
      //void** key;
   sort shit when it's added
   isNum currently accepts stuff like "1-....352-3" <-- bad parsing
   we don't have a way to escape strings
   " and ' are interchangeable
   max number that can be encoded is only 31 digits (not true anymore)
   Add stuff to read from file and command line arguments

   write a good json parser in Haskell with error handling and syntax.
*/

