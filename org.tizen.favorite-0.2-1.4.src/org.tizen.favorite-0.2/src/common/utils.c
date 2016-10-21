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
#include <app_control.h>
#include <app_debug.h>

#include "define.h"
#include "datamgr.h"

Evas_Object *utils_add_window(const char *name)
{
	Evas_Object *win;

	if (!name) {
		_ERR("Invalid argument.");
		return NULL;
	}

	win = elm_win_add(NULL, name, ELM_WIN_BASIC);
	if (!win) {
		_ERR("elm_win_add failed.");
		return NULL;
	}

	elm_win_title_set(win, name);
	elm_win_focus_highlight_enabled_set(win, EINA_TRUE);

	evas_object_show(win);

	return win;
}

Evas_Object *utils_add_layout(Evas_Object *parent, const char *group,
		Eina_Bool resize)
{
	Evas_Object *layout;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	layout = elm_layout_add(parent);
	if (!layout) {
		_ERR("elm_layout_add failed.");
		return NULL;
	}

	if (group)
		elm_layout_file_set(layout, EDJEFILE, group);

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND,
			EVAS_HINT_EXPAND);

	if (resize)
		elm_win_resize_object_add(parent, layout);

	return layout;
}

Evas_Object *utils_add_gengrid(Evas_Object *parent,
		int item_size_x, int item_size_y)
{
	Evas_Object *grid;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	grid = elm_gengrid_add(parent);
	if (!grid) {
		_ERR("elm_gengrid_add failed.");
		return NULL;
	}

	evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND,
			EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);

	elm_gengrid_multi_select_set(grid, EINA_FALSE);
	elm_gengrid_horizontal_set(grid, EINA_TRUE);
	elm_gengrid_align_set(grid, 0.0, 0.0);
	elm_gengrid_select_mode_set(grid, ELM_OBJECT_SELECT_MODE_ALWAYS);
	elm_gengrid_item_size_set(grid, ELM_SCALE_SIZE(item_size_x),
			ELM_SCALE_SIZE(item_size_y));
	elm_scroller_policy_set(grid, ELM_SCROLLER_POLICY_OFF,
			ELM_SCROLLER_POLICY_OFF);

	return grid;
}

Evas_Object *utils_add_box(Evas_Object *parent, const char *part,
		Eina_Bool horizontal, Evas_Coord padding_h,
		Evas_Coord padding_v)
{
	Evas_Object *box;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	box = elm_box_add(parent);
	if (!box) {
		_ERR("elm_box_add failed.");
		return NULL;
	}

	elm_box_horizontal_set(box, horizontal);
	elm_box_padding_set(box, ELM_SCALE_SIZE(padding_h),
			ELM_SCALE_SIZE(padding_v));

	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND,
			EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

	if (part)
		elm_object_part_content_set(parent, part, box);

	evas_object_show(box);

	return box;
}

Evas_Object *utils_add_button(Evas_Object *parent, const char *part,
		const char *text, const char *style)
{
	Evas_Object *btn;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	btn = elm_button_add(parent);
	if (!btn) {
		_ERR("elm_button_add failed.");
		return NULL;
	}

	if (part)
		elm_object_part_content_set(parent, part, btn);
	if (text)
		elm_object_text_set(btn, text);
	if (style)
		elm_object_style_set(btn, style);

	evas_object_show(btn);

	return btn;
}

Evas_Object *utils_add_table(Evas_Object *parent, const char *part,
		int padding_x, int padding_y)
{
	Evas_Object *table;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	table = elm_table_add(parent);
	if (!table) {
		_ERR("elm_table_add failed.");
		return NULL;
	}

	elm_table_padding_set(table, ELM_SCALE_SIZE(padding_x),
			ELM_SCALE_SIZE(padding_y));
	evas_object_show(table);

	if (part)
		elm_object_part_content_set(parent, part, table);

	return table;
}

static void _notify_timeout_cb(void *data, Evas_Object *obj, void *ei)
{
	if (!obj)
		return;

	evas_object_del(obj);
}

Evas_Object *utils_add_notify(Evas_Object *parent, const char *text,
		const char *noti_style, const char *label_style, int timeout)
{
	Evas_Object *notify, *lbl;

	if (!parent) {
		_ERR("Invalid argument.");
		return NULL;
	}

	notify = elm_notify_add(parent);
	if (!notify) {
		_ERR("elm_notify_add failed.");
		return NULL;
	}

	evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND,
			EVAS_HINT_EXPAND);
	elm_notify_align_set(notify, 0.0, 1.0);
	elm_notify_timeout_set(notify, timeout);

	if (noti_style)
		elm_object_style_set(notify, noti_style);

	lbl = elm_label_add(notify);
	if (!lbl) {
		_ERR("elm_label_add failed.");
		evas_object_del(notify);
		return NULL;
	}

	elm_object_content_set(notify, lbl);
	evas_object_show(lbl);

	if (label_style)
		elm_object_style_set(lbl, label_style);

	if (text)
		elm_object_text_set(lbl, text);

	evas_object_smart_callback_add(notify, SIG_TIMEOUT,
			_notify_timeout_cb, NULL);
	evas_object_show(notify);

	return notify;
}

bool utils_launch_app(const char *appid, const char *key, const char *value)
{
	app_control_h app_ctrl;
	int r;

	if (!appid) {
		_ERR("Invalid argument.");
		return false;
	}

	r = app_control_create(&app_ctrl);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("App control handle create failed.");
		return false;
	}

	r = app_control_set_operation(app_ctrl, APP_CONTROL_OPERATION_DEFAULT);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("App control set operation failed.");
		goto err;
	}

	if (key && value) {
		r = app_control_add_extra_data(app_ctrl, key, value);
		if (r != APP_CONTROL_ERROR_NONE) {
			_ERR("App control set extra data failed.");
			goto err;
		}
	}

	r = app_control_set_app_id(app_ctrl, appid);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("App control set app id failed.");
		goto err;
	}

	r = app_control_send_launch_request(app_ctrl, NULL, NULL);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("App control send launch request failed.");
		goto err;
	}

	app_control_destroy(app_ctrl);

	return true;

err:
	app_control_destroy(app_ctrl);

	return false;
}
