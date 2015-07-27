#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_stat_layer;
static TextLayer *s_twitter_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_quote_layer;
static char s_str [] = "        NO SLEEP WATCH ";
static char s_str_2 [] = " World Record 264.4 hrs";
static const uint32_t segments[] = { 10,10,10,20,40,50,100,10,10,10,10,10,10, 100,10,10,10, 10,10,10,100,10,10,10,10,10,10,100,10,10,10,10,10,10,100 };
int human = 1;
int counter = 1;
int acceldecay =0;

VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char buffer2[]= "                                                                                     ";
 // static char buffer3[] = "Twitter";
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    //strftime(buffer, sizeof("00:00 A"), "%I:%M %P", tick_time);
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  // Combine header
  snprintf(buffer2, sizeof(buffer2), "%s\n%s", s_str,s_str_2);
  
  // Generate Cool Humans
   switch (human) {
    // This is component that displays the image watchface
    case 1:
      // Cycle the icon
      text_layer_set_text(s_quote_layer, "You have to learn the rules of the game. And then you have to play better than anyone else.");    
      text_layer_set_text(s_twitter_layer, "Albert Einstein");    
      break;
 
    case 2:
     text_layer_set_text(s_quote_layer, "There is only one corner of the universe you can be certain of improving, and that's your own self.");    
      text_layer_set_text(s_twitter_layer, "Aldous Huxley");
      break;

    case 3:
          text_layer_set_text(s_quote_layer, "The secret of getting ahead is getting started.");    

      text_layer_set_text(s_twitter_layer, "Mark Twain");
      break;


      case 4:
        text_layer_set_text(s_quote_layer, "Stay hungry. Stay foolish."); 
      text_layer_set_text(s_twitter_layer, "Steve Jobs");
      break;
     
     
      case 5:
     text_layer_set_text(s_quote_layer, "Breathe in throgh the nose. Pause. Breathe out through the mouth. Pause."); 
      text_layer_set_text(s_twitter_layer, "Lex Dreitser");
      break;
     
      case 6:
       human = 1;
     break;


  }

  counter++;
   if(counter == 10)
   {     
     counter = 0;
     human++;
   }
  
  


  // buffer3 = " Twitter";
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_stat_layer, buffer2);
  battery_state_service_peek();
}

 static void accel_handler(AccelData *data, uint32_t num_samples) {
   // If facing up Light It Up!!!
   if(((data[0].z < -980) && (data[0].z >-1020))&& ((data[1].z < -980) && (data[1].z >-1020)) && ((data[2].z < -980) && (data[2].z >-1020)) ){
     // light_enable_interaction();
     update_time();
    

        //vibes_double_pulse();
     }
   int tol = 10;
   int ztop = data[0].z + tol;
   int zbot = data[0].z - tol;
   int xtop = data[0].x + tol;
   int xbot = data[0].x - tol;
   int ytop = data[0].y + tol;
   int ybot = data[0].y - tol;
   
   if((data[1].z < ztop)&&(data[1].z > zbot) && (data[2].z < ztop)&&(data[2].z > zbot)&& (data[1].y < ytop)&&(data[1].y > ybot) && (data[2].y < ytop)&&(data[2].y > ybot)&&  (data[1].x < xtop)&&(data[1].x > xbot) && (data[2].x < xtop)&&(data[2].x > xbot)){
      vibes_enqueue_custom_pattern(pat);
      light_enable_interaction();
   }
 }
static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "100% charged";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "Charging %d%%", charge_state.charge_percent);
  } else {
    snprintf(battery_text, sizeof(battery_text), " %d%% Battery", charge_state.charge_percent);
  }
  
  text_layer_set_text(s_battery_layer, battery_text);
  text_layer_set_size(s_battery_layer, (GSize){charge_state.charge_percent *1.44,20} ); 
}



static void main_window_load(Window *window) {
 // Create GBitmap, then set to created BitmapLayer
//s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1);


 // Create quote TextLayer 
  s_quote_layer = text_layer_create(GRect(0, 30, 144, 75));
  text_layer_set_background_color(s_quote_layer, GColorClear);
  text_layer_set_text_color(s_quote_layer, GColorBlack);
  text_layer_set_font(s_quote_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_quote_layer, GTextAlignmentCenter);
  
// Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 106, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
   s_stat_layer = text_layer_create(GRect(0, 0, 144, 30));
   //text_layer_set_text(s_stat_layer, "World Record: 264.4 hours");
  
   s_twitter_layer = text_layer_create(GRect(0, 101, 144, 16));
   text_layer_set_background_color(s_twitter_layer, GColorClear);
   text_layer_set_text_alignment(s_twitter_layer, GTextAlignmentCenter);
   text_layer_set_font(s_twitter_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  
   s_battery_layer = text_layer_create(GRect(0, 152, 144, 16));
   text_layer_set_background_color(s_battery_layer, GColorBlack);
   text_layer_set_text_color(s_battery_layer, GColorWhite);
   text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  
//    s_heading_layer = text_layer_create(GRect(35, 150, 144, 16));
//    text_layer_set_font(s_heading_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
//    text_layer_set_text(s_heading_layer, "00.0000  ");
  
  
   // Add as a child layer to the Window's root layer
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_stat_layer));
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_twitter_layer));
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_quote_layer)); 
  
  
    //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_heading_layer));
  
    battery_state_service_subscribe(handle_battery);
   
}

static void main_window_unload(Window *window) {
   text_layer_destroy(s_time_layer);
   text_layer_destroy(s_stat_layer);
   text_layer_destroy(s_twitter_layer);
   text_layer_destroy(s_battery_layer);
   text_layer_destroy(s_quote_layer);
 
  battery_state_service_unsubscribe();
  accel_data_service_unsubscribe();
   

}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  handle_battery(battery_state_service_peek());  
  
}
static void init() {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Subscribe to the accelerometer data service
  int num_samples = 3;
    accel_data_service_subscribe(num_samples, accel_handler);

    // Choose update rate
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
// Create main Window element and assign to pointer
  s_main_window = window_create();
  
  #ifdef PBL_SDK_2
    window_set_fullscreen(s_main_window, true);
  #endif
    
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}