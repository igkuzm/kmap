/**
 * File              : test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 14.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "kmap.h"
#include <stdio.h>
#include <stdlib.h>

char readChar(){
	char a;
	scanf("%c", &a);
	return a;	
}

char *readString(){
	int i = 0;
	char *a = (char *) calloc(BUFSIZ, sizeof(char));
	if (!a) {
		fprintf(stderr, "ERROR. Cannot allocate memory\n");		
		return NULL;
	}										
	
	while (1) {
		scanf("%c", &a[i]);
		if (a[i] == '\n') {
			break;
		}
		else {
			i++;
		}
	}
	a[i] = '\0';
	return a;
}

void print_header(){
	system("clear");	
	printf("#######################################\n");
	printf("KMAP test\n");
	printf("#######################################\n");
	printf("\n");
}

int print_map_callback(void *item, void *user_data) {
	int *i = user_data;
	KMAPNODE *node = item;
	char *key = node->key;
	char *value = node->value;
	
	printf("%d.\t'%s':\t'%s'\n", *i, key, value);
	i[0]++;

	return 0;
}

void print_list(KMAP *map){
	printf("#######################################\n");
	printf("KMAP ITEMS:\n");
	if (k_list_is_empty(map)) {
		printf("NULL\n");
	} else {
		int i = 0;
		k_list_foreach_item(map, &i, print_map_callback);
	}
	printf("#######################################\n");
	printf("\n");
}

void print_navigation(){
	printf("#######################################\n");
	printf("s - set value for key\n");
	printf("g - get value for key\n");
	printf("k - get all keys\n");
	printf("n - get all nodes(key/value pairs)\n");
	printf("r - remove map for key\n");
	printf("a - remove all\n");
	printf("#######################################\n");
	printf("q - quit\n");
}

void start_test(KMAP *map){
Start:;
	print_header();
	print_list(map);
	print_navigation();
	char c;
	while (1){		
		c = readChar();
		switch (c) {
			case 'q':
				goto Exit;
				break;
			case 's':
				{
					printf("Enter key\n");
					getchar();
					char *key = readString();
					printf("Enter value\n");
					char *value = readString();					
					k_map_set_value_for_key(map, value, key);
					goto Start;
					break;
				}				
			case 'g':
				{
					printf("Enter key\n");
					getchar();
					char *key = readString();
					char *value = k_map_get_value_for_key(map, key);
					if (value != NULL) {
						printf("RESULT: '%s' : '%s'\n", key, value);
					} else {
						printf("ERROR\n");
					}
					getchar();
					goto Start;
					break;
				}				
			case 'r':
				{
					printf("Enter key\n");
					getchar();
					char *key = readString();
					int err = k_map_remove_map_for_key(map, key);
					if (err) {
						printf("ERROR\n");
					} else {
						printf("OK\n");
					}
					getchar();
					goto Start;
					break;
				}				
			case 'a':
				{
					getchar();
					k_map_remove_all(map);
					goto Start;
					break;
				}				
			case 'k':
				{
					getchar();
					char **keys;
					int count = k_map_all_keys(map, &keys);
					int i;
					for (i = 0; i < count; ++i) {
						printf("%d. '%s'\n", i, keys[i]);
					}
					free(keys);
					getchar();
					goto Start;
					break;
				}				
			case 'n':
				{
					getchar();
					KMAPNODE **nodes;
					int count = k_map_all_nodes(map, &nodes);
					int i;
					for (i = 0; i < count; ++i) {
						char *value = nodes[i]->value;
						printf("%d. '%s' : '%s'\n", i, nodes[i]->key, value);
					}
					free(nodes);
					getchar();
					goto Start;
					break;
				}				
			default:
				goto Start;
				break;
		}	
	}
	Exit:;
	printf("Exit...\n");
	k_map_free(map);
}

int main(int argc, char *argv[])
{
	printf("Starting klist test...\n");
	KMAP *map = k_map_new();
	start_test(map);
	return 0;
}

