/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <glib.h>
#include <Elementary.h>
#include <gridmgr.h>
#include <app_debug.h>
#include <app_contents.h>

#include "define.h"
#include "grid.h"

#define STYLE_MUSIC "style.music"
#define ITEM_MUSIC_X (488 + 26)
#define ITEM_MUSIC_Y (134 + 26)

static char *_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *text = NULL;

	if (!data) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	if (!strcmp(part, PART_THUMB_TEXT))
		text = dmgr->get_data(data, DATA_NAME);
	else if (!strcmp(part, PART_THUMB_SUBTEXT1))
		text = dmgr->get_data(data, DATA_ARTIST);
	else if (!strcmp(part, PART_THUMB_SUBTEXT2))
		text = dmgr->get_data(data, DATA_ALBUM_NAME);

	if (!text)
		return NULL;

	return strdup(text);
}

static Evas_Object *_content_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	Evas_Object *icon, *img;
	char *thumbnail;

	if (!data || !obj) {
		_ERR("Invalid argument.");
		return NULL;
	}

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	thumbnail = dmgr->get_data(data, DATA_THUMBNAIL);

	if (!thumbnail) {
		icon = elm_icon_add(obj);
		if (!icon) {
			_ERR("elm_icon_add failed.");
			return NULL;
		}

		elm_icon_thumb_set(icon, dmgr->get_data(data, DATA_PATH), NULL);
		elm_image_aspect_fixed_set(icon, EINA_FALSE);

		return icon;
	}

	img = elm_image_add(obj);
	if (!img) {
		_ERR("elm_image_add failed.");
		return NULL;
	}

	elm_image_file_set(img, thumbnail, NULL);
	elm_image_aspect_fixed_set(img, EINA_FALSE);
	evas_object_show(img);

	return img;
}

static struct grid_class _gclass = {
	.item_style = STYLE_MUSIC,
	.text_get = _text_get,
	.content_get = _content_get,
	.state_get = NULL,
	.del = NULL,
};

static Eina_List *_create_item_list(void)
{
	struct datamgr *dmgr;

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->get_favorites)
		return NULL;

	return dmgr->get_favorites(ITEM_MUSIC);
}

static void _destroy_item_list(Eina_List *list)
{
	struct datamgr *dmgr;

	if (!list)
		return;

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->free_favorites)
		return;

	dmgr->free_favorites(list);
}

static bool _item_action(Elm_Object_Item *it, enum action_type type, void *data)
{
	struct datamgr *dmgr;

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->action)
		return false;

	if (!dmgr->action(it, type, data)) {
		_ERR("The item action failed.");
		return false;
	}

	return true;
}

static bool _item_updated(void *pre_data, void *new_data)
{
	struct datamgr *dmgr;

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->data_updated)
		return false;

	return dmgr->data_updated(pre_data, new_data);
}

static struct grid_data _gdata = {
	.id = GRID_MUSIC,
	.item_size_x = ITEM_MUSIC_X,
	.item_size_y = ITEM_MUSIC_Y,
	.gclass = &_gclass,
	.create_item_list = _create_item_list,
	.destroy_item_list = _destroy_item_list,
	.item_action = _item_action,
	.item_updated = _item_updated,
};

struct grid_data *get_music_grid_data(void)
{
	return &_gdata;
}
