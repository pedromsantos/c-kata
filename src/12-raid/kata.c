#include "kata.h"
#include <stdlib.h>
#include <stdio.h>

Raid* raid_create(void) {
    Raid* raid = malloc(sizeof(Raid));
    if (raid) {
        raid->data = NULL;
    }
    return raid;
}

void raid_destroy(Raid* raid) {
    if (raid) {
        free(raid);
    }
}

GuildMember* guild_member_create(void) {
    GuildMember* member = malloc(sizeof(GuildMember));
    if (member) {
        member->raids = NULL;
        member->raid_count = 0;
        member->friends = NULL;
        member->friend_count = 0;
    }
    return member;
}

void guild_member_destroy(GuildMember* member) {
    if (member) {
        free(member->raids);
        free(member->friends);
        free(member);
    }
}

GuildMember** guild_member_get_friends(GuildMember* member) {
    return member->friends;
}

void guild_member_add_friend(GuildMember* member, GuildMember* friend) {
    member->friend_count++;
    member->friends = (GuildMember**)realloc(member->friends,
                                              sizeof(GuildMember*) * member->friend_count);
    member->friends[member->friend_count - 1] = friend;
}

void guild_member_add_raid(GuildMember* member, Raid* raid) {
    member->raid_count++;
    member->raids = (Raid**)realloc(member->raids, sizeof(Raid*) * member->raid_count);
    member->raids[member->raid_count - 1] = raid;
}

Raid** guild_member_get_raids(GuildMember* member) {
    return member->raids;
}

GuildMember* guild_dao_find_active_player(void) {
    fprintf(stderr, "Method not implemented\n");
    return NULL;
}

Raid** raid_dao_find_raids_by(GuildMember* member) {
    (void)member;
    fprintf(stderr, "Method not implemented\n");
    return NULL;
}

RaidService* raid_service_create(void) {
    RaidService* service = malloc(sizeof(RaidService));
    if (service) {
        service->data = NULL;
    }
    return service;
}

void raid_service_destroy(RaidService* service) {
    if (service) {
        free(service);
    }
}

Raid** raid_service_get_raids_by_guild_member(RaidService* service, GuildMember* other) {
    (void)service;
    GuildMember* player = guild_dao_find_active_player();

    if (player == NULL) {
        return NULL;
    }

    GuildMember** friends = guild_member_get_friends(other);
    for (int i = 0; i < other->friend_count; i++) {
        if (friends[i] == player) {
            return raid_dao_find_raids_by(other);
        }
    }

    return NULL;
}
