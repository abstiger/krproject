#include <stdlib.h>
#include <assert.h>
#include "krutils/kr_utils.h"
#include "krutils/kr_cache.h"

static void small_test(void)
{
    T_KRCache *cache = kr_cache_new (3, kr_string_hash, kr_string_equal,
					                 (KRDupFunc )kr_strdup, kr_free,
	                                 NULL, NULL, NULL);
	assert(cache != NULL);
	
	int array[3] = {1, 2, 3};
	kr_cache_set(cache, "a", (void *)&array[0]);
	kr_cache_set(cache, "b", (void *)&array[1]);
	kr_cache_set(cache, "c", (void *)&array[2]);
	
	int a = *(int *)(kr_cache_get(cache, "a"));
	int b = *(int *)(kr_cache_get(cache, "b"));
	int c = *(int *)(kr_cache_get(cache, "c"));
	
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %d\n", c);
	
	assert(a == 1);
	assert(b == 2);
	assert(c == 3);
	assert(kr_cache_get_size(cache) == 3);
	
	kr_cache_dump(cache, stdout);
	
	kr_cache_free(cache);
}

static void big_test(void)
{
	int i;
	char caKey[10];
	char caValue[50];
	void *value;

    T_KRCache *cache = kr_cache_new (1024, kr_string_hash, kr_string_equal,
					                 (KRDupFunc )kr_strdup, kr_free,
	                                 (KRDupFunc )kr_strdup, kr_free, NULL);
	assert(cache != NULL);
    
    //fill cache node
	for (i = 0; i < 2000; i++) {
	    memset(caKey, 0x00, sizeof(caKey));
	    snprintf(caKey, sizeof(caKey), "%d", i);
	    snprintf(caValue, sizeof(caValue), "value-%d", i);
		kr_cache_set(cache, caKey, caValue);
	}

	kr_cache_set_max_size(cache, 1000);

	assert(kr_cache_get_size(cache) == 1000);
	printf("size = %u, %u\n", kr_cache_get_size(cache), kr_list_length(cache->list));
	
	// should be cache hit
	printf("Expecting cache hit\n");
	value = kr_cache_get(cache, "1800");
	printf("Check Key:[%s], Value:[%s]\n", "1800", (char *)value);

	// should be cache hit
	printf("Expecting cache hit\n");
	value = kr_cache_get(cache, "1803");
	printf("Check Key:[%s], Value:[%s]\n", "1803", (char *)value);
	
	// should be miss
	printf("Expecting cache miss\n");
	value = kr_cache_get(cache, "999");
	printf("Check Key:[%s], Value:[%s]\n", "999", (char *)value);

	// should be miss
	printf("Expecting cache miss\n");
	value = kr_cache_get(cache, "993");
	printf("Check Key:[%s], Value:[%s]\n", "993", (char *)value);
	
	kr_cache_dump(cache, stdout);
	
	kr_cache_free(cache);
}


int main(int argc, char *argv[])
{
	small_test();
	big_test ();
	
	exit(0);
}