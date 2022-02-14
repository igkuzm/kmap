/**
 * File              : kmap.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 14.02.2022
 * Last Modified Date: 14.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include <stdio.h>
#include "klist/klist.h"

typedef KLIST KMAP;

typedef enum k_map_err {
	KMAP_ERR_NONE = 0,
	KMAP_ERR_NO_MAP_WITH_KEY
} KMAP_ERR;

typedef struct k_map_node_t {
	char key[BUFSIZ];
	void *value;
} KMAPNODE;

//create new MAP to store key/value pairs
KMAP *k_map_new();

//set (create or rewrite) value for key
void k_map_set_value_for_key(KMAP *map, void *value, const char *key);

//return value pointer for key
void *k_map_get_value_for_key(KMAP *map, const char *key);

//remove key/value pair
KMAP_ERR k_map_remove_map_for_key(KMAP *map, const char *key);

//remove all key/value pairs
void k_map_remove_all(KMAP *map);

//remove KMAP and free memory
void k_map_free(KMAP *map);

//number of all key/value pairs
int k_map_size(KMAP *map);

//return number of key/value pairs and fill array of keys - dont forget free(keys)
int k_map_all_keys(KMAP *map, char ***keys);

//return number of key/value pairs and fill array of nodes - dont forget free(nodes)
int k_map_all_nodes(KMAP *map, KMAPNODE ***nodes);

//return the return of callback. Callback stops if not 0 return
int k_map_foreach_node(KMAP *map, void *user_data, int (*callback)(KMAPNODE *node, void *user_data));
