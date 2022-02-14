/**
 * File              : kmap.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 14.02.2022
 * Last Modified Date: 14.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "kmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

KMAPNODE *k_map_node_alloc(){
	KMAPNODE *node = malloc(sizeof(KMAPNODE));
	if (node == NULL) {
		perror("KNODE malloc");
		exit(EXIT_FAILURE);
	}
	return node;
}

KMAPNODE *k_map_node_new(void *value, const char *key){
	KMAPNODE *node = k_map_node_alloc();
	node->value = value;
	strncpy(node->key, key, BUFSIZ - 1);
	node->key[BUFSIZ - 1] = '\0';
	return node;
}

KMAP *k_map_new(){
	KMAP *map = k_list_new();
	return map;
};

int k_map_get_value_for_key_callback(void *item, void *user_data){
	KLIST *list = user_data;
	void **value = k_list_item_at(list, 0);
	const char *key = k_list_item_at(list, 1); 
	KMAPNODE *node = item;
	
	if (strcmp(node->key, key) == 0) {
		*value = node->value;
		return 1;
	}
	return 0;
}

void *k_map_get_value_for_key(KMAP *map, const char *key){
	if (k_list_is_empty(map))
		return NULL;	

	void *value = NULL;
	KLIST *list = k_list_new();
	k_list_append(list, &value);
	k_list_append(list, (void *)key);
	
	k_list_foreach_item(map, list, k_map_get_value_for_key_callback);
	k_list_free(list);

	return value;
}

int k_map_set_value_for_key_callback(void *item, void *user_data){
	KMAPNODE *node = item;
	KMAPNODE *data = user_data;
	if (strcmp(node->key, data->key) == 0) {
		node->value = data->value;
		return 1;
	}
	return 0;
}

void k_map_set_value_for_key(KMAP *map, void *value, const char *key) {
	KMAPNODE *node = k_map_node_new(value, key);
	
	if (k_list_is_empty(map)) {
		k_list_append(map, node);
		return;
	}

	int c = k_list_foreach_item(map, node, k_map_set_value_for_key_callback);	

	if (!c) { //there is no NODE with key - create new
		k_list_append(map, node);	
	} else {
		//no need node - the value was set
		free(node);
	}	
}

int k_map_remove_map_for_key_callback(void *item, void *user_data){
	KMAPNODE *node = item;
	KLIST *data = user_data;
	KMAP *map = k_list_item_at(data, 0);
	const char *key = k_list_item_at(data, 1);
	if (strcmp(node->key, key) == 0) {
		k_list_remove_item(map, node);
		return 1;
	}
	return 0;
}

KMAP_ERR k_map_remove_map_for_key(KMAP *map, const char *key) {
	if (k_list_is_empty(map)) {
		return KMAP_ERR_NO_MAP_WITH_KEY;
	}	

	KLIST *data = k_list_new();
	k_list_append(data, map);
	k_list_append(data, (void *)key);
	
	int c = k_list_foreach_item(map, data, k_map_remove_map_for_key_callback);
	k_list_free(data);	
	if (!c)
		return KMAP_ERR_NO_MAP_WITH_KEY;
	
	return KMAP_ERR_NONE;
}

int k_map_free_all_callback(void *item, void *user_data){
	free(item);
	return 0;
}

void k_map_remove_all(KMAP *map){
	//free all items first
	k_list_foreach_item(map, NULL, k_map_free_all_callback);

	//remove all items from list
	k_list_remove_all_items(map);
}

void k_map_free(KMAP *map) {
	//free all items first
	k_list_foreach_item(map, NULL, k_map_free_all_callback);
	
	k_list_free(map);
}

int k_map_size(KMAP *map){
	return k_list_size(map);
}

int k_map_all_keys_callback(void *item, void *user_data){
	KLIST *list = user_data;
	int *i = k_list_item_at(list, 0);
	char **keys = k_list_item_at(list, 1);
	KMAPNODE *node = item;
	keys[*i] = node->key;
	i[0]++;

	return 0;
}

int k_map_all_keys(KMAP *map, char ***_keys) {
	char **keys = malloc(BUFSIZ);
	if (keys == NULL) {
		perror("keys malloc");
		exit(EXIT_FAILURE);
	}
	if (_keys)
		*_keys = keys;
	if (k_list_is_empty(map))
		return 0;
	int i = 0;
	KLIST *list = k_list_new();
	k_list_append_items(list, &i, keys, NULL);
	k_list_foreach_item(map, list, k_map_all_keys_callback);
	k_list_free(list);

	return i;
}

int k_map_all_nodes_callback(void *item, void *user_data){
	KLIST *list = user_data;
	int *i = k_list_item_at(list, 0);
	KMAPNODE **nodes = k_list_item_at(list, 1);
	KMAPNODE *node = item;
	nodes[*i] = node;
	i[0]++;

	return 0;
}

int k_map_all_nodes(KMAP *map, KMAPNODE ***_nodes) {
	KMAPNODE **nodes = malloc(BUFSIZ);
	if (nodes == NULL) {
		perror("nodes malloc");
		exit(EXIT_FAILURE);
	}
	if (_nodes)
		*_nodes = nodes;
	if (k_list_is_empty(map))
		return 0;
	int i = 0;
	KLIST *list = k_list_new();
	k_list_append_items(list, &i, nodes, NULL);
	k_list_foreach_item(map, list, k_map_all_nodes_callback);
	k_list_free(list);

	return i;
}

int k_list_foreach_node_callback(void *item, void *_user_data){
	KMAPNODE *node = item;
	KLIST *list = _user_data;
	void *user_data = k_list_item_at(list, 0);
	int (*callback)(KMAPNODE *node, void *user_data) = k_list_item_at(list, 1);
	return callback(node, user_data);
}

int k_map_foreach_node(KMAP *map, void *user_data, int (*callback)(KMAPNODE *node, void *user_data)) {
	KLIST *list = k_list_new();
	k_list_append_items(list, user_data, callback, NULL);
	int c = k_list_foreach_item(map, list, k_list_foreach_node_callback);
	k_list_free(list);
	return c;
}
