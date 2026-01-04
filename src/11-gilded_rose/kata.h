#ifndef GILDED_ROSE_H
#define GILDED_ROSE_H

typedef struct {
    char* name;
    int sell_in;
    int quality;
} Item;

typedef struct {
    Item* items;
    int item_count;
} GildedRose;

Item* item_create(const char* name, int sell_in, int quality);
void item_destroy(Item* item);

GildedRose* gilded_rose_create(Item* items, int item_count);
void gilded_rose_destroy(GildedRose* rose);
void gilded_rose_update_quality(GildedRose* rose);

#endif /* GILDED_ROSE_H */
