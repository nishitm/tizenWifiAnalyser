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
#include <ui-gadget.h>
#include <app_define.h>
#include <app_debug.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "utils.h"
#include "view.h"

SET_TAG(PACKAGE);

struct _appdata {
	Evas_Object *win;

	const char *name;
};

Evas_Object *bx = NULL;
Evas_Object* win ;

typedef struct Progressbar_Example
{
   Evas_Object *pb1;
   Evas_Object *pb2; /* pulsing */
   Evas_Object *pb3;
   Evas_Object *pb4;
   Evas_Object *pb5;
   Evas_Object *pb6; /* pulsing */
   Evas_Object *pb7;
   Evas_Object *pb8; /* pulsing */

   Eina_Bool    run;
   Ecore_Timer *timer;
} Progressbar_Example;

static Progressbar_Example example_data;

// static Eina_Bool
// _progressbar_example_value_set(void *data)
// {
//    double progress;

//    progress = elm_progressbar_value_get(example_data.pb1);
//    if (progress < 1.0) progress += 0.0001;
//    else progress = 0.0;

//    /* just the non-pulsing ones need an update */
//    elm_progressbar_value_set(example_data.pb1, progress);
//    elm_progressbar_value_set(example_data.pb3, progress);
//    elm_progressbar_value_set(example_data.pb4, progress);
//    elm_progressbar_value_set(example_data.pb5, progress);
//    elm_progressbar_value_set(example_data.pb7, progress);

//    if (progress < 1.0) return ECORE_CALLBACK_RENEW;

//    example_data.run = 0;
//    return ECORE_CALLBACK_CANCEL;
//}

// static void
// _progressbar_example_start(void        *data,
//                            Evas_Object *obj,
//                            void        *event_info)
// {
//    //elm_progressbar_pulse(example_data.pb2, EINA_TRUE);
//    //elm_progressbar_pulse(example_data.pb6, EINA_TRUE);
//    elm_progressbar_pulse(example_data.pb8, EINA_TRUE);

//    // if (!example_data.run)
//    //   {
//    //      example_data.timer = ecore_timer_add(
//    //         0.1, _progressbar_example_value_set, NULL);
//    //      example_data.run = EINA_TRUE;
//    //   }
// }

/* end of show */
static void
_progressbar_example_stop(void        *data,
                          Evas_Object *obj,
                          void        *event_info)
{
   elm_progressbar_pulse(example_data.pb2, EINA_FALSE);
   elm_progressbar_pulse(example_data.pb6, EINA_FALSE);
   elm_progressbar_pulse(example_data.pb8, EINA_FALSE);

   if (example_data.run)
     {
        ecore_timer_del(example_data.timer);
        example_data.run = EINA_FALSE;
     }
}

// /* Format callback */
// static char *
// _progress_format_cb(double val)
// {
//    static char buf[30];
//    int files = (1-val)*14000;
//    if (snprintf(buf, 30, "%i files left", files) > 0)
//      return strdup(buf);
//    return NULL;
// }

// static void
// _progress_format_free(char *str)
// {
//    free(str);
// }

// /* Callback for "changed" signal */
// static void
// _on_changed(void        *data,
//             Evas_Object *obj,
//             void        *event_info)
// {
//    static char buf[30];
//    static time_t tstart = 0;
//    static double eta = 0;
//    time_t tdiff;
//    double val;
//    Evas_Object *label =  (Evas_Object *)data;

//    val = elm_progressbar_value_get(obj);
//    if (val == 0)
//      {
//         tstart = 0;
//         elm_object_text_set(label, "ETA: N/A");
//         return;
//      }

//    /* First invocation */
//    if (tstart == 0)
//      {
//         tstart = time(NULL);
//      }

//    /* Calculate ETA and update */
//    tdiff = time(NULL) - tstart;
//    eta = 0.3*eta + 0.7*(tdiff/val)*(1-val);
//    snprintf(buf, 30, "ETA: %.0fs", eta);
//    elm_object_text_set(label, buf);
// }

static void
_on_done(void        *data,
         Evas_Object *obj,
         void        *event_info)
{
  //printf("Cancelling");
   _progressbar_example_stop(NULL, NULL, NULL);
   elm_exit();
}



/*static void
on_click(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}*/


static void _terminate(void *data)
{
	struct _appdata *ad;

	if (!data) {
		_ERR("Get data failed.");
		return;
	}
	ad = data;

	if (ad->win) {
		evas_object_del(ad->win); 
		ad->win = NULL;
	}

}


static void _app_control(app_control_h control, void *data)
{
    // then u need to move this also in create??

   Evas_Object *li,*pb;
   //Evas_Object *hbx,*bt_bx,*bt, *ic1, *ic2, *label;
   //char buf[PATH_MAX];
   //char request[100];

   //char *newrequst[50];
   char **buf;
   buf = (char **)malloc(sizeof(char *));
   buf[0] = (char *)malloc(sizeof(char)*10);

   //char* Value = NULL;   
   struct _appdata *ad=data;
   Evas_Object *win = ad->win; // this would work right?yes

   
   
   if(app_control_get_extra_data(control,"control", buf)==APP_CONTROL_ERROR_NONE)
   {
       //try once .. okay 
       if(atoi(buf[0])==1)
       {


//           "wheel" style progress bar 
           pb = elm_progressbar_add(win);
           elm_object_style_set(pb, "wheel");
           elm_object_text_set(pb, "Style: wheel");
           elm_progressbar_pulse_set(pb, EINA_TRUE);
           //elm_obj_progressbar_pulse(EINA_TRUE);   
           elm_progressbar_pulse(pb, EINA_TRUE);
           evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
           evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
           elm_box_pack_end(bx, pb);
           evas_object_show(pb);
           example_data.pb8 = pb;


            //   li = elm_list_add(win);
            // evas_object_size_hint_weight_set(li, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
            // evas_object_size_hint_align_set(li, EVAS_HINT_FILL, EVAS_HINT_FILL);
            // elm_box_pack_end(bx, li);
            // //for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
            // elm_list_item_append(li,"iitk" , NULL, NULL, NULL, NULL);

            // evas_object_show(li);
            // elm_list_go(li);


           // bt = elm_button_add(win);
           // elm_object_text_set(bt, "Start");
           // evas_object_smart_callback_add(bt, "clicked", _progressbar_example_start,NULL);
       }
       else if(atoi(buf[0])==2)
       {

            //but the output is not this .>> because it was 1 when we compiled. okay !

            li = elm_list_add(win);
            evas_object_size_hint_weight_set(li, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
            evas_object_size_hint_align_set(li, EVAS_HINT_FILL, EVAS_HINT_FILL);
            elm_box_pack_end(bx, li);
            //for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
            elm_list_item_append(li,"iitk" , NULL, NULL, NULL, NULL);

            evas_object_show(li);
            elm_list_go(li);
       }
   }

   
   //elm_app_info_set(elm_main, "elementary", "images/logo_small.png");

   
   
   
   // pb with no label, default unit label and no icon 
   // pb = elm_progressbar_add(win);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // elm_box_pack_end(bx, pb);
   // evas_object_show(pb);
   // example_data.pb1 = pb;
   
   //pb with label, and set to pulse 
   // pb = elm_progressbar_add(win);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_object_text_set(pb, "Infinite bounce");
   // elm_progressbar_pulse_set(pb, EINA_TRUE);
   // elm_box_pack_end(bx, pb);
   // evas_object_show(pb);
   // example_data.pb2 = pb;

   // ic1 = elm_icon_add(win);
   // snprintf(buf, sizeof(buf), "%s/images/logo_small.png", elm_app_data_dir_get());
   // elm_image_file_set(ic1, buf, NULL);
   // evas_object_size_hint_aspect_set(ic1, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

   // // pb with label, icon, custom unit label function and span size set 
   // pb = elm_progressbar_add(win);
   // elm_object_text_set(pb, "Label");
   // elm_object_part_content_set(pb, "icon", ic1);
   // elm_progressbar_inverted_set(pb, EINA_TRUE);
   // elm_progressbar_unit_format_function_set(pb, _progress_format_cb,
   //                                          _progress_format_free);
   // elm_progressbar_span_size_set(pb, 200);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(bx, pb);
   // evas_object_show(ic1);
   // evas_object_show(pb);
   // example_data.pb3 = pb;

   // //pb with label and changed trigger  
   // pb = elm_progressbar_add(win);
   // elm_object_text_set(pb, "Label");
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(bx, pb);
   // evas_object_show(pb);

   // label = elm_label_add(win);
   // elm_object_text_set(label, "ETA: N/A");
   // evas_object_size_hint_align_set(label, 0.5, 0.5);
   // evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(bx, label);
   // evas_object_show(label);

   // evas_object_smart_callback_add(pb, "changed", _on_changed, label);
   // example_data.pb4 = pb;

   // hbx = elm_box_add(win);
   // elm_box_horizontal_set(hbx, EINA_TRUE);
   // evas_object_size_hint_weight_set(hbx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // evas_object_size_hint_align_set(hbx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   // elm_box_pack_end(bx, hbx);
   // evas_object_show(hbx);

   // //vertical pb 
   // pb = elm_progressbar_add(win);
   // elm_progressbar_horizontal_set(pb, EINA_FALSE);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(hbx, pb);
   // elm_object_text_set(pb, "percent");
   // evas_object_show(pb);
   // example_data.pb5 = pb;

   // //vertical pb, with pulse and custom (small) span size 
   // pb = elm_progressbar_add(win);
   // elm_progressbar_horizontal_set(pb, EINA_FALSE);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_progressbar_span_size_set(pb, 80);
   // elm_progressbar_pulse_set(pb, EINA_TRUE);
   // elm_progressbar_unit_format_set(pb, NULL);
   // elm_object_text_set(pb, "Infinite bounce");
   // elm_box_pack_end(hbx, pb);
   // evas_object_show(pb);
   // example_data.pb6 = pb;

   // ic2 = elm_icon_add(win);
   // elm_image_file_set(ic2, buf, NULL);
   // evas_object_size_hint_aspect_set(ic2, EVAS_ASPECT_CONTROL_HORIZONTAL, 1, 1);

   // //vertical pb, inverted, with custom unit format and icon
   // pb = elm_progressbar_add(win);
   // elm_progressbar_horizontal_set(pb, EINA_FALSE);
   // elm_object_text_set(pb, "Label");
   // elm_object_part_content_set(pb, "icon", ic2);
   // elm_progressbar_inverted_set(pb, EINA_TRUE);
   // elm_progressbar_unit_format_set(pb, "%1.2f%%");
   // elm_progressbar_span_size_set(pb, 200);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(hbx, pb);
   // evas_object_show(ic2);
   // evas_object_show(pb);
   // example_data.pb7 = pb;

   // //"wheel" style progress bar 
   // pb = elm_progressbar_add(win);
   // elm_object_style_set(pb, "wheel");
   // elm_object_text_set(pb, "Style: wheel");
   // elm_progressbar_pulse_set(pb, EINA_TRUE);
   // evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   // evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(bx, pb);
   // evas_object_show(pb);
   // example_data.pb8 = pb;

   // bt_bx = elm_box_add(win);
   // elm_box_horizontal_set(bt_bx, EINA_TRUE);
   // evas_object_size_hint_weight_set(bt_bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   // elm_box_pack_end(bx, bt_bx);
   // evas_object_show(bt_bx);

   // bt = elm_button_add(win);
   // elm_object_text_set(bt, "Start");
   // evas_object_smart_callback_add(bt, "clicked", _progressbar_example_start,
   //                                NULL);
   // elm_box_pack_end(bt_bx, bt);
   // evas_object_show(bt);

   // bt = elm_button_add(win);
   // elm_object_text_set(bt, "Stop");
   // evas_object_smart_callback_add(bt, "clicked", _progressbar_example_stop,
   //                                NULL);
   // elm_box_pack_end(bt_bx, bt);
   // evas_object_show(bt);

  
  

   // Evas_Object *win = NULL;
   // Evas_Object *btn = NULL;

   // elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   // /* Create an win, associate it with a canvas and */
   // /* turn it visible on WM (Window Manager).       */
   // char *buffer=malloc(sizeof(char)*100);
   // app_control_get_extra_data(control,"myKey", &buffer);
   // win = elm_win_util_standard_add("Greetings", "Hello, World!");
   // elm_win_autodel_set(win, EINA_TRUE);

   // /* Create a btn, associate to a function, associate */
   // /* to win,  give a dimension and position.          */
   // btn = elm_button_add(win);
   // elm_object_text_set(btn, buffer);

   // evas_object_smart_callback_add(btn, "clicked", on_click, NULL);
   // evas_object_resize(btn, 120, 30);
   // evas_object_move(btn, 60, 15);
   // evas_object_show(btn);

   // evas_object_resize(win, 240, 60);
   // evas_object_show(win);
	// struct _appdata *ad;
	// char *category;
	// int r;

	// if (!data) {
	// 	_ERR("Get data failed.");
	// 	return;
	// }
	// ad = data;

	// if (ad->win)
	// 	elm_win_activate(ad->win);

	// r = app_control_get_extra_data(control, PARAM_CATEGORY, &category);
	// if (r != APP_CONTROL_ERROR_NONE)
	// 	category = NULL;

	// if (category)
	// 	viewmgr_update_view(VIEW_BASE, UPDATE_MENU, category);

	// if (!viewmgr_push_view(VIEW_BASE))
	// 	_ERR("Push view failed.");

	// char **buffer;
	// buffer=(char**)malloc(sizeof(char*));
	// buffer[0]=(char*)malloc(sizeof(char)*100);

	// app_control_get_extra_data(control,"myKey", buffer);

	// viewmgr_update_view(VIEW_BASE, UPDATE_MENU, buffer[0]);
	// viewmgr_push_view(VIEW_BASE);
}

static void _pause(void *data)
{
// 	view_state state;

// 	// state = viewmgr_get_view_state(VIEW_ACTION_MENU);
// 	// if (state == VIEW_STATE_VISIBLE)
// 	// 	viewmgr_hide_view(VIEW_ACTION_MENU);

// 	// state = viewmgr_get_view_state(VIEW_PIN);
// 	// if (state == VIEW_STATE_VISIBLE)
// 	// 	viewmgr_hide_view(VIEW_PIN);

	inputmgr_enable(EINA_FALSE);
}

static void _resume(void *data)
{
//	inputmgr_enable(EINA_TRUE);
}
static bool _create(void *data)
{
//  inputmgr_enable(EINA_TRUE); fine ??//go to place from where u deleted these line...
  struct _appdata *ad;
   ad = data;
  
   win = elm_win_util_standard_add("progressbar", "Progress bar example");
   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);
   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx); //where is this box used? Window m ek box banta usme sab aata h wha use kiya h.. oh in control callback it is used? yes 
   evas_object_show(win);
   ad->win = win;
    return true;
}

int main(int argc, char *argv[])
{
	struct _appdata ad;
	ui_app_lifecycle_callback_s cbs = {
		.create = _create,
		.terminate = _terminate,
		.app_control = _app_control,
		.pause = _pause,
		.resume = _resume,
	};

	memset(&ad, 0x00, sizeof(ad));
	ad.name = PACKAGE;

	return ui_app_main(argc, argv, &cbs, &ad);
}
