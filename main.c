#include <stdio.h>
#include "json/json.h"

int test_lex(char* s) {

   int num_lexemes, size_lexemes;
   LEXEME* lexemes = lex_str(s, &num_lexemes, &size_lexemes);

   int i = 0;
   for (; i < num_lexemes; i++) {
      switch (lexemes[i].type) {
      case l_open_dict:
         printf("\n{");
         break;
      case l_close_dict:
         printf("\n}");
         break;
      case l_open_lst:
         printf("\n[");
         break;
      case l_close_lst:
         printf("\n]");
         break;
      case l_semi:
         printf("\n:");
         break;
      case l_comma:
         printf("\n,");
         break;
      case l_str:
         printf("\nSTRINGGGGG\n\t %s", lexemes[i].str_value);
         break;
      case l_num:
         printf("\nNUMBER\n\t %Lf", lexemes[i].num_value);
      default:
         break;
      }
   }
   return 0;
}

void print_json(JSON* j) {
   switch (j->type) {
   case json_str:
      printf("%s", (char*)(j->p));
      break;
   case json_num:
      printf("%Le", *((long double*)(j->p)));
      break;
   case json_dict:
      JSON_DICT *j = (JSON_DICT*)(j->p);
      int i;
      for (i = 0; i < j->num_entries; i++) {
         printf("key: %s", j->keys[i]);
         printf("value: ");
         print_json(j->entries[i]);
      }
      break;
   case json_lst:
      JSON_LST *j = (JSON_LST*)(j->p);
      int i;
      for (i = 0; i < j->items_len; i++) {
         print_json(&(j->items[i]));
      }
      break;
   default:
      printf("bad json type");
      break;
   }
}

void test_parse(int nargs, char* s) {

   int num_lexemes, size_lexemes;
   LEXEME* lexemes = lex_str(s, &num_lexemes, &size_lexemes);

   JSON* j = parse(lexemes, num_lexemes);
   print_json(j);

}

int main(int nargs, char** args) {
   char* s = "{ - 23.43 't est' {'hello': 5 'world'}, [] 'test' 3 ''";

   if (nargs == 2) {
      FILE* f = fopen(args[1], "r");
      //READ FILE HERE
      //s = blah();
   }

   //result = test_lex(nargs, args);
   test_parse(nargs, args);
}
