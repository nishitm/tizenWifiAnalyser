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

#include <Elementary.h>
#include <gridmgr.h>
#include <app_debug.h>
#include <app_contents.h>

#include "define.h"
#include "grid.h"
#include "datamgr.h"

#define STYLE_TV "style.tv"
#define ITEM_TV_X (378 + 26)
#define ITEM_TV_Y (294 + 26)

static char *_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *name;

	if (!data) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	name = dmgr->get_data(data, DATA_NAME);
	if (!name)
		return NULL;

	return name;
}

static Evas_Object *_content_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	Evas_Object *img, *rect;
	struct color_data bright, dark;
	char *thumbnail, *name;

	if (!data || !obj) {
		_ERR("Invalid argument.");
		return NULL;
	}

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	if (!strcmp(part, PART_THUMB_ICON)
			|| !strcmp(part, PART_THUMB_DEFAULT_TV_ICON)) {
		thumbnail = dmgr->get_data(data, DATA_THUMBNAIL);

		img = elm_image_add(obj);
		if (!img) {
			_ERR("elm_image_add failed.");
			return NULL;
		}
		elm_image_aspect_fixed_set(img, EINA_FALSE);
		evas_object_show(img);

		if (!strcmp(part, PART_THUMB_ICON) && thumbnail)
			elm_image_file_set(img, thumbnail, NULL);
		else if (!strcmp(part, PART_THUMB_DEFAULT_TV_ICON)
				&& !thumbnail)
			elm_image_file_set(img, DEFAULT_TV_PNG, NULL);
		else
			img = NULL;

		return img;
	} else if (!strcmp(part, PART_THUMB_BG)) {
		name = dmgr->get_data(data, DATA_NAME);

		rect = evas_object_rectangle_add(obj);
		if (!rect) {
			_ERR("elm_rectangle_add failed.");
			return NULL;
		}

		app_contents_get_color(name, &bright, &dark);
		evas_object_color_set(rect, bright.r, bright.g,
				bright.b, bright.a);

		return rect;
	} else if (!strcmp(part, PART_TEXT_BG)) {
		name = dmgr->get_data(data, DATA_NAME);

		rect = evas_object_rectangle_add(obj);
		if (!rect) {
			_ERR("elm_rectangle_add failed.");
			return NULL;
		}

		app_contents_get_color(name, &bright, &dark);
		evas_object_color_set(rect, dark.r, dark.g, dark.b, dark.a);

		return rect;
	}

	return NULL;
}

static struct grid_class _gclass = {
	.item_style = STYLE_TV,
	.text_get = _text_get,
	.content_get = _content_get,
	.state_get = NULL,
	.del = NULL,
};

static Eina_List *_create_item_list(void)
{
	struct datamgr *dmgr;

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->get_favorites)
		return NULL;

	return dmgr->get_favorites(ITEM_CHANNEL);
}

static void _destroy_item_list(Eina_List *list)
{
	struct datamgr *dmgr;

	if (!list)
		return;

	dmgr = get_channel_datamgr();
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

	dmgr = get_channel_datamgr();
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

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->data_updated)
		return false;

	return dmgr->data_updated(pre_data, new_data);
}

static bool _item_locked(Elm_Object_Item *it, bool *locked)
{
	struct datamgr *dmgr;

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->data_locked)
		return false;

	if (!dmgr->data_locked(it, locked)) {
		_ERR("Get lock status failed.");
		return false;
	}

	return true;
}

static bool _load_item_banner(Evas_Object *banner, void *data)
{
	struct datamgr *dmgr;
	char *title, *subtitle, *description;

	if (!banner || !data) {
		_ERR("Invaild argument.");
		return false;
	}

	dmgr = get_channel_datamgr();
	if (!dmgr || !dmgr->get_data)
		return false;

	elm_layout_file_set(banner, EDJEFILE, GRP_TV_BANNER);

	title = dmgr->get_data(data, DATA_NAME);
	if (title)
		elm_object_part_text_set(banner, PART_TITLE, title);

	subtitle = dmgr->get_data(data, DATA_SUBTITLE);
	if (subtitle)
		elm_object_part_text_set(banner, PART_SUBTITLE, subtitle);

	description = dmgr->get_data(data, DATA_DESCRIPTION);
	if (description)
		elm_object_part_text_set(banner, PART_DESCRIPT, description);

	return true;
}

static struct grid_data _gdata = {
	.id = GRID_TV,
	.item_size_x = ITEM_TV_X,
	.item_size_y = ITEM_TV_Y,
	.gclass = &_gclass,
	.create_item_list = _create_item_list,
	.destroy_item_list = _destroy_item_list,
	.item_action = _item_action,
	.item_updated = _item_updated,
	.item_locked = _item_locked,
	.load_item_banner = _load_item_banner,
};

struct grid_data *get_tv_grid_data(void)
{
	return &_gdata;
}
