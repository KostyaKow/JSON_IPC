#include <stdio.h>
#include "json/json.h"

int test_lex(cstring_t s) {

   int num_lexemes, size_lexemes;
   LEXEME* lexemes = lex_str(s, &num_lexemes, &size_lexemes);

   int i = 0;
   for (; i < num_lexemes; i++) {
      printf("%s ", lex_to_str(&lexemes[i]));
      /*switch (lexemes[i].type) {
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
      }*/
   }
   return 0;
}

void test_parse(cstring_t s) {

   int num_lexemes, size_lexemes;
   LEXEME* lexemes = lex_str(s, &num_lexemes, &size_lexemes);

   int ender;
   LexTree* tree = parse_tree(lexemes, num_lexemes, &ender);
   //printLexTree(tree, 0);

   JSON* j = parse(tree);
   print_json(j);
}

int main(int nargs, char** args) {
   //char* s = "{ - 23.43 't est' {'hello': 5 'world'}, [] 'test' 3 ''";
   char* s = "[1, 2, { 'hello':'world', 'test':[33, 4]}, [5, 6], 7, [[]]] 8";

   if (nargs == 2) {
      FILE* f = fopen(args[1], "r");
      //READ FILE HERE
      //s = blah();
   }

   //int result = test_lex(s);
   test_parse(s);
   return 0;
}
