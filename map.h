#ifndef MAP_H
#define MAP_H


extern void *create_map(unsigned int buckets);
extern void put_map(const void *map, const char *key, const void *value);
extern void *get_map(const void *map, const char *key);
extern void destroy_map(const void *map);


#endif // !MAP_H

