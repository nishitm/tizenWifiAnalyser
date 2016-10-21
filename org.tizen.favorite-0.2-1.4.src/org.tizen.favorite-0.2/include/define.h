/*
 * Copyright (c 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License;
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

#ifndef __AIR_FAVORITE_DEFINE_H__
#define __AIR_FAVORITE_DEFINE_H__

/* View ID */
#define VIEW_BASE "VIEW_BASE"
#define VIEW_ACTION_MENU "VIEW_ACTION_MENU"
#define VIEW_PIN "VIEW_PIN"

/* Grid ID */
#define GRID_TV "TV"
#define GRID_MOVIE "Movie"
#define GRID_GALLERY "Gallery"
#define GRID_MUSIC "Music"
#define GRID_APP "Apps"
#define GRID_WEB "Web"

/* Group */
#define GRP_VIEW_BASE "grp.view.base"
#define GRP_VIEW_ACTION_MENU "grp.view.action.menu"
#define GRP_TV_BANNER "grp.tv.banner"

/* Part for edc file */
#define PART_TOP_TITLE "part.top.title"
#define PART_TOP_MENU "part.top.menu"
#define PART_GENGRID "part.gengrid"
#define PART_NO_CONTENTS "part.no.contents"
#define PART_CONTENTS_COUNT "part.contents.count"
#define PART_CONTENTS_SUBCOUNT "part.contents.subcount"
#define PART_COUNT_DIVIDE_LINE "part.count.divide.line"
#define PART_THUMB_BG "part.thumb.bg"
#define PART_TEXT_BG "part.text.bg"
#define PART_THUMB_ICON "part.thumb.icon"
#define PART_THUMB_TEXT "part.thumb.text"
#define PART_THUMB_TEXT_BR "part.thumb.text.browser"
#define PART_THUMB_SUBTEXT1 "part.thumb.subtext1"
#define PART_THUMB_SUBTEXT2 "part.thumb.subtext2"
#define PART_THUMB_PLAY_ICON "part.thumb.play.icon"
#define PART_THUMB_DEFAULT_TV_ICON "part.thumb.default.tv.icon"
#define PART_LIVETV_BTN "part.livetv.button"
#define PART_ACTION_MENU "part.action.menu"
#define PART_BANNER "part.banner"
#define PART_TITLE "part.title"
#define PART_SUBTITLE "part.subtitle"
#define PART_DESCRIPT "part.description"

/* Style */
#define STYLE_MENU_BTN "style.menu.button"
#define STYLE_LIVETV_BTN "style.livetv.button"
#define STYLE_ACTION_MENU_FAV_BTN "style.action.menu.favorite.button"
#define STYLE_ACTION_MENU_LOCK_BTN "style.action.menu.lock.button"
#define STYLE_TOAST "style.toast"

/* Signal */
#define SIG_FOCUSED "focused"
#define SIG_UNFOCUSED "unfocused"
#define SIG_SELECTED "selected"
#define SIG_UNSELECTED "unselected"
#define SIG_ITEM_SELECTED "item.selected"
#define SIG_ITEM_FOCUSED "item_focused"
#define SIG_VISIBLE "visible"
#define SIG_INVISIBLE "invisible"
#define SIG_DISABLE "elm,state,disabled"
#define SIG_TOGGLE "toggle"
#define SIG_TIMEOUT "timeout"
#define SIG_SHOW "show"
#define SIG_HIDE "hide"
#define SIG_HIDE_DONE "hide.done"

/* Source */
#define SRC_EDC "edc"
#define SRC_MENU_BTN "menubtn"
#define SRC_ACTION_BTN "source.action.btn"
#define SRC_LOCK_ICON "source.lock.icon"

/* Transition */
#define TRANSITION_FOCUS DECELERATE 0.20
#define TRANSITION_CLICK DECELERATE 0
#define TRANSITION_MENU_SHOW SIN 0.45
#define TRANSITION_MENU_HIDE DECEL 0.3

/* Text */
#define STR_FAVORITE "Favorite"
#define STR_LOCK "Lock"
#define STR_NULL "(NULL)"
#define STR_NO_CONTENTS "No Contents"
#define STR_CNT_TV "channels"
#define STR_CNT_MOVIE "movies"
#define STR_CNT_GALLERY "files"
#define STR_CNT_GALLERY_PHOTO "photos"
#define STR_CNT_GALLERY_VIDEO "videos"
#define STR_CNT_MUSIC "songs"
#define STR_CNT_APPS "apps"
#define STR_CNT_WEBS "bookmark"
#define STR_BROWSER "Web Browser"
#define STR_LIVETV "Live TV"
#define STR_REMOVE_FAVORITE "Removed from your favorite."
#define STR_SUCCESS "success"
#define STR_FAIL "fail"
#define STR_LOCKED "Content is locked."
#define STR_UNLOCKED "Content is unlocked."

/* Font */
#define FONT_TIZENSANS_REGULAR "BreezeSans"
#define FONT_TIZENSANS_LIGHT "BreezeSans:style=Light"

/* Color */
#define COLOR_TEXT_BG 113 128 147 255
#define COLOR_ITEM_FOCUS 0 119 246 255
#define COLOR_ITEM_FOCUS_0 0 119 246 0
#define COLOR_BR_R 255
#define COLOR_BR_G 255
#define COLOR_BR_B 255
#define COLOR_BR_A 255

/* Size */
#define SIZE_BOX_PAD_H (40 + 40)
#define SIZE_BOX_PAD_V 0
#define SIZE_STR 1024
#define SIZE_APP_ICON_MAX 100
#define SIZE_ACTION_TABLE_BORDER 0
#define SIZE_COL_MAX 4
#define SIZE_TOAST_TIMEOUT 5.0

/* Count */
#define COUNT_MENU 6
#define COUNT_ACTION 2
#define COUNT_BANNER 1

/* Image */
#define PLAY_ICON_PNG "ic_thumbnail_play.png"
#define DEFAULT_IMAGE_PNG IMGDIR"/ic_thumbnail_picture.png"
#define DEFAULT_APP_ICON_PNG IMGDIR"/ic_default_app.png"
#define DEFAULT_TV_PNG IMGDIR"/ic_default_tv.png"
#define BROWSER_ICON_PNG IMGDIR"/ic_apps_web.png"
#define ACTION_FAV_NOR_PNG "btn_menu_favorite_nor.png"
#define ACTION_FAV_FOC_PNG "btn_menu_favorite_foc.png"
#define ACTION_FAV_SEL_PNG "btn_menu_favorite_sel.png"
#define ACTION_FAV_DIS_PNG "btn_menu_favorite_dis.png"
#define ACTION_UNLOCK_NOR_PNG "btn_menu_unlock_nor.png"
#define ACTION_UNLOCK_FOC_PNG "btn_menu_unlock_foc.png"
#define ACTION_LOCK_NOR_PNG "btn_menu_lock_nor.png"
#define ACTION_LOCK_FOC_PNG "btn_menu_lock_foc.png"
#define ACTION_LOCK_SEL_PNG "btn_menu_lock_sel.png"
#define ACTION_LOCK_DIS_PNG "btn_menu_lock_dis.png"
#define LOCK_ICON_PNG "ic_thumbnail_lock_01.png"

/* Path */
#define PATH_PLAY_ICON_PNG IMGDIR"/ic_thumbnail_play.png"

#endif /* __AIR_FAVORITE_DEFINE_H__ */
