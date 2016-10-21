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
#include <dlog.h>
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

     Evas_Object *li1,*pb,*li2; 

     char **buf;
     buf = (char **)malloc(sizeof(char *));
     buf[0] = (char *)malloc(sizeof(char)*10);

     struct _appdata *ad=data;
     Evas_Object *win = ad->win; 

     

     if(app_control_get_extra_data(control,"control", buf)==APP_CONTROL_ERROR_NONE)
     {
          if(atoi(buf[0])==1)
          {
               pb = elm_progressbar_add(win);
               elm_object_style_set(pb, "wheel");
               elm_object_text_set(pb, "Style: wheel");
               elm_progressbar_pulse_set(pb, EINA_TRUE);
               elm_progressbar_pulse(pb, EINA_TRUE);
               evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
               evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
               elm_box_pack_end(bx, pb);
               evas_object_show(pb);
               example_data.pb8 = pb;
          }
     }

     if(app_control_get_extra_data(control,"pre", buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          //elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_TRUE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li1 = elm_list_add(win);
          evas_object_size_hint_weight_set(li1, 0.0, 0.30);
          evas_object_size_hint_align_set(li1, EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li1);
          elm_list_item_append(li1,"Waiting To Send/Receive File",NULL,NULL,NULL,NULL);
          evas_object_show(li1);
          elm_list_go(li1);
          example_data.pb8 = pb;
     }

     if(app_control_get_extra_data(control,"ap_name",buf)==APP_CONTROL_ERROR_NONE)
     {
          
          li1 = elm_list_add(win);
          evas_object_size_hint_weight_set(li1, 0.0, 0.30);
          evas_object_size_hint_align_set(li1, EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li1);
          elm_list_item_append(li1,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li1);
          elm_list_go(li1);
     }
     
     if(app_control_get_extra_data(control,"connect",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          li2 = elm_list_add(win);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"Connected To Access Point",NULL,NULL,NULL,NULL);
          elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }

     if(app_control_get_extra_data(control,"receiving",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          // elm_object_text_set(pb, "Receiving");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_TRUE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li2 = elm_list_add(win);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"Receiving Into File",NULL,NULL,NULL,NULL);
          elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }


     if(app_control_get_extra_data(control,"received",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_FALSE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li2 = elm_list_add(win);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"Received Into File @ ",NULL,NULL,NULL,NULL);
          elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }

     if(app_control_get_extra_data(control,"sending",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          // elm_object_text_set(pb, "Receiving");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_TRUE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li2 = elm_list_add(win);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
        
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"Sending File",NULL,NULL,NULL,NULL);
          elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }

     if(app_control_get_extra_data(control,"connecting",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          //elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          // elm_object_text_set(pb, "Receiving");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_TRUE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li2 = elm_list_add(win);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
        
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"Waiting to Connect",NULL,NULL,NULL,NULL);
          //elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }

     if(app_control_get_extra_data(control,"sent",buf)==APP_CONTROL_ERROR_NONE)
     {
          elm_box_clear(bx);
          pb = elm_progressbar_add(win);
          elm_object_style_set(pb, "wheel");
          elm_object_text_set(pb, "Style: wheel");
          elm_progressbar_pulse_set(pb, EINA_TRUE);
          elm_progressbar_pulse(pb, EINA_FALSE);
          evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.2);
          evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
          elm_box_pack_end(bx, pb);
          evas_object_show(pb);

          li2 = elm_list_add(win);
          evas_object_size_hint_weight_set(li2, 0.0, 0.30);
          evas_object_size_hint_align_set(li2,EVAS_HINT_FILL, EVAS_HINT_FILL);
          elm_box_pack_end(bx, li2);
          elm_list_item_append(li2,"File Sent",NULL,NULL,NULL,NULL);
          elm_list_item_append(li2,buf[0],NULL,NULL,NULL,NULL);
          evas_object_show(li2);
          elm_list_go(li2);
     }
     
}

static void _pause(void *data)
{
  inputmgr_enable(EINA_FALSE);
}

static void _resume(void *data)
{

}

static bool _create(void *data)
{
  struct _appdata *ad;
   ad = data;
  
   win = elm_win_util_standard_add("progressbar", "Progress bar example");
   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);
   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx); 
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
