#include <kv.h>
#include <string.h>
#include <stdlib.h>

kv_t *kv_init(size_t capacity) {

    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(sizeof(kv_t), capacity);

    if (table->entries == NULL) {
        return NULL;
    }

    return table;
}

size_t hash(const char *value, int capacity) {
    size_t hash = 0x13371337deadbeef;

    while(*value) {
        hash ^= *value;
        hash = hash << 8;
        hash += *value;

        value++;
    }

    return hash % capacity;
}

//kv-put
//params:
//  db : pointer to the db
//  key : pointer to the key val
//  value : ponter to value itself
//returns: index of the key, otherwise -1 on err and -2 not found
int kv_put(kv_t *db, const char *key, const char *value) {
    if (!db || !key || !value) {
        return -1;
    }

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity -1; i++) {

        size_t real_idx = (idx + 1) % db->capacity;
        
        kv_entry_t *entry = &db->entries[real_idx];

        // key is already set, updating
        if (entry->key && entry->key != (void*)TOMBSTONE
            && !strcmp(entry->key,key)) {
            
            char *newval = strdup(value);
            if (!newval) {
                return -1;
            }
            free(entry->value);
            entry->value = newval;
            return real_idx;
        }

        //land in a slot that is empty
        //null or tombstone
        if(!entry -> key || entry->key == (void*)TOMBSTONE) {
            char *newval = strdup(value);
            char *newkey = strdup(key);

            if (!newval || !newkey) {
                free(newkey);
                free(newval);
                
                return -1;
            } 

            entry->value = newval;
            entry->key = newkey;
            db->count++;
            return real_idx;
        }
    }

    //db is occupied
    return -1;
}

char *kv_get(kv_t *db, const char *key) {
    if( !db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity; i++) {
        size_t real_idx = (idx + 1) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // no
        if (entry->key == NULL) {
            return NULL;
        }

        // find entry and keys match
        if(entry->key && entry->key != (void *)TOMBSTONE && !strcmp(entry->key, key)) {
            return entry->value;
        }
    }

    return NULL;
}

int kv_delete(kv_t *db, const char *key) {
    if (!db || !key) return -1;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity; i++) {
        size_t real_idx = (idx +1) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // no
        if (entry->key == NULL) {
            return -1;
        }

        // find entry and keys match
        if(entry->key && 
            entry->key != (void *)TOMBSTONE && 
            !strcmp(entry->key, key)
        ) {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = (void *)TOMBSTONE;
            entry->value = NULL;

            return real_idx;
        }

    }

    return 0;
}

void kv_free(kv_t *db) {

    for (int i =0; i < db->capacity; i++) {
        kv_entry_t *e = &db->entries[i];

        if(e->key && e->key != (void *)TOMBSTONE) {
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }
    }

    free(db->entries);
    free(db);

}