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

#define STYLE_WEBS "style.webs"
#define ITEM_WEBS_X (378 + 26)
#define ITEM_WEBS_Y (294 + 26)

static char *_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *title = NULL;

	if (!data) {
		_ERR("Data is NULL");
		return NULL;
	}

	dmgr = get_webs_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	title = dmgr->get_data(data, DATA_NAME);
	if (!title)
		return NULL;

	return strdup(title);
}

static Evas_Object *_content_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	struct color_data bright, dark;
	Evas_Object *img, *rect;
	char *snapshot, *name;

	if (!data || !obj) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_webs_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	if (!strcmp(part, PART_THUMB_ICON)) {
		snapshot = dmgr->get_data(data, DATA_THUMBNAIL);
		if (!snapshot) {
			/* FIXME: The default snapshot image should be added. */
			return NULL;
		}

		img = elm_image_add(obj);
		if (!img) {
			_ERR("elm_image_add failed.");
			return NULL;
		}

		elm_image_file_set(img, snapshot, NULL);
		elm_image_aspect_fixed_set(img, EINA_FALSE);
		evas_object_show(img);

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
	.item_style = STYLE_WEBS,
	.text_get = _text_get,
	.content_get = _content_get,
	.state_get = NULL,
	.del = NULL,
};

static Eina_List *_create_item_list(void)
{
	struct datamgr *dmgr;

	dmgr = get_webs_datamgr();
	if (!dmgr || !dmgr->get_favorites)
		return NULL;

	return dmgr->get_favorites(ITEM_WEBS);
}

static void _destroy_item_list(Eina_List *list)
{
	struct datamgr *dmgr;

	if (!list)
		return;

	dmgr = get_webs_datamgr();
	if (!dmgr || !dmgr->free_favorites)
		return;

	dmgr->free_favorites(list);
}

static struct grid_data _gdata = {
	.id = GRID_WEB,
	.item_size_x = ITEM_WEBS_X,
	.item_size_y = ITEM_WEBS_Y,
	.gclass = &_gclass,
	.create_item_list = _create_item_list,
	.destroy_item_list = _destroy_item_list,
};

struct grid_data *get_webs_grid_data(void)
{
	return &_gdata;
}
