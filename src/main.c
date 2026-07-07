#include <stdio.h>
#include <kv.h>
#include <assert.h>
#include <string.h>

void test_kv_init() {
    kv_t *db = kv_init(16);
    assert(db != NULL);
    assert(db->capacity == 16);
    assert(db->count == 0);
    kv_free(db);
}

void test_kv_put_get() {
    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    assert(strcmp(kv_get(db, "name"), "alice") == 0);
    assert(kv_get(db, "missing") == NULL);

    kv_free(db);
}

void test_kv_delete() {
    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    kv_put(db, "city", "berlin");

    assert(kv_delete(db, "name") == 0);
    assert(kv_get(db, "name") == NULL);
    assert(db->count == 1);

    assert(kv_delete(db, "missing") == -1);

    kv_free(db);
}

int main() {
    test_kv_init();
    test_kv_put_get();
    test_kv_delete();
}


