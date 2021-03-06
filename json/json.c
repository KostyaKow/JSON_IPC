#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "json.h"

#define error(s) \
   printf("%s", s);

int _debug_loop_rec_counter = 0;
#define _DEBUG_MAX_ITERATIONS 200
#define _DEBUG_CHECK_REC \
   assert(_debug_loop_rec_counter++ < _DEBUG_MAX_ITERATIONS)


///LEX
inline bool isNum(char c) {
   return (c >= 48 && c <=57) || c == '-' || c == '.';
}

LEXEME* lex_str(cstring_t s, int* num_lexemes_, int* size_lexemes_) {
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

            i += j; i--; //TODO: check why i-- works
            num_lexemes++;
         }
         break;
      }
   }

   *num_lexemes_ = num_lexemes;
   *size_lexemes_ = size_lexemes;
   return lexemes;
}
///END LEX

///LEX TREE

inline LexTree* newLexTree(void) {
   LexTree* t = malloc(sizeof(LexTree));
   t->num = t->size = 0;
   t->elems = NULL;
   return t;
}

inline void addLexTree(LexTree* t, void* v, bool atom) {
   if (t->num + 1 >= t->size) {
      t->size = t->size*2 + 1;
      t->elems = realloc(t->elems, t->size * sizeof(struct _lex_tree_elem));
   }
   t->elems[t->num].atom = atom;
   if (atom)
      t->elems[t->num].l = (LEXEME*)v;
   else
      t->elems[t->num].t = (LexTree*)v;
   t->num++;
}

// if passed list and atom, return only list and ending place
LexTree* parse_tree(LEXEME* lexemes, int num_lexemes, int* ender) {
   _DEBUG_CHECK_REC; assert(num_lexemes > 0);

   int i, j;
   LexTree* tree = newLexTree();
   LEXEME* l = &lexemes[0];
   enum LEXEME_TYPE t = l->type;

   addLexTree(tree, l, true);
   //printLexTree(tree, 0);
   //printf("kk:%s\n", lex_to_str(l));

   //no naked atom at root
   assert(t != l_str && t != l_num);
   assert (t == l_open_lst || t == l_open_dict);

   int nestedness = 1;
   for(i = 1; i < num_lexemes; i++) {
      t = lexemes[i].type;
      if (t == l_open_lst || t == l_open_dict)
         nestedness++;
      if (t == l_close_lst || t == l_close_dict)
         nestedness--;
      if (nestedness == 0)
         break;
   }
   assert(nestedness == 0); //unclosed dict/lst detected
   *ender = i;
   for (j = 1; j <= i; j++) {
      l = &lexemes[j];
      t = l->type;

      //if (t == l_str || t = l_num || t == l_comma || t == l_semi || t == l_close_dict || t == l_close_lst)
      if (t != l_open_lst && t != l_open_dict)
         addLexTree(tree, l, true);
      else if (t == l_open_lst || t == l_open_dict) {
         int end;
         LexTree* sub_tree = parse_tree(lexemes + j, num_lexemes - j, &end);
         addLexTree(tree, sub_tree, false);
         j += end; //skip parsed thing
      }
   }
   return tree;
}

///END LEX TREE

///JSON PARSE
JSON* new_json(enum JSON_TYPE type, void* element) {
   JSON* j = (JSON*)malloc(sizeof(JSON));
   j->type = type; //json_dict
   j->p = (void*)element;
   return j;
}

JSON* new_json_num(num_t num) {
   num_t* n = malloc(sizeof(num_t));
   *n = num;
   return new_json(json_num, n);
}

JSON* new_json_str(cstring_t str) {
   char* s = malloc(strlen(str));
   strcpy(s, str);
   return new_json(json_str, s);
}

JSON_DICT* new_dict(void) {
   JSON_DICT* d = (JSON_DICT*)malloc(sizeof(JSON_DICT));
   d->num = d->size = 0;
   d->items = NULL;
   return d;
}

void dict_add_entry(JSON* j, cstring_t key, JSON* entry) {
   assert(j->type == json_dict);

   JSON_DICT* d = (JSON_DICT*)j->p;

   if (d->num + 1 >= d->size) {
      d->size = d->size * 2 + 1;
      d->items = realloc(d->items, d->size * sizeof(struct _dict_item));
   }
   struct _dict_item* i = &d->items[d->num];
   i->key = key;
   i->entry = entry;
   d->num += 1;
}

JSON_LST* new_lst(void) {
   JSON_LST* l = (JSON_LST*)malloc(sizeof(JSON_LST));
   l->num = l->size = 0;
   l->items = NULL;
   return l;
}

void lst_add_item(JSON* j, JSON* item) {
   assert(j->type == json_lst);

   JSON_LST* l = j->p;

   if (l->num + 1 >= l->size) {
      l->size = l->size * 2 + 1;
      l->items = realloc(l->items, l->size * sizeof(JSON));
   }
   l->items[l->num] = *item;
   l->num++;
}

   /*
   if (l->type == l_str || l->type == l_num) //removeme
      assert(tree->num == 1);

   switch (l->type) {
   case l_str:
      ret = new_json(json_str, l->str_value);
      return ret;
      break;
   case l_num:
      long double* d = malloc(sizeof(long double));
      *d = &l->num_value;
      ret = new_json(json_num, d);
      return ret;
      break;
   default:
      assert(0 == "bad lexeme");
   }*/


JSON* parse_tree_entry_to_json(struct _lex_tree_elem *elem, bool* close_dict, bool* close_lst) {
   LEXEME* lexeme = NULL;
   JSON* ret = NULL;
   enum LEXEME_TYPE t;
   *close_lst = false;
   *close_dict = false;

   if (!elem->atom)
      ret = parse(elem->t);
   else {
      lexeme = elem->l;
      t = lexeme->type;
      if (t == l_close_lst)
         *close_lst = true;
      else if (t == l_close_dict)
         *close_dict = true;
      else if (t == l_str)
         ret = new_json_str(lexeme->str_value);
      else if (t == l_num)
         ret = new_json_num(lexeme->num_value);
      else //can only be str or num at this point
         assert(0 == 2);
   }
   return ret;
}

JSON* parse(LexTree* tree) {
   _DEBUG_CHECK_REC;
   JSON* ret = NULL;
   LEXEME* lexeme = NULL;
   enum LEXEME_TYPE t;
   int i = 0;
   bool close_dict, close_lst;

   struct _lex_tree_elem *elem = &tree->elems[0];
   assert(elem->atom);

   lexeme = elem->l; //printf("%s", lex_to_str(lexeme));
   t = lexeme->type;

   assert(t == l_open_dict || t == l_open_lst);

   if (t == l_open_dict) { //dictionary
      JSON_DICT* d = new_dict();
      ret = new_json(json_dict, d);

      for (i = 1; i < tree->num; i++) {
         elem = &tree->elems[i];
         assert(elem->atom); //key is always string
         lexeme = elem->l;
         t = lexeme->type;

         //removeme printf("%s", lex_to_str(lexeme)); printf("%i", t == l_str);
         assert(t == l_str || t == l_close_dict);
         if (t == l_close_dict) //if close dictionary, then return parsed thingy
            return ret;
         assert(i + 3 < tree->num); //otherwise, make sure we have "key":blah
         char* key = lexeme->str_value;

         //check that we have ":"
         elem = &tree->elems[i+1];
         assert(elem->atom);
         lexeme = elem->l;
         assert(lexeme->type == l_semi);

         elem = &tree->elems[i+2]; //entry

         JSON* child = parse_tree_entry_to_json(elem, &close_dict, &close_lst);
         assert(!close_dict && !close_lst); //should be our entry
         dict_add_entry(ret, key, child);

         elem = &tree->elems[i+3]; //] or ,
         assert(elem->atom);
         t = elem->l->type;
         assert(t == l_comma || t == l_close_dict);
         if (t == l_comma)
            i+=3;
         else if (t == l_close_dict)
            return ret;
      }
      assert(0 == 1);
   }
   else if (t == l_open_lst) { //list
      JSON_LST* lst = new_lst();
      ret = new_json(json_lst, lst);

      for (i = 1; i < tree->num; i++) {
         elem = &tree->elems[i];

         JSON* child = parse_tree_entry_to_json(elem, &close_dict, &close_lst);
         if (close_lst)
            return ret;
         assert(!close_dict);
         lst_add_item(ret, child);

         //check that next element is comma or bracket. it has to be
         if (i+1 < tree->num) {
            elem = &tree->elems[i+1];
            assert(elem->atom);
            t = elem->l->type;
            //kk printf("%s", lex_to_str(elem->l)); //kk removeme
            assert(t == l_comma || t == l_close_lst);
            if (t == l_comma)
               i++;
         }
      }
      //this means we didn't find list ender ]
      assert(0 == 1);
   }

   assert(0 == 1);
   return NULL;
}
///END JSON PARSE


///OLD STUFF
/*
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


JSON* parse(LEXEME* lexemes, int num_lexemes) {
   JSON* ret = NULL;

   int start, end;

   LEXEME* lexeme = &lexemes[0];
   switch (lexeme->type) {
   case l_str:
      ret = new_json(json_str, lexeme->str_value);
      return ret;
      break;
   case l_num:
      ret = new_json(json_num, lexeme->num_value);
      break;
   case l_open_lst:
      int i = 0, nestedness = 1;
      for(i = 1; i < num_lexemes; i++) {
         if (lexemes[i].type == l_open_lst)
            nestedness++;
         if (lexemes[i].type == l_close_lst)
            nestedness--;
         if (nestedness == 0)
            break;
      }
      assert(nestedness == 0);

   }
}


JSON* parse_string1(string s, int start, int end) {
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
///END OLD STUFF






