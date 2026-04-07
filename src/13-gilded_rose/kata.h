#ifndef ROSE_INCLUDED
#define ROSE_INCLUDED

typedef struct
{
    char *name;
    int sellIn;
    int quality;
} Item;

typedef struct
{
    Item* items;
    int size;
} GildedRose;

extern Item* init_item(Item* item, const char *name, int sellIn, int quality);
extern void update_quality(Item items[], int size);
extern char* print_item(char* buffer, Item* item);

extern Item* item_create(const char *name, int sellIn, int quality);
extern void item_destroy(Item* item);
extern GildedRose* gilded_rose_create(Item* items, int size);
extern void gilded_rose_destroy(GildedRose* rose);
extern void gilded_rose_update_quality(GildedRose* rose);

#endif