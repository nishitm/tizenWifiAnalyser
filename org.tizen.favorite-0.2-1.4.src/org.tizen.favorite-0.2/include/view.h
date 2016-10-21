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

#ifndef __AIR_FAVORITE_VIEW_H__
#define __AIR_FAVORITE_VIEW_H__

view_class *view_base_get_vclass(void);
view_class *view_action_menu_get_vclass(void);
view_class *view_pin_get_vclass(void);

enum update_type {
	UPDATE_DATA = 0,
	UPDATE_GRID,
	UPDATE_SHOW_TOAST,
	UPDATE_LOCK,
	UPDATE_UNLOCK,
	UPDATE_MENU,
};

#endif /* __AIR_APPS_VIEW_H__ */
