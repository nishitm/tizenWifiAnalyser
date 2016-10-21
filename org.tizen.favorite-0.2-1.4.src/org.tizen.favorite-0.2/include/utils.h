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

#ifndef __AIR_FAVORITE_UTILS_H__
#define __AIR_FAVORITE_UTILS_H__

Evas_Object *utils_add_window(const char *name);
Evas_Object *utils_add_layout(Evas_Object *parent, const char *group,
		Eina_Bool resize);
Evas_Object *utils_add_gengrid(Evas_Object *parent,
		int item_size_x, int item_size_y);
Evas_Object *utils_add_box(Evas_Object *parent, const char *part,
		Eina_Bool horizontal, Evas_Coord padding_h,
		Evas_Coord padding_v);
Evas_Object *utils_add_button(Evas_Object *parent, const char *part,
		const char *text, const char *style);
Evas_Object *utils_add_table(Evas_Object *table, const char *part,
		int padding_x, int padding_y);
Evas_Object *utils_add_notify(Evas_Object *parent, const char *text,
		const char *noti_style, const char *label_style,
		int timeout);
bool utils_launch_app(const char *appid, const char *key, const char *value);

#endif /* __AIR_FAVORITE_UTILS_H__ */
