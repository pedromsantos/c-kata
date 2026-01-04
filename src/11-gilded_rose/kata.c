#include "kata.h"
#include <stdlib.h>
#include <string.h>

Item* item_create(const char* name, int sell_in, int quality) {
    Item* item = (Item*)malloc(sizeof(Item));
    if (item) {
        item->name = strdup(name);
        item->sell_in = sell_in;
        item->quality = quality;
    }
    return item;
}

void item_destroy(Item* item) {
    if (item) {
        free(item->name);
        free(item);
    }
}

GildedRose* gilded_rose_create(Item* items, int item_count) {
    GildedRose* rose = (GildedRose*)malloc(sizeof(GildedRose));
    if (rose) {
        rose->items = items;
        rose->item_count = item_count;
    }
    return rose;
}

void gilded_rose_destroy(GildedRose* rose) {
    if (rose) {
        free(rose);
    }
}

void gilded_rose_update_quality(GildedRose* rose) {
    if (!rose) return;

    for (int i = 0; i < rose->item_count; i++) {
        Item* item = &rose->items[i];

        if (strcmp(item->name, "Aged Brie") != 0 &&
            strcmp(item->name, "Backstage passes to a TAFKAL80ETC concert") != 0) {
            if (item->quality > 0 && strcmp(item->name, "Sulfuras, Hand of Ragnaros") != 0) {
                item->quality = item->quality - 1;
            }
        } else if (item->quality < 50) {
            item->quality = item->quality + 1;
            if (strcmp(item->name, "Backstage passes to a TAFKAL80ETC concert") == 0) {
                if (item->sell_in < 11 && item->quality < 50) {
                    item->quality = item->quality + 1;
                }
                if (item->sell_in < 6 && item->quality < 50) {
                    item->quality = item->quality + 1;
                }
            }
        }

        if (strcmp(item->name, "Sulfuras, Hand of Ragnaros") != 0) {
            item->sell_in = item->sell_in - 1;
        }

        if (item->sell_in < 0) {
            if (strcmp(item->name, "Aged Brie") != 0) {
                if (strcmp(item->name, "Backstage passes to a TAFKAL80ETC concert") != 0) {
                    if (item->quality > 0 && strcmp(item->name, "Sulfuras, Hand of Ragnaros") != 0) {
                        item->quality = item->quality - 1;
                    }
                } else {
                    item->quality = item->quality - item->quality;
                }
            } else if (item->quality < 50) {
                item->quality = item->quality + 1;
            }
        }
    }
}
