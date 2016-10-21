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

#include <app.h>
#include <Elementary.h>
#include <viewmgr.h>
#include <inputmgr.h>
#include <gridmgr.h>
#include <app_debug.h>

#include "define.h"
#include "utils.h"
#include "grid.h"
#include "datamgr.h"
#include "view.h"

const char *str_count[] = {
	STR_CNT_TV,
	STR_CNT_MOVIE,
	STR_CNT_GALLERY,
	STR_CNT_MUSIC,
	STR_CNT_APPS,
	STR_CNT_WEBS,
	NULL
};

struct _priv {
	struct gridmgr *gmgr;
	Evas_Object *win;
	Evas_Object *base;
	Evas_Object *box;
	Evas_Object *menu[COUNT_MENU];
	Evas_Object *notify;
	int cur_menu_id;
};

static struct grid_data *(*_get_grid_data[COUNT_MENU])(void) = {
	get_tv_grid_data,
	get_movie_grid_data,
	get_gallery_grid_data,
	get_music_grid_data,
	get_apps_grid_data,
	get_webs_grid_data
};

static void _draw_content_count(Evas_Object *base, int count, int id)
{
	struct datamgr *dmgr;
	char str[SIZE_STR];

	snprintf(str, sizeof(str), "%d %s", count, str_count[id]);
	elm_object_part_text_set(base, PART_CONTENTS_COUNT, str);
	elm_object_part_text_set(base, PART_CONTENTS_SUBCOUNT, "");
	elm_object_signal_emit(base, SIG_INVISIBLE, PART_COUNT_DIVIDE_LINE);

	if (!strcmp(str_count[id], STR_CNT_GALLERY)) {
		dmgr = get_media_datamgr();
		if (!dmgr || !dmgr->get_count)
			return;

		snprintf(str, sizeof(str), "%d %s %d %s",
				dmgr->get_count(ITEM_PHOTO),
				STR_CNT_GALLERY_PHOTO,
				dmgr->get_count(ITEM_VIDEO),
				STR_CNT_GALLERY_VIDEO);
		elm_object_part_text_set(base, PART_CONTENTS_SUBCOUNT, str);
		elm_object_signal_emit(base, SIG_VISIBLE,
				PART_COUNT_DIVIDE_LINE);
	}
}

static bool _item_list_updated(struct grid_data *gdata,
		Eina_List *pre_list, Eina_List *new_list)
{
	int pre_cnt, new_cnt;
	void *pre_data, *new_data;
	int i;

	pre_cnt = eina_list_count(pre_list);
	new_cnt = eina_list_count(new_list);

	if (pre_cnt != new_cnt)
		return true;

	for (i = 0; i < new_cnt; i++) {
		pre_data = eina_list_nth(pre_list, i);
		new_data = eina_list_nth(new_list, i);

		if (gdata->item_updated(pre_data, new_data))
			return true;
	}

	return false;
}

static bool _update_current_grid(struct _priv *priv)
{
	struct grid_data *gdata;
	Evas_Object *prev_grid;
	Eina_List *pre_list;

	if (!priv->gmgr || !priv->base) {
		_ERR("Invalid argument.");
		return false;
	}

	gdata = _get_grid_data[priv->cur_menu_id]();
	if (!gdata || !gdata->id || !gdata->grid)
		return false;

	pre_list = gdata->list;
	gdata->list = gdata->create_item_list();

	elm_object_part_text_set(priv->base, PART_NO_CONTENTS, "");
	prev_grid = elm_object_part_content_unset(priv->base, PART_GENGRID);
	evas_object_hide(prev_grid);

	if (!gdata->list) {
		elm_object_part_text_set(priv->base,
				PART_NO_CONTENTS, STR_NO_CONTENTS);
	} else {
		elm_object_part_content_set(priv->base,
				PART_GENGRID, gdata->grid);
		evas_object_show(gdata->grid);

		if (_item_list_updated(gdata, pre_list, gdata->list))
			gridmgr_append_list(priv->gmgr, gdata->id, gdata->list);
	}

	_draw_content_count(priv->base, eina_list_count(gdata->list),
			priv->cur_menu_id);

	return true;
}

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

static void _menu_focused_cb(int id, void *data, Evas_Object *obj,
		Elm_Object_Item *item)
{
	struct _priv *priv;
	int pre_menu_id;

	if (!data || !obj) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	pre_menu_id = priv->cur_menu_id;

	if (pre_menu_id == id)
		return;

	if (priv->menu[pre_menu_id])
		elm_object_signal_emit(priv->menu[pre_menu_id],
				SIG_UNSELECTED, SRC_MENU_BTN);

	priv->cur_menu_id = id;

	if (!_update_current_grid(priv))
		_ERR("Load current grid view failed.");
}

static void _menu_key_down_cb(int id, void *data, Evas *e, Evas_Object *obj,
		Evas_Event_Key_Down *ev)
{
	struct grid_data *gdata;

	if (!ev) {
		_ERR("Invalid argument.");
		return;
	}

	if (!strcmp(ev->keyname, KEY_BACK)
			|| !strcmp(ev->keyname, KEY_ESC)) {
		ui_app_exit();
	} else if (!strcmp(ev->keyname, KEY_ENTER)
			|| !strcmp(ev->keyname, KEY_DOWN)) {
		gdata = _get_grid_data[id]();
		if (!gdata || !gdata->grid)
			return;

		elm_object_focus_set(gdata->grid, EINA_TRUE);
	}
}

static void _grid_mouse_move_cb(int id, void *data, Evas *e, Evas_Object *obj,
		Evas_Event_Mouse_Move *ev)
{
	Elm_Object_Item *it;

	if (!obj || !ev) {
		_ERR("Invalid argument.");
		return;
	}

	it = elm_gengrid_at_xy_item_get(obj, ev->cur.canvas.x,
			ev->cur.canvas.y, NULL, NULL);

	if (!it)
		return;

	if (!elm_object_item_focus_get(it))
		elm_object_item_focus_set(it, EINA_TRUE);
}

static void _grid_focused_cb(int id, void *data, Evas_Object *obj,
		Elm_Object_Item *item)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	elm_object_signal_emit(priv->menu[priv->cur_menu_id],
			SIG_SELECTED, SRC_MENU_BTN);
}

static void _grid_item_focused_cb(void *data, Elm_Object_Item *it,
		const char *emission, const char *source)
{
	struct grid_data *gdata;

	if (!data || !it) {
		_ERR("Invalid argument.");
		return;
	}
	gdata = data;

	gdata->focused_item = it;
}

static void _grid_item_selected_cb(void *data, Elm_Object_Item *it,
		const char *emission, const char *source)
{
	struct grid_data *gdata;

	if (!data || !it) {
		_ERR("Invalid argument.");
		return;
	}
	gdata = data;

	if (gdata->item_action)
		gdata->item_action(it, ACTION_LAUNCH, NULL);

	elm_gengrid_item_selected_set(it, EINA_FALSE);
}

static void _grid_realized_cb(int id, void *data, Evas_Object *obj,
		Elm_Object_Item *item)
{
	struct grid_data *gdata;

	if (!data || !item) {
		_ERR("Invalid argument.");
		return;
	}

	gdata = _get_grid_data[id]();
	if (!gdata || !gdata->id || !gdata->gclass) {
		_ERR("Get grid data failed.");
		return;
	}

	elm_object_item_signal_callback_add(item, SIG_ITEM_FOCUSED,
			SRC_EDC, _grid_item_focused_cb, gdata);
	elm_object_item_signal_callback_add(item, SIG_ITEM_SELECTED,
			SRC_EDC, _grid_item_selected_cb, gdata);
}

static void _grid_unrealized_cb(int id, void *data, Evas_Object *obj,
		Elm_Object_Item *item)
{
	if (!item) {
		_ERR("Invalid argument.");
		return;
	}

	elm_object_item_signal_callback_del(item, SIG_ITEM_FOCUSED,
			SRC_EDC, _grid_item_focused_cb);
	elm_object_item_signal_callback_del(item, SIG_ITEM_SELECTED,
			SRC_EDC, _grid_item_selected_cb);
}

static void _grid_key_down_cb(int id, void *data, Evas *e, Evas_Object *obj,
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
		elm_object_focus_set(priv->menu[priv->cur_menu_id], EINA_TRUE);
}

static input_handler _menu_input_handler = {
	.mouse_move = _mouse_move_cb,
	.focused = _menu_focused_cb,
	.key_down = _menu_key_down_cb
};

static input_handler _grid_input_handler = {
	.mouse_move = _grid_mouse_move_cb,
	.focused = _grid_focused_cb,
	.realized = _grid_realized_cb,
	.unrealized = _grid_unrealized_cb,
	.key_down = _grid_key_down_cb
};

static bool _draw_top_area(struct _priv *priv)
{
	Evas_Object *btn;
	struct grid_data *gdata;
	int i;

	elm_object_part_text_set(priv->base, PART_TOP_TITLE, STR_FAVORITE);

	priv->box = utils_add_box(priv->base, PART_TOP_MENU, EINA_TRUE,
			SIZE_BOX_PAD_H, SIZE_BOX_PAD_V);
	if (!priv->box) {
		_ERR("Add box failed.");
		return false;
	}

	for (i = 0; i < COUNT_MENU; i++) {
		gdata = _get_grid_data[i]();
		if (!gdata || !gdata->id) {
			evas_object_del(priv->box);
			return false;
		}

		btn = utils_add_button(priv->box, NULL, gdata->id,
				STYLE_MENU_BTN);
		if (!btn) {
			_ERR("Add button failed.");
			evas_object_del(priv->box);
			return false;
		}

		elm_box_pack_end(priv->box, btn);
		inputmgr_add_callback(btn, i, &_menu_input_handler, priv);

		priv->menu[i] = btn;
	}

	elm_object_focus_set(priv->menu[0], EINA_TRUE);
	elm_object_focus_next_object_set(priv->menu[0], priv->menu[0],
			ELM_FOCUS_LEFT);
	elm_object_focus_next_object_set(priv->menu[COUNT_MENU - 1],
			priv->menu[COUNT_MENU - 1], ELM_FOCUS_RIGHT);

	return true;
}

static bool _add_grid(struct _priv *priv, int id)
{
	struct grid_data *gdata;
	Evas_Object *grid;

	gdata = _get_grid_data[id]();
	if (!gdata || !gdata->id || !gdata->gclass) {
		_ERR("Get grid data failed.");
		return false;
	}

	grid = utils_add_gengrid(priv->base,
			gdata->item_size_x, gdata->item_size_y);
	if (!grid) {
		_ERR("Add grid failed.");
		return false;
	}

	if (!gridmgr_add_grid(priv->gmgr, gdata->id, grid, gdata->gclass)) {
		_ERR("Gridmgr add grid failed.");
		evas_object_del(grid);
		return false;
	}

	inputmgr_add_callback(grid, id, &_grid_input_handler, priv);

	gdata->grid = grid;

	return true;
}

static Evas_Object *_create(Evas_Object *win, void *data)
{
	struct _priv *priv;
	struct gridmgr *gmgr;
	Evas_Object *base;
	int i;

	if (!win) {
		_ERR("Invalid argument.");
		return NULL;
	}

	priv = calloc(1, sizeof(*priv));
	if (!priv) {
		_ERR("Calloc failed.");
		return NULL;
	}

	base = utils_add_layout(win, GRP_VIEW_BASE, EINA_TRUE);
	if (!base) {
		_ERR("Add layout failed.");
		free(priv);
		return NULL;
	}

	elm_win_resize_object_add(win, base);

	gmgr = gridmgr_create();
	if (!gmgr) {
		_ERR("Grid manager initialized failed.");
		goto err;
	}

	priv->win = win;
	priv->base = base;
	priv->gmgr = gmgr;
	priv->cur_menu_id = -1;

	for (i = 0; i < COUNT_MENU; i++) {
		if (!_add_grid(priv, i)) {
			_ERR("Add grid failed.");
			gridmgr_destroy(gmgr);
			goto err;
		}
	}

	if (!viewmgr_set_view_data(VIEW_BASE, priv)) {
		_ERR("Set view data failed.");
		gridmgr_destroy(gmgr);
		goto err;
	}

	if (!_draw_top_area(priv))
		_ERR("Draw top bottom area failed.");

	return base;

err:
	evas_object_del(base);
	free(priv);

	return NULL;
}

static void _show(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	if (priv->base)
		evas_object_show(priv->base);
}

static void _hide(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	if (priv->base)
		evas_object_hide(priv->base);
}

static void _update(void *view_data, int update_type, void *data)
{
	struct _priv *priv;
	struct grid_data *gdata;
	char *menu_category;
	int i;

	if (!view_data) {
		_ERR("Get data failed.");
		return;
	}
	priv = view_data;

	switch (update_type) {
	case UPDATE_MENU:
		if (!data)
			return;

		menu_category = data;

		for (i = 0; i < COUNT_MENU; i++) {
			gdata = _get_grid_data[i]();

			if (gdata && gdata->id
					&& !strcmp(gdata->id, menu_category)) {
				elm_object_focus_set(priv->menu[i], EINA_TRUE);
				break;
			}
		}

		break;

	case UPDATE_GRID:
		_update_current_grid(priv);
		break;

	case UPDATE_SHOW_TOAST:
		priv->notify = utils_add_notify(priv->base, (char *)data,
				STYLE_TOAST, STYLE_TOAST, SIZE_TOAST_TIMEOUT);

		break;

	case UPDATE_LOCK:
		if (!data)
			return;

		gdata = data;

		elm_object_item_signal_emit(gdata->focused_item, SIG_VISIBLE,
				SRC_LOCK_ICON);

		break;

	case UPDATE_UNLOCK:
		if (!data)
			return;

		gdata = data;

		elm_object_item_signal_emit(gdata->focused_item, SIG_INVISIBLE,
				SRC_LOCK_ICON);

		break;
	default:
		_ERR("Unhandled update type.");
	}
}

static void _destroy(void *data)
{
	struct _priv *priv;
	struct grid_data *gdata;
	int i;

	if (!data) {
		_ERR("Invalid argument.");
		return;
	}
	priv = data;

	for (i = 0; i < COUNT_MENU; i++) {
		gdata = _get_grid_data[i]();
		if (gdata->list)
			gdata->destroy_item_list(gdata->list);
	}

	if (priv->base)
		evas_object_del(priv->base);

	if (priv->gmgr)
		gridmgr_destroy(priv->gmgr);

	free(priv);
}

static view_class _vclass = {
	.view_id = VIEW_BASE,
	.create = _create,
	.show = _show,
	.hide = _hide,
	.update = _update,
	.destroy = _destroy,
};

view_class *view_base_get_vclass(void)
{
	return &_vclass;
}
