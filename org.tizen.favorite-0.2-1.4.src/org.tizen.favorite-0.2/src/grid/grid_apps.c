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

#define STYLE_APPS "style.apps"
#define ITEM_APPS_X (212 + 26)
#define ITEM_APPS_Y (294 + 26)

static char *_text_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *name = NULL;

	if (!data) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_apps_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	name = dmgr->get_data(data, DATA_NAME);
	if (!name)
		return NULL;

	if ((!strcmp(name, STR_BROWSER)
				&& !strcmp(part, PART_THUMB_TEXT_BR))
			|| (strcmp(name, STR_BROWSER)
				&& !strcmp(part, PART_THUMB_TEXT)))
		return strdup(name);

	return NULL;
}

static Evas_Object *_content_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	Evas_Object *img, *rect = NULL;
	struct color_data bright, dark;
	char *icon, *name, *img_path;
	int w, h;

	if (!data || !obj) {
		_ERR("Data is NULL.");
		return NULL;
	}

	dmgr = get_apps_datamgr();
	if (!dmgr || !dmgr->get_data)
		return NULL;

	if (!strcmp(part, PART_THUMB_ICON)) {
		icon = dmgr->get_data(data, DATA_THUMBNAIL);

		img = elm_image_add(obj);
		if (!img) {
			_ERR("elm_image_add failed.");
			return NULL;
		}

		if (!icon || !strcmp(icon, STR_NULL))
			img_path = DEFAULT_APP_ICON_PNG;
		else
			img_path = icon;

		if (!elm_image_file_set(img, img_path, NULL))
			elm_image_file_set(img, DEFAULT_APP_ICON_PNG, NULL);

		elm_image_object_size_get(img, &w, &h);

		if (w <= SIZE_APP_ICON_MAX && h <= SIZE_APP_ICON_MAX)
			elm_image_resizable_set(img, EINA_FALSE, EINA_FALSE);

		return img;
	} else if (!strcmp(part, PART_THUMB_BG)) {
		name = dmgr->get_data(data, DATA_NAME);

		rect = evas_object_rectangle_add(obj);
		if (!rect) {
			_ERR("elm_rectangle_add failed.");
			return NULL;
		}

		if (!strcmp(name, STR_BROWSER)) {
			evas_object_color_set(rect, COLOR_BR_R, COLOR_BR_G,
					COLOR_BR_B, COLOR_BR_A);
			return rect;
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

		if (!strcmp(name, STR_BROWSER)) {
			evas_object_color_set(rect, COLOR_BR_R, COLOR_BR_G,
					COLOR_BR_B, COLOR_BR_A);
			return rect;
		}

		app_contents_get_color(name, &bright, &dark);
		evas_object_color_set(rect, dark.r, dark.g, dark.b, dark.a);

		return rect;
	}

	return NULL;
}

static Eina_Bool _state_get(void *data, Evas_Object *obj, const char *part)
{
	struct datamgr *dmgr;
	char *name;

	if (!data) {
		_ERR("Data is NULL.");
		return EINA_FALSE;
	}

	dmgr = get_apps_datamgr();
	if (!dmgr || !dmgr->get_data)
		return EINA_FALSE;

	name = dmgr->get_data(data, DATA_NAME);
	if (!name)
		return EINA_FALSE;

	if (!strcmp(name, STR_BROWSER))
		return EINA_TRUE;

	return EINA_FALSE;
}

static struct grid_class _gclass = {
	.item_style = STYLE_APPS,
	.text_get = _text_get,
	.content_get = _content_get,
	.state_get = _state_get,
	.del = NULL,
};

static Eina_List *_create_item_list(void)
{
	struct datamgr *dmgr;

	dmgr = get_apps_datamgr();
	if (!dmgr || !dmgr->get_favorites)
		return NULL;

	return dmgr->get_favorites(ITEM_APPS);
}

static void _destroy_item_list(Eina_List *list)
{
	struct datamgr *dmgr;

	if (!list)
		return;

	dmgr = get_apps_datamgr();
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

	dmgr = get_apps_datamgr();
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

	dmgr = get_apps_datamgr();
	if (!dmgr || !dmgr->data_updated)
		return false;

	return dmgr->data_updated(pre_data, new_data);
}

static struct grid_data _gdata = {
	.id = GRID_APP,
	.item_size_x = ITEM_APPS_X,
	.item_size_y = ITEM_APPS_Y,
	.gclass = &_gclass,
	.create_item_list = _create_item_list,
	.destroy_item_list = _destroy_item_list,
	.item_action = _item_action,
	.item_updated = _item_updated,
};

struct grid_data *get_apps_grid_data(void)
{
	return &_gdata;
}
