#include <string.h>
#include "kata.h"
#include <stdio.h>
#include <stdlib.h>

Item*
item_create(const char *name, int sellIn, int quality)
{
    Item* item = malloc(sizeof(Item));
    if (item) {
        init_item(item, name, sellIn, quality);
    }
    return item;
}

void
item_destroy(Item* item)
{
    if (item) {
        free(item->name);
        free(item);
    }
}

GildedRose*
gilded_rose_create(Item* items, int size)
{
    GildedRose* rose = malloc(sizeof(GildedRose));
    if (rose) {
        rose->items = items;
        rose->size = size;
    }
    return rose;
}

void
gilded_rose_destroy(GildedRose* rose)
{
    if (rose) {
        free(rose);
    }
}

void
gilded_rose_update_quality(GildedRose* rose)
{
    if (rose) {
        update_quality(rose->items, rose->size);
    }
}

Item*
init_item(Item* item, const char *name, int sellIn, int quality)
{
    item->sellIn = sellIn;
    item->quality = quality;
    item->name = strdup(name);

    return item;
}

extern char*
print_item(char* buffer, Item* item)
{
    sprintf(buffer, "%s, %d, %d", item->name, item->sellIn, item->quality);
}

void
update_quality(Item items[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (strcmp(items[i].name, "Aged Brie") && strcmp(items[i].name, "Backstage passes to a TAFKAL80ETC concert"))
        {
            if (items[i].quality > 0)
            {
                if (strcmp(items[i].name, "Sulfuras, Hand of Ragnaros"))
                {
                    items[i].quality = items[i].quality - 1;
                }
            }
        }
        else
        {
            if (items[i].quality < 50)
            {
                items[i].quality = items[i].quality + 1;

                if (!strcmp(items[i].name, "Backstage passes to a TAFKAL80ETC concert"))
                {
                    if (items[i].sellIn < 11)
                    {
                        if (items[i].quality < 50)
                        {
                            items[i].quality = items[i].quality + 1;
                        }
                    }

                    if (items[i].sellIn < 6)
                    {
                        if (items[i].quality < 50)
                        {
                            items[i].quality = items[i].quality + 1;
                        }
                    }
                }
            }
        }

        if (strcmp(items[i].name, "Sulfuras, Hand of Ragnaros"))
        {
            items[i].sellIn = items[i].sellIn - 1;
        }

        if (items[i].sellIn < 0)
        {
            if (strcmp(items[i].name, "Aged Brie"))
            {
                if (strcmp(items[i].name, "Backstage passes to a TAFKAL80ETC concert"))
                {
                    if (items[i].quality > 0)
                    {
                        if (strcmp(items[i].name, "Sulfuras, Hand of Ragnaros"))
                        {
                            items[i].quality = items[i].quality - 1;
                        }
                    }
                }
                else
                {
                    items[i].quality = items[i].quality - items[i].quality;
                }
            }
            else
            {
                if (items[i].quality < 50)
                {
                    items[i].quality = items[i].quality + 1;
                }
            }
        }
    }
}