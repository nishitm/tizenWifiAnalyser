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

#ifndef __AIR_FAVORITE_GRID_H__
#define __AIR_FAVORITE_GRID_H__

#include "datamgr.h"

struct grid_data {
	const char *id;
	Evas_Object *grid;
	int item_size_x;
	int item_size_y;
	Eina_List *list;
	struct grid_class *gclass;
	Elm_Object_Item *focused_item;

	Eina_List *(*create_item_list)(void);
	void (*destroy_item_list)(Eina_List *list);
	bool (*item_action)(Elm_Object_Item *it,
			enum action_type type, void *data);
	bool (*item_updated)(void *pre_data, void *new_data);
	bool (*item_locked)(Elm_Object_Item *it, bool *locked);
	bool (*load_item_banner)(Evas_Object *banner, void *data);
};

struct grid_data *get_tv_grid_data(void);
struct grid_data *get_movie_grid_data(void);
struct grid_data *get_gallery_grid_data(void);
struct grid_data *get_music_grid_data(void);
struct grid_data *get_apps_grid_data(void);
struct grid_data *get_webs_grid_data(void);

#endif /* __AIR_FAVORITE_GRID_H__ */
