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

#define STYLE_GALLERY "style.gallery"
#define ITEM_GALLERY_X (200 + 6)
#define ITEM_GALLERY_Y (200 + 6)

static char *_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *duration = NULL;

	if (!data) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	duration = dmgr->get_data(data, DATA_DURATION);
	if (!duration)
		return NULL;

	return duration;
}

static Evas_Object *_content_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	Evas_Object *img;
	char *img_path;

	if (!data || !obj) {
		_ERR("Invalid argument.");
		return NULL;
	}

	dmgr = get_media_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	if (!strcmp(part, PART_THUMB_ICON)) {
		img = elm_image_add(obj);
		if (!img) {
			_ERR("elm_image_add failed.");
			return NULL;
		}

		img_path = dmgr->get_data(data, DATA_THUMBNAIL);
		if (!img_path) {
			img_path = DEFAULT_IMAGE_PNG;
			elm_image_resizable_set(img, EINA_FALSE, EINA_FALSE);
		}

		elm_image_file_set(img, img_path, NULL);
		elm_image_aspect_fixed_set(img, EINA_FALSE);
		evas_object_show(img);

		return img;
	} else if (!strcmp(part, PART_THUMB_PLAY_ICON) &&
			dmgr->get_data(data, DATA_DURATION)) {
		img = elm_image_add(obj);
		if (!img) {
			_ERR("elm_image_add failed.");
			return NULL;
		}

		elm_image_file_set(img, PATH_PLAY_ICON_PNG, NULL);
		elm_image_aspect_fixed_set(img, EINA_FALSE);
		evas_object_show(img);

		return img;
	}

	return NULL;
}

static struct grid_class _gclass = {
	.item_style = STYLE_GALLERY,
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

	return dmgr->get_favorites(ITEM_GALLERY);
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
	.id = GRID_GALLERY,
	.item_size_x = ITEM_GALLERY_X,
	.item_size_y = ITEM_GALLERY_Y,
	.gclass = &_gclass,
	.create_item_list = _create_item_list,
	.destroy_item_list = _destroy_item_list,
	.item_action = _item_action,
	.item_updated = _item_updated,
};

struct grid_data *get_gallery_grid_data(void)
{
	return &_gdata;
}
