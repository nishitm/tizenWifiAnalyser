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

#include <web/web_bookmark.h>
#include <app_debug.h>

#include "datamgr.h"

struct web_data {
	char *title;
	char *snapshot_path;
};

static Eina_List *_get_favorites(enum item_type type)
{
	Eina_List *list = NULL;
	int *ids = NULL;
	int r, ids_count, i, width, height, size;
	char *title;
	unsigned char *path;
	struct web_data *wdata;

	if (type != ITEM_WEBS)
		return NULL;

	r = bp_bookmark_adaptor_initialize();
	if (r != 0) {
		_ERR("Bookmark adaptor initialize failed.");
		return NULL;
	}

	r = bp_bookmark_adaptor_get_ids_p(&ids, &ids_count,
			-1, 0, -1, -1, -1, -1, BP_BOOKMARK_O_SEQUENCE, 0);
	if (r != 0) {
		_ERR("Bookmark adaptor get ids failed.");
		return NULL;
	}

	for (i = 0; i < ids_count; i++) {
		r = bp_bookmark_adaptor_get_title(ids[i], &title);
		if (r != 0) {
			_ERR("Bookmark adaptor get title failed.");
			continue;
		}

		r = bp_bookmark_adaptor_get_snapshot(ids[i], &width, &height,
				&path, &size);
		if (r != 0) {
			_ERR("Bookmark adaptor get snapshot failed.");
			continue;
		}

		wdata = calloc(1, sizeof(*wdata));
		if (!wdata) {
			_ERR("Calloc failed.");
			continue;
		}

		if (title)
			wdata->title = title;
		if (path)
			wdata->snapshot_path = strdup((const char *)path);

		list = eina_list_append(list, wdata);
	}

	free(ids);
	bp_bookmark_adaptor_deinitialize();

	return list;
}

static char *_get_data(void *data, enum data_type type)
{
	struct web_data *wdata;

	if (!data) {
		_ERR("Invalid argument.");
		return NULL;
	}
	wdata = data;

	switch (type) {
	case DATA_NAME:
		return wdata->title;

	case DATA_THUMBNAIL:
		return wdata->snapshot_path;

	default:
		_ERR("Invalid data type.");
		return NULL;
	}
}

static void _free_favorites(Eina_List *list)
{
	struct web_data *wdata;

	if (!list)
		return;

	EINA_LIST_FREE(list, wdata) {
		if (!wdata)
			continue;

		free(wdata->title);
		free(wdata->snapshot_path);
		free(wdata);

		wdata = NULL;
	}
}

static bool _action(Elm_Object_Item *it, enum action_type type, void *data)
{
	/* It will be implemented later. */

	return true;
}

static struct datamgr _dmgr = {
	.get_data = _get_data,
	.get_count = NULL,
	.get_favorites = _get_favorites,
	.free_favorites = _free_favorites,
	.action = _action,
};

struct datamgr *get_webs_datamgr(void)
{
	return &_dmgr;
}
