#ifndef RAID_H
#define RAID_H

typedef struct Raid {
    void* data;
} Raid;

typedef struct GuildMember {
    Raid** raids;
    int raid_count;
    struct GuildMember** friends;
    int friend_count;
} GuildMember;

typedef struct {
    void* data;
} RaidService;

Raid* raid_create(void);
void raid_destroy(Raid* raid);

GuildMember* guild_member_create(void);
void guild_member_destroy(GuildMember* member);
GuildMember** guild_member_get_friends(GuildMember* member);
void guild_member_add_friend(GuildMember* member, GuildMember* friend);
void guild_member_add_raid(GuildMember* member, Raid* raid);
Raid** guild_member_get_raids(GuildMember* member);

GuildMember* guild_dao_find_active_player(void);
Raid** raid_dao_find_raids_by(GuildMember* member);

RaidService* raid_service_create(void);
void raid_service_destroy(RaidService* service);
Raid** raid_service_get_raids_by_guild_member(RaidService* service, GuildMember* other);

#endif /* RAID_H */
