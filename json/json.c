#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "json.h"

JSON* make_new_json(void) {
    JSON* j = (JSON*)malloc(sizeof(JSON));
    j->type = json_dict;

    JSON_DICT* d = (JSON_DICT*)malloc(sizeof(JSON_DICT));
    d->num_entries = d->size_entries = 0;
    d->keys = d->entries = 0;

    j->p = (void*)d;
    return j;
}

void json_dict_add_entry(JSON* j; const char* key, JSON* entry) {
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
