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
#include <media_content.h>
#include <app_contents.h>
#include <app_media.h>
#include <app_debug.h>
#include <app_define.h>

#include "define.h"
#include "datamgr.h"
#include "utils.h"

static void _media_favorite_list_foreach(gpointer data, gpointer user_data)
{
	app_media *am;
	media_info_h media;
	Eina_List **l;
	int ret;
	char *id;

	if (!data || !user_data) {
		_ERR("Data is NULL.");
		return;
	}

	id = (char *)data;
	l = (Eina_List **)user_data;

	ret = media_info_get_media_from_db(id, &media);
	if (ret != MEDIA_CONTENT_ERROR_NONE) {
		_ERR("Get media info handle failed.");
		return;
	}

	if (!media) {
		_ERR("Media handle is NULL.");
		return;
	}

	am = app_media_create(media);
	if (!am) {
		_ERR("app_media_create failed.");
		media_info_destroy(media);
		return;
	}

	*l = eina_list_append(*l, am);
}

static Eina_List *_get_favorites(enum item_type type)
{
	GList *id_list = NULL;
	Eina_List *l = NULL;
	enum app_contents_type contents_type;

	switch (type) {
	case ITEM_MOVIE:
		contents_type = CONTENTS_MOVIE;
		break;

	case ITEM_GALLERY:
		contents_type = CONTENTS_GALLERY;
		break;

	case ITEM_MUSIC:
		contents_type = CONTENTS_MUSIC;
		break;

	default:
		_ERR("Unhandled media type.");
		return NULL;
	}

	if (app_contents_get_favorite_list(contents_type, &id_list)
			!= APP_CONTENTS_ERROR_NONE) {
		_ERR("Get media favorite id list failed.");
		return NULL;
	}

	if (!id_list)
		return NULL;

	if (media_content_connect() != MEDIA_CONTENT_ERROR_NONE) {
		_ERR("Connect media content service failed.");
		return NULL;
	}

	g_list_foreach(id_list, _media_favorite_list_foreach, &l);

	media_content_disconnect();

	if (id_list)
		app_contents_free_favorite_list(id_list);

	return l;
}

static char *_get_video_duration(app_media_info *minfo)
{
	int duration, h, m, s, sec;
	char str[SIZE_STR];

	if (minfo->media_type != MEDIA_CONTENT_TYPE_VIDEO)
		return NULL;

	duration = minfo->video->duration;

	sec = duration / 1000;
	h = sec / 3600;
	m = (sec % 3600) / 60;
	s = sec % 60;

	if (h)
		snprintf(str, sizeof(str), "%d:%02d:%02d", h, m, s);
	else
		snprintf(str, sizeof(str), "%d:%02d", m, s);

	return strdup(str);
}

static char *_get_data(void *data, enum data_type type)
{
	app_media *am;
	app_media_info *minfo;

	if (!data) {
		_ERR("Invalid argument.");
		return NULL;
	}
	am = data;

	minfo = app_media_get_info(am);
	if (!minfo) {
		_ERR("Get app_media_info failed.");
		return NULL;
	}

	switch (type) {
	case DATA_NAME:
		return minfo->title;

	case DATA_THUMBNAIL:
		return minfo->thumbnail_path;

	case DATA_PATH:
		return minfo->file_path;

	case DATA_DURATION:
		if (!minfo->video)
			return NULL;
		return _get_video_duration(minfo);

	case DATA_ARTIST:
		if (!minfo->audio)
			return NULL;
		return minfo->audio->artist;

	case DATA_ALBUM_NAME:
		if (!minfo->audio)
			return NULL;
		return minfo->audio->album;

	default:
		_ERR("Unhandled data type.");
	}

	return NULL;
}

static int _get_count(enum item_type type)
{
	GList *id_list = NULL;
	int cnt;
	enum app_contents_type contents_type;

	if (type == ITEM_PHOTO)
		contents_type = CONTENTS_PHOTO;
	else if (type == ITEM_VIDEO)
		contents_type = CONTENTS_VIDEO;
	else
		return 0;

	if (app_contents_get_favorite_list(contents_type, &id_list)
			!= APP_CONTENTS_ERROR_NONE) {
		_ERR("Get favorite id list failed.");
		return 0;
	}

	cnt = g_list_length(id_list);

	if (id_list)
		app_contents_free_favorite_list(id_list);

	return cnt;
}

static void _free_favorites(Eina_List *list)
{
	app_media *am;

	if (!list)
		return;

	EINA_LIST_FREE(list, am) {
		if (!am)
			continue;

		app_media_destroy(am);
	}
}

static bool _action(Elm_Object_Item *it, enum action_type type, void *data)
{
	app_media *am;
	app_media_info *minfo;
	int r;

	if (!it) {
		_ERR("Invalid argument.");
		return false;
	}

	am = elm_object_item_data_get(it);
	if (!am)
		return false;

	minfo = app_media_get_info(am);
	if (!minfo) {
		_ERR("Get app_media_info failed.");
		return false;
	}

	if (!minfo->media_id)
		return false;

	switch (type) {
	case ACTION_LAUNCH:
		if (!utils_launch_app(APP_ID_MEDIAHUB, PARAM_MEDIA_ID,
					minfo->media_id)) {
			_ERR("Mediahub launch failed.");
			return false;
		}

		break;

	case ACTION_UPDATE_FAVORITE:
		r = app_contents_favorite_set(CONTENTS_MEDIA, minfo->media_id,
				false);
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
	app_media *am1, *am2;
	app_media_info *minfo1, *minfo2;

	am1 = data1;
	am2 = data2;

	minfo1 = app_media_get_info(am1);
	if (!minfo1) {
		_ERR("Get app_media_info failed.");
		return false;
	}

	minfo2 = app_media_get_info(am2);
	if (!minfo2) {
		_ERR("Get app_media_info failed.");
		return false;
	}

	if (!strcmp(minfo1->media_id, minfo2->media_id))
		return false;

	return true;
}

static struct datamgr _dmgr = {
	.get_data = _get_data,
	.get_count = _get_count,
	.get_favorites = _get_favorites,
	.free_favorites = _free_favorites,
	.action = _action,
	.data_updated = _data_updated,
};

struct datamgr *get_media_datamgr(void)
{
	return &_dmgr;
}
