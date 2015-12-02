#include <stdio.h>
#include "json/json.h"

int main(int nargs, char** args) {
   char* s = "{\"hello\":\"world\"}, []";

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

   printf("got here");

   int i;
   for (i = 0; i < num_lexemes; i++) {
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
         printf("\nSTRINGGGGG\n\t %s", lexemes[i].value);
         break;
      default:
         break;
      }
   }
   return 0;
}
