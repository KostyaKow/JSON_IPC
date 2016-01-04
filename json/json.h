#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef const char* cstring;

enum JSON_TYPE {
   json_str,
   json_dict,
   json_lst,
   json_num //best precision by default
};

typedef struct JSON {
   void* p;
   enum JSON_TYPE type;
} JSON;

typedef struct JSON_LST {
   JSON* items;
   int items_len;
} JSON_LST;

typedef struct JSON_DICT {
   char** keys;
   JSON** entries;
   uint32_t num_entries;
   uint32_t size_entries;
} JSON_DICT;

///

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

//cstring json_get_str(const JSON* item);
//float json_get_float(const JSON* item);
//const JSON_LST* json_get_lst(const JSON* item);
//const JSON_DICT* json_get_dict(const JSON* item);

//creates JSON with elemnt of type type
/*JSON* new_json(enum JSON_TYPE type, void* element);
void free_json(JSON* j);
void json_dict_add_entry(JSON* j, cstring key, JSON* entry);*/

LEXEME* lex_str(cstring s, int* num_lexemes_, int* size_lexemes_);
char* lex_to_str(LEXEME* l);

JSON* parse(LEXEME* lexemes, int num_lexemes);


///new STUFF
struct LexTree;
typedef struct LexTree {
   struct _lex_tree_elem {
      union {
         struct LexTree* t;
         LEXEME* l;
      };
      bool atom;
   } *elems;
   int num, size;
} LexTree;

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

