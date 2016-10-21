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
#include <ui-gadget.h>
#include <app_define.h>
#include <app_debug.h>

#include "grid.h"
#include "view.h"
#include "define.h"

struct _priv {
	Evas_Object *win;
	ui_gadget_h ug;
	struct grid_data *gdata;
};

static void _send_message(struct _priv *priv, const char *result)
{
	app_control_h app_control;
	int r;

	r = app_control_create(&app_control);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("Fail to create app_control");
		return;
	}

	r = app_control_add_extra_data(app_control, PARAM_PIN, result);
	if (r != APP_CONTROL_ERROR_NONE) {
		_ERR("Fail to add data");
		app_control_destroy(app_control);
		return;
	}

	ug_send_message(priv->ug, app_control);
	app_control_destroy(app_control);
}

static void _pincode_checked(struct _priv *priv, char *pincode)
{
	bool locked;

	if (!priv->gdata->item_action(priv->gdata->focused_item,
				ACTION_UPDATE_LOCK, pincode)) {
		_ERR("Update lock failed.");
		_send_message(priv, STR_FAIL);
		return;
	}

	if (!priv->gdata->item_locked(priv->gdata->focused_item, &locked)) {
		_ERR("Get lock status failed.");
		goto end;
	}


	if (locked) {
		viewmgr_update_view(VIEW_BASE, UPDATE_LOCK, priv->gdata);
		viewmgr_update_view(VIEW_BASE, UPDATE_SHOW_TOAST, STR_LOCKED);
	} else {
		viewmgr_update_view(VIEW_BASE, UPDATE_UNLOCK, priv->gdata);
		viewmgr_update_view(VIEW_BASE, UPDATE_SHOW_TOAST, STR_UNLOCKED);
	}

end:
	viewmgr_hide_view(VIEW_PIN);
	viewmgr_hide_view(VIEW_ACTION_MENU);
}

static void _ug_result_cb(ui_gadget_h ug, app_control_h result, void *priv)
{
	char *pincode;
	int r;

	if (!priv) {
		_ERR("Invalid argument.");
		return;
	}

	r = app_control_get_extra_data(result, PARAM_PIN, &pincode);
	if (r != APP_CONTROL_ERROR_NONE || !pincode) {
		_ERR("Fail to get data");
		return;
	}

	_pincode_checked(priv, pincode);

	free(pincode);
}

static void _ug_destroy_cb(ui_gadget_h ug, void *priv)
{
	viewmgr_hide_view(VIEW_PIN);
}

static ui_gadget_h _load_pin_ug(struct _priv *priv)
{
	ui_gadget_h ug;
	struct ug_cbs cbs = {0,};

	cbs.result_cb = _ug_result_cb;
	cbs.destroy_cb = _ug_destroy_cb;
	cbs.priv = priv;

	ug = ug_create(NULL, UG_ID_PIN, UG_MODE_FRAMEVIEW, NULL, &cbs);

	return ug;
}

static Evas_Object *_create(Evas_Object *win, void *data)
{
	struct _priv *priv;

	if (!win) {
		_ERR("Get window object failed.");
		return NULL;
	}

	priv = calloc(1, sizeof(*priv));
	if (!priv) {
		_ERR("Calloc failed.");
		return NULL;
	}

	priv->win = win;

	if (!viewmgr_set_view_data(VIEW_PIN, priv)) {
		_ERR("Set view data failed.");
		free(priv);
		return NULL;
	}

	return win;
}

static void _show(void *data)
{
	struct _priv *priv;
	ui_gadget_h ug;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	ug = _load_pin_ug(priv);
	if (!ug) {
		_ERR("Create pin ug failed.");
		return;
	}

	priv->ug = ug;
}

static void _hide(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	if (priv->ug)
		ug_destroy(priv->ug);
}

static void _update(void *view_data, int update_type, void *data)
{
	struct _priv *priv;

	if (!view_data) {
		_ERR("Get data failed.");
		return;
	}
	priv = view_data;

	if (update_type == UPDATE_DATA)
		priv->gdata = data;
}

static void _destroy(void *data)
{
	struct _priv *priv;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	priv = data;

	if (priv->ug)
		ug_destroy(priv->ug);

	free(priv);
}

static view_class _vclass = {
	.view_id = VIEW_PIN,
	.create = _create,
	.show = _show,
	.hide = _hide,
	.update = _update,
	.destroy = _destroy,
};

view_class *view_pin_get_vclass(void)
{
	return &_vclass;
}
