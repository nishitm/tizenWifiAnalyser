/* Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <viewmgr.h>
#include <inputmgr.h>
#include <app_contents.h>
#include <app_define.h>
#include <app_debug.h>

#include "define.h"
#include "utils.h"
#include "view.h"
#include "grid.h"
#include "datamgr.h"

enum button_type {
	BTN_LIVETV = 0,
	BTN_FAVORITE,
	BTN_LOCK,
};

struct _priv {
	Evas_Object *win;
	Evas_Object *base;
	Evas_Object *live_btn;
	Evas_Object *menu_btn[COUNT_ACTION];
	Evas_Object *banner;
	int size_menu;
	struct grid_data *gdata;
};

struct _action_info {
	int id;
	const char *title;
	const char *style;
};

static struct _action_info action_info[] = {
	{
		.id = BTN_FAVORITE,
		.title = STR_FAVORITE,
		.style = STYLE_ACTION_MENU_FAV_BTN
	},
	{
		.id = BTN_LOCK,
		.title = STR_LOCK,
		.style = STYLE_ACTION_MENU_LOCK_BTN
	}
};

static void _mouse_move_cb(int id, void *data, Evas *e, Evas_Object *obj,
		Evas_Event_Mouse_Move *ev)
{
	if (!obj) {
		_ERR("Invalid argument.");
		return;
	}

	if (!elm_object_focus_get(obj))
		elm_object_focus_set(obj, EINA_TRUE);
}

static void _livetv_selected(struct _priv *priv)
{
	if (!utils_launch_app(APP_ID_LIVETV, NULL, NULL)) {
		_ERR("Launch live tv failed.");
		return;
	}

	elm_object_signal_emit(priv->base, SIG_HIDE, SRC_EDC);
}

static void _favorite_selected(struct _priv *priv)
{
	struct grid_data *gdata;

	gdata = priv->gdata;

	if (!gdata->item_action(gdata->focused_item,
				ACTION_UPDATE_FAVORITE, NULL)) {
		_ERR("Unfavorite failed.");
		return;
	}

	viewmgr_update_view(VIEW_BASE, UPDATE_GRID, gdata);
	viewmgr_update_view(VIEW_BASE, UPDATE_SHOW_TOAST, STR_REMOVE_FAVORITE);
	elm_object_signal_emit(priv->base, SIG_HIDE, SRC_EDC);
}

static void _lock_selected(struct grid_data *gdata)
{
	if (!gdata->item_action || !gdata->item_locked)
		return;

	viewmgr_update_view(VIEW_PIN, UPDATE_DATA, gdata);
	viewmgr_show_view(VIEW_PIN);
}

static void _clicked_cb(int id, void *data, Evas_Object *obj)
{
	struct _priv *priv;

	if (!data)
		return;
	priv = data;

	if (!priv->gdata)
		return;

	switch (id) {
	case BTN_LIVETV:
		_livetv_selected(priv);
		break;

	case BTN_FAVORITE:
		_favorite_selected(priv);
		break;

	case BTN_LOCK:
		_lock_selected(priv->gdata);
		break;

	default:
		_ERR("Unhandled action type.");
	}
}

static void _key_down_cb(int id, void *data, Evas *e, Evas_Object *obj,
		Evas_Event_Key_Down *ev)
{
	struct _priv *priv;

	if (!data || !ev) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	if (!strcmp(ev->keyname, KEY_BACK)
			|| !strcmp(ev->keyname, KEY_ESC))
		elm_object_signal_emit(priv->base, SIG_HIDE, SRC_EDC);
}

static input_handler _btn_input_handler = {
	.mouse_move = _mouse_move_cb,
	.clicked = _clicked_cb,
	.key_down = _key_down_cb
};

static bool _draw_livetv_area(struct _priv *priv)
{
	Evas_Object *btn;

	btn = utils_add_button(priv->base,
			PART_LIVETV_BTN, STR_LIVETV, STYLE_LIVETV_BTN);
	if (!btn) {
		_ERR("Add button failed.");
		return false;
	}

	inputmgr_add_callback(btn, BTN_LIVETV, &_btn_input_handler, priv);

	priv->live_btn = btn;

	return true;
}

static void _set_focus_policy(struct _priv *priv)
{
	int i;

	elm_object_focus_next_object_set(priv->live_btn, priv->live_btn,
			ELM_FOCUS_UP);
	elm_object_focus_next_object_set(priv->live_btn, priv->live_btn,
			ELM_FOCUS_LEFT);
	elm_object_focus_next_object_set(priv->live_btn, priv->live_btn,
			ELM_FOCUS_RIGHT);
	elm_object_focus_next_object_set(priv->live_btn, priv->menu_btn[0],
			ELM_FOCUS_DOWN);

	elm_object_focus_next_object_set(priv->menu_btn[0], priv->menu_btn[0],
			ELM_FOCUS_LEFT);

	elm_object_focus_next_object_set(priv->menu_btn[priv->size_menu - 1],
			 priv->menu_btn[priv->size_menu - 1], ELM_FOCUS_RIGHT);

	for (i = 0; i < priv->size_menu; i++)
		elm_object_focus_next_object_set(priv->menu_btn[i],
				priv->menu_btn[i], ELM_FOCUS_DOWN);
}

static void _set_action_state(int idx, Evas_Object *btn, struct _priv *priv)
{
	struct grid_data *gdata;
	bool locked;

	if (!priv->gdata)
		return;

	gdata = priv->gdata;

	if (action_info[idx].id == BTN_LOCK) {
		/* Channel can only be locked */
		if (!strcmp(gdata->id, GRID_TV) && gdata->item_locked) {
			gdata->item_locked(gdata->focused_item, &locked);

			if (locked)
				elm_object_signal_emit(btn, SIG_TOGGLE,
						SRC_ACTION_BTN);
		}
	}
}

static bool _draw_action_area(struct _priv *priv)
{
	Evas_Object *table, *btn;
	int i, row = 0, col = 0;

	table = utils_add_table(priv->base, PART_ACTION_MENU,
			SIZE_ACTION_TABLE_BORDER, SIZE_ACTION_TABLE_BORDER);
	if (!table) {
		_ERR("Add table failed.");
		return false;
	}

	if (priv->gdata->item_locked)
		priv->size_menu = COUNT_ACTION;
	else
		priv->size_menu = COUNT_ACTION - 1;

	for (i = 0; i < priv->size_menu; i++) {
		btn = utils_add_button(priv->base, NULL, action_info[i].title,
				action_info[i].style);
		if (!btn) {
			_ERR("Add button failed.");
			evas_object_del(table);
			return false;
		}

		inputmgr_add_callback(btn, action_info[i].id,
				&_btn_input_handler, priv);

		_set_action_state(i, btn, priv);

		col = i % SIZE_COL_MAX;
		row = i / SIZE_COL_MAX;

		elm_table_pack(table, btn, col, row, 1, 1);

		priv->menu_btn[i] = btn;
	}

	_set_focus_policy(priv);

	return true;
}

static bool _draw_banner_area(struct _priv *priv)
{
	Evas_Object *banner;
	void *data;

	data = elm_object_item_data_get(priv->gdata->focused_item);
	if (!data)
		return false;

	banner = utils_add_layout(priv->base, NULL, EINA_FALSE);
	if (!banner) {
		_ERR("Add layout failed.");
		return false;
	}

	if (!priv->gdata->load_item_banner ||
			!priv->gdata->load_item_banner(banner, data))
		return false;

	elm_object_part_content_set(priv->base, PART_BANNER, banner);

	priv->banner = banner;

	return true;
}

static void _hide_done_cb(void *data, Evas_Object *obj,
		const char *emmision, const char *source)
{
	viewmgr_hide_view(VIEW_ACTION_MENU);
}

static Evas_Object *_create(Evas_Object *win, void *data)
{
	struct _priv *priv;
	Evas_Object *base;

	if (!win) {
		_ERR("Get window object failed.");
		return NULL;
	}

	priv = calloc(1, sizeof(*priv));
	if (!priv) {
		_ERR("Calloc failed.");
		return NULL;
	}

	base = utils_add_layout(win, GRP_VIEW_ACTION_MENU, EINA_TRUE);
	if (!base) {
		_ERR("Add layout failed.");
		free(priv);
		return NULL;
	}

	priv->win = win;
	priv->base = base;

	if (!viewmgr_set_view_data(VIEW_ACTION_MENU, priv)) {
		_ERR("Set view data failed.");
		evas_object_del(base);
		free(priv);
		return NULL;
	}

	elm_object_signal_callback_add(base, SIG_HIDE_DONE, SRC_EDC,
			_hide_done_cb, NULL);

	_draw_livetv_area(priv);

	return base;
}

static void _show(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	if (priv->base)
		evas_object_show(priv->base);

	_draw_action_area(priv);
	_draw_banner_area(priv);

	elm_object_signal_emit(priv->base, SIG_SHOW, SRC_EDC);
	elm_object_focus_set(priv->live_btn, EINA_TRUE);
}

static void _hide(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	if (priv->banner)
		evas_object_del(priv->banner);

	if (priv->base)
		evas_object_hide(priv->base);
}

static void _update(void *view_data, int update_type, void *data)
{
	struct _priv *priv;

	if (!view_data) {
		_ERR("Get data failed.");
		return;
	}
	priv = view_data;

	if (update_type == UPDATE_DATA) {
		if (!data)
			return;

		priv->gdata = data;
	}
}

static void _destroy(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	if (priv->base)
		evas_object_del(priv->base);

	free(priv);
}

static view_class _vclass = {
	.view_id = VIEW_ACTION_MENU,
	.create = _create,
	.show = _show,
	.hide = _hide,
	.update = _update,
	.destroy = _destroy,
};

view_class *view_action_menu_get_vclass(void)
{
	return &_vclass;
}
