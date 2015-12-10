#include <stdio.h>
#include "json/json.h"

int test_lex(int nargs, char** args) {
   char* s = "{ 23.43 't est' {'hello': 5 'world'}, [] 'test' 3 ''";

   int num_lexemes, size_lexemes;
   LEXEME* lexemes;

   if (nargs != 2)
      lexemes = lex_str(s, &num_lexemes, &size_lexemes);
   else {
      FILE* f = fopen(args[1], "r");
      //READ FILE HERE
      char* str_to_lex;
      lexemes = lex_str(str_to_lex, &num_lexemes, &size_lexemes);
   }
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
int main(int nargs, char** args) {
   return test_lex(nargs, args);
}
