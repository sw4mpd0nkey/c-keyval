#include <stdio.h>
#include <kv.h>
#include <assert.h>
#include <string.h>

void test_kv_init() {
    printf("Testing kv_init()\n");
    kv_t *db = kv_init(16);
    assert(db != NULL);
    assert(db->capacity == 16);
    assert(db->count == 0);
    kv_free(db);
    printf("Test Complete\n----------------------------------\n\n");
}

void test_kv_put_get() {
    printf("Testing kv_put and kv_get()\n");

    kv_t *db = kv_init(32);

    kv_put(db, "fruit", "apple");
    kv_put(db, "veggie", "carrot");
    kv_put(db, "grain", "wheat");
    kv_put(db, "protein", "egg");
    kv_put(db, "protein", "burger");

    printf("added five entries with one dupe, next testing getting keys\n");

    assert(strcmp(kv_get(db, "fruit"), "apple") == 0);
    assert(strcmp(kv_get(db, "veggie"), "carrot") == 0);
    assert(strcmp(kv_get(db, "grain"), "wheat") == 0);
    assert(strcmp(kv_get(db, "protein"), "burger") == 0);

    printf("all get calls look ok, attempting an entry that dne\n");

    assert(kv_get(db, "missing") == NULL);

    kv_free(db);
    printf("Test Complete\n----------------------------------\n\n");
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


