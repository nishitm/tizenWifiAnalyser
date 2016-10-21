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
#include <tv_service_proxy_channel_info.h>
#include <app_debug.h>
#include <app_define.h>

#include "define.h"
#include "datamgr.h"
#include "utils.h"

struct channel_data {
	int id;
	char *name;
	long major;
	long minor;
};

static void _channel_favorite_list_foreach(gpointer data, gpointer user_data)
{
	struct channel_data *cdata;
	TvServiceChannel *channel;
	Eina_List **l;

	if (!data || !user_data) {
		_ERR("Data is NULL.");
		return;
	}

	channel = (TvServiceChannel *)data;
	l = (Eina_List **)user_data;

	cdata = calloc(1, sizeof(*cdata));
	if (!cdata) {
		_ERR("Calloc failed.");
		return;
	}

	cdata->id = channel->service_id;
	cdata->major = channel->major;
	cdata->minor = channel->minor;

	cdata->name = strdup(channel->program_name);

	*l = eina_list_append(*l, cdata);
}

static Eina_List *_get_favorites(enum item_type type)
{
	GList *id_list = NULL;
	Eina_List *l = NULL;
	int r;

	if (type != ITEM_CHANNEL)
		return NULL;

	r = tv_service_channel_info_create();
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service channel info create failed.");
		return NULL;
	}

	r = tv_service_get_channel_list(TV_SERVICE_CHANNEL_MODE_FAVORITE,
			TV_SERVICE_ANTENNA_TYPE_ALL, &id_list);
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service Get channel list failed.");
		tv_service_channel_info_destroy();
		return NULL;
	}

	if (!id_list) {
		tv_service_channel_info_destroy();
		return NULL;
	}

	g_list_foreach(id_list, _channel_favorite_list_foreach, &l);

	tv_service_free_channel_list(id_list);
	tv_service_channel_info_destroy();

	return l;
}

static char *_get_channel_title(struct channel_data *cdata)
{
	char str[SIZE_STR];

	if (cdata->minor > 0)
		snprintf(str, sizeof(str), "%ld-%ld %s", cdata->major,
				cdata->minor, cdata->name);
	else
		snprintf(str, sizeof(str), "%ld %s", cdata->major, cdata->name);

	return strdup(str);
}

static char *_get_data(void *data, enum data_type type)
{
	struct channel_data *cdata;

	if (!data) {
		_ERR("Invalid argument.");
		return NULL;
	}
	cdata = data;

	switch (type) {
	case DATA_NAME:
		return _get_channel_title(cdata);

	case DATA_THUMBNAIL:
		/* It will fixed after tv-service's implementation */
		return NULL;

	case DATA_SUBTITLE:
		/* It will be implemented as GUI guide */
		return NULL;

	case DATA_DESCRIPTION:
		/* It will be implemented as GUI guide */
		return NULL;

	default:
		_ERR("Invalid data type.");
		return NULL;
	}
}

static void _free_favorites(Eina_List *list)
{
	struct channel_data *cdata;

	if (!list)
		return;

	EINA_LIST_FREE(list, cdata) {
		if (!cdata)
			continue;

		free(cdata->name);
		free(cdata);

		cdata = NULL;
	}
}

static bool _unset_favorite(struct channel_data *cdata)
{
	int r;

	r = tv_service_channel_info_create();
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service channel info create failed.");
		return false;
	}

	r = tv_service_delete_favorite_channel(cdata->id);
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service delete favorite channel failed.");
		tv_service_channel_info_destroy();
		return false;
	}

	tv_service_channel_info_destroy();

	return true;
}

static bool _update_lock(struct channel_data *cdata, void *data)
{
	TvServiceChannel channel;
	char *pincode;
	int r;

	if (!data)
		return false;

	pincode = data;

	r = tv_service_channel_info_create();
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service channel info create failed.");
		return false;
	}

	r = tv_service_get_channel(cdata->id, &channel);
	if (r != TVS_ERROR_OK) {
		tv_service_channel_info_destroy();
		return false;
	}

	if (channel.locked)
		r = tv_service_unlock_channel(cdata->id, pincode);
	else
		r = tv_service_lock_channel(cdata->id, pincode);

	tv_service_channel_info_destroy();

	if (r < 0)
		return false;

	return true;
}

static bool _action(Elm_Object_Item *it, enum action_type type, void *data)
{
	struct channel_data *cdata;
	char str[SIZE_STR];

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	cdata = elm_object_item_data_get(it);
	if (!cdata)
		return false;

	switch (type) {
	case ACTION_LAUNCH:
		snprintf(str, sizeof(str), "%d", cdata->id);

		if (!utils_launch_app(APP_ID_LIVETV, PARAM_SERVICE_ID, str)) {
			_ERR("Livetv launch failed.");
			return false;
		}

		break;

	case ACTION_UPDATE_FAVORITE:
		if (!_unset_favorite(cdata)) {
			_ERR("Unset favorite channel failed.");
			return false;
		}

		break;

	case ACTION_UPDATE_LOCK:
		if (!_update_lock(cdata, data)) {
			_ERR("Update lock status failed.");
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
	struct channel_data *cdata1, *cdata2;

	cdata1 = data1;
	cdata2 = data2;

	if (cdata1->id == cdata2->id)
		return false;

	return true;
}

static bool _data_locked(Elm_Object_Item *it, bool *locked)
{
	struct channel_data *cdata;
	TvServiceChannel channel;
	int r;

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	cdata = elm_object_item_data_get(it);
	if (!cdata)
		return false;

	r = tv_service_channel_info_create();
	if (r != TVS_ERROR_OK) {
		_ERR("Tv Service channel info create failed.");
		return false;
	}

	r = tv_service_get_channel(cdata->id, &channel);
	if (r != TVS_ERROR_OK) {
		tv_service_channel_info_destroy();
		return false;
	}

	*locked = channel.locked;

	tv_service_channel_info_destroy();

	return true;
}

static struct datamgr _dmgr = {
	.get_data = _get_data,
	.get_count = NULL,
	.get_favorites = _get_favorites,
	.free_favorites = _free_favorites,
	.action = _action,
	.data_updated = _data_updated,
	.data_locked = _data_locked,
};

struct datamgr *get_channel_datamgr(void)
{
	return &_dmgr;
}
