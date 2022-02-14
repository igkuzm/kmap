**KEY/VALUE container in C**

Map container to store key/value pairs (like NSDictionary in ObjectiveC) written in pure C.  Key is string - Value is pointer.
Simple example:

```
#include "kmap.h"

int main(int argc, char *argv[])
{
	KMAP *map = k_map_new(); //create new container
	k_map_set_value_for_key(map, "hello", "привет"); //set value for key "hello"
	k_map_set_value_for_key(map, "world", "мир"); //set value for key "world"

	char *hello_in_russian = k_map_get_value_for_key(map, "hello");
	char *world_in_russian = k_map_get_value_for_key(map, "world");
	
	printf("Let's say HELLO WORL in Russian:\n")
	printf("%s %s!", hello_in_russian, world_in_russian);

	k_map_free(map); //free memory
	
	return 0;
}



```
