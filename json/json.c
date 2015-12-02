#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef const char* string;

int json_get_int(const JSON* item);
string json_get_str(const JSON* item);
float json_get_float(const JSON* item);
const JSON_LST* json_get_lst(const JSON* item);
const JSON_DICT* json_get_dict(const JSON* item);


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
   //int (*cmp)(void* a, void* b);
   //void** key;
   char** keys;
   void** entries;
} JSON_DICT;
