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
#include <stdlib.h>
#include <pkgmgr-info.h>
#include <app_contents.h>
#include <app_debug.h>
#include <app_define.h>

#include "datamgr.h"
#include "define.h"
#include "utils.h"

struct app_data {
	char *id;
	char *name;
	char *icon;
};

static void _app_favorite_list_foreach(gpointer data, gpointer user_data)
{
	struct app_data *adata;
	pkgmgrinfo_appinfo_h pah;
	char *id, *name, *icon;
	Eina_List **l;

	if (!data || !user_data) {
		_ERR("Data is NULL.");
		return;
	}

	id = (char *)data;
	l = (Eina_List **)user_data;

	if (pkgmgrinfo_appinfo_get_usr_appinfo(id,
				getuid(), &pah) != PMINFO_R_OK) {
		_ERR("Get appinfo handle failed.");
		return;
	}

	if (pkgmgrinfo_appinfo_get_label(pah, &name) != PMINFO_R_OK) {
		_ERR("Get app name failed.");
		goto end;
	}

	if (pkgmgrinfo_appinfo_get_icon(pah, &icon) != PMINFO_R_OK) {
		_ERR("Get app icon failed.");
		goto end;
	}

	adata = calloc(1, sizeof(*adata));
	if (!adata) {
		_ERR("Calloc failed.");
		goto end;
	}

	if (id)
		adata->id = strdup(id);

	/* FIXME: The browser's name and icon is defined as GUI. */
	if (!strcmp(id, APP_ID_BROWSER)) {
		adata->name = strdup(STR_BROWSER);
		adata->icon = strdup(BROWSER_ICON_PNG);
	} else {
		if (name)
			adata->name = strdup(name);
		if (icon)
			adata->icon = strdup(icon);
	}

	*l = eina_list_append(*l, adata);

end:
	pkgmgrinfo_appinfo_destroy_appinfo(pah);
}

static Eina_List *_get_favorites(enum item_type type)
{
	GList *id_list = NULL;
	Eina_List *l = NULL;

	if (type != ITEM_APPS)
		return NULL;

	if (app_contents_get_favorite_list(CONTENTS_APP, &id_list)
			!= APP_CONTENTS_ERROR_NONE) {
		_ERR("Get app favorite id list failed.");
		return NULL;
	}

	if (!id_list)
		return NULL;

	g_list_foreach(id_list, _app_favorite_list_foreach, &l);

	app_contents_free_favorite_list(id_list);

	return l;
}

static char *_get_data(void *data, enum data_type type)
{
	struct app_data *adata;

	if (!data) {
		_ERR("Invalid argument.");
		return NULL;
	}
	adata = data;

	switch (type) {
	case DATA_NAME:
		return adata->name;

	case DATA_THUMBNAIL:
		return adata->icon;

	default:
		_ERR("Invalid data type.");
		return NULL;
	}
}

static void _free_favorites(Eina_List *list)
{
	struct app_data *adata;

	if (!list)
		return;

	EINA_LIST_FREE(list, adata) {
		if (!adata)
			continue;

		free(adata->id);
		free(adata->name);
		free(adata->icon);
		free(adata);

		adata = NULL;
	}
}

static bool _action(Elm_Object_Item *it, enum action_type type, void *data)
{
	struct app_data *adata;
	int r;

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	adata = elm_object_item_data_get(it);
	if (!adata || !adata->id)
		return false;

	switch (type) {
	case ACTION_LAUNCH:
		if (!utils_launch_app(adata->id, NULL, NULL)) {
			_ERR("Launch %s failed.", adata->id);
			return false;
		}

		break;

	case ACTION_UPDATE_FAVORITE:
		r = app_contents_favorite_set(CONTENTS_APP, adata->id, false);
		if (r != APP_CONTENTS_ERROR_NONE) {
			_ERR("Favorite unset failed.");
			return false;
		}

		break;

	default:
		_ERR("Unhandled action type.");
		return false;
	}

	return true;
}

static bool _data_updated(void *data1, void *data2)
{
	struct app_data *adata1, *adata2;

	adata1 = data1;
	adata2 = data2;

	if (!strcmp(adata1->id, adata2->id))
		return false;

	return true;
}

static struct datamgr _dmgr = {
	.get_data = _get_data,
	.get_count = NULL,
	.get_favorites = _get_favorites,
	.free_favorites = _free_favorites,
	.action = _action,
	.data_updated = _data_updated,
};

struct datamgr *get_apps_datamgr(void)
{
	return &_dmgr;
}
