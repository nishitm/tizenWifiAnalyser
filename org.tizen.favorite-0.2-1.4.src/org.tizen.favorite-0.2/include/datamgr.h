#ifndef __AIR_FAVORITE_DATAMGR_H__
#define __AIR_FAVORITE_DATAMGR_H__

#include <stdbool.h>
#include <Elementary.h>

enum item_type {
	ITEM_CHANNEL,
	ITEM_MOVIE,
	ITEM_PHOTO,
	ITEM_VIDEO,
	ITEM_GALLERY,
	ITEM_MUSIC,
	ITEM_APPS,
	ITEM_WEBS,
};

enum data_type {
	DATA_NAME = 0,
	DATA_THUMBNAIL,
	DATA_PATH,
	DATA_ARTIST,
	DATA_ALBUM_NAME,
	DATA_DURATION,
	DATA_SUBTITLE,
	DATA_DESCRIPTION,
};

enum action_type {
	ACTION_LAUNCH = 0,
	ACTION_UPDATE_FAVORITE,
	ACTION_UPDATE_LOCK,
};

struct datamgr {
	Eina_List *(*get_favorites)(enum item_type type);
	void (*free_favorites)(Eina_List *list);
	int (*get_count)(enum item_type type);
	char *(*get_data)(void *data, enum data_type type);
	bool (*action)(Elm_Object_Item *it, enum action_type type, void *data);
	bool (*data_updated)(void *data1, void *data2);
	bool (*data_locked)(Elm_Object_Item *it, bool *locked);
};

struct datamgr *get_channel_datamgr(void);
struct datamgr *get_media_datamgr(void);
struct datamgr *get_apps_datamgr(void);
struct datamgr *get_webs_datamgr(void);

#endif /* __AIR_FAVORITE_DATAMGR_H__ */
