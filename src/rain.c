#include <pebble.h>

#define KEY_READY 0
#define KEY_LOCATION 1
#define KEY_RAINDROPS 2
#define KEY_RAINTIMES 3
#define KEY_RAINPERHOUR 4
#define KEY_RAINUNIT 5
#define KEY_NOW 6
#define KEY_REFRESH 7
#define KEY_TEMP 8
#define KEY_DATE 9
#define KEY_LONGTERM 10
#define KEY_24H 11

static GFont s_unibody_font;

static Window *s_main_window;
static Layer *s_graph_layer;
static Layer *s_pointer_layer;
static Layer *s_main_ui_layer;
static TextLayer *s_currenttime_layer;
static TextLayer *s_time_layer;
static TextLayer *s_ampm_layer;
static TextLayer *s_date_layer;
static TextLayer *s_temp_layer;
static TextLayer *s_location_layer;
static TextLayer *s_raininfo_small_layer;
static TextLayer *s_raininfo_large_layer;
static GPath *s_graph_path;

static uint8_t amountOfData = 0;
static uint8_t rainDrops[61];
static uint8_t ampms[61];
static char* rainPerHour[61];
static char* rainTimes[61];
static char* temps[61];
static char* dates[61];

static char rainPerH[61][6];

static char rainTime[61][6];

static char temp[61][6];

static char date[61][6];

static char* rainUnit;
static uint8_t currentDataIterator = 0;
static uint8_t currentPosition = 0;
static uint8_t nowPointer = 0;
static uint8_t refreshPointer = 5;
static bool positionChanged = false;
static bool getLongTerm = false;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[6];
  static char s_ampm_buffer[3];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M":"%I:%M", tick_time);
  strftime(s_ampm_buffer, sizeof(s_ampm_buffer), clock_is_24h_style() ? " ":"%p", tick_time);

  // Display this time on the TextLayer
  if(currentPosition == nowPointer) {
    text_layer_set_text(s_time_layer, s_time_buffer);
    text_layer_set_text(s_ampm_layer, s_ampm_buffer);
  }else{
    text_layer_set_text(s_currenttime_layer, s_time_buffer);
  }
}

static void draw_graph(Layer *this_layer, GContext *ctx) {

  static struct GPathInfo s_graph_path_info[1];

  GRect bounds = layer_get_bounds(this_layer);
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

  s_graph_path_info[0].num_points = amountOfData;
  
  static GPoint points[128];
  uint8_t i = 0;

  while (i < amountOfData) {
    int32_t degree = TRIG_MAX_ANGLE * (i-((amountOfData-1)/4)) / (amountOfData-1);
    uint8_t radius = (rainDrops[i] * (center.x-30) -1)/255 + 1;

    uint8_t x = center.x + (30+radius) * cos_lookup(degree) / TRIG_MAX_RATIO;
    uint8_t y = center.y + (30+radius) * sin_lookup(degree) / TRIG_MAX_RATIO;

    GPoint point = GPoint(x, y);
    points[i] = point;

    i++;
  }

  s_graph_path_info[0].points = points; 

  // create graph
  s_graph_path = gpath_create(s_graph_path_info);

  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorDarkGray));

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_graph_path);

  // destroy gpath
  gpath_destroy(s_graph_path);
}

static void draw_line(Layer *this_layer, GContext *ctx) {

  GRect bounds = layer_get_bounds(this_layer);
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

  #if defined(PBL_COLOR)
    graphics_context_set_stroke_color(ctx, GColorDarkGray);
    graphics_draw_line(ctx, GPoint(center.x, center.y - 30), GPoint(center.x, center.y - center.x));
  #endif

  int32_t degree = TRIG_MAX_ANGLE * (currentPosition-((amountOfData-1)/4)) / (amountOfData-1);

  uint8_t x1 = center.x + 30 * cos_lookup(degree) / TRIG_MAX_RATIO;
  uint8_t y1 = center.y + 30 * sin_lookup(degree) / TRIG_MAX_RATIO;

  uint8_t x2 = center.x + center.x * cos_lookup(degree) / TRIG_MAX_RATIO;
  uint8_t y2 = center.y + center.x * sin_lookup(degree) / TRIG_MAX_RATIO;

  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
  graphics_draw_line(ctx, GPoint(x1, y1), GPoint(x2, y2));
}

static void set_current_raininfo() {
  static char raininfo_layer_buffer[12];
  static char raininfozero_layer_buffer[12];

  snprintf(raininfo_layer_buffer, sizeof(raininfo_layer_buffer), "%s", rainPerHour[currentPosition]);
  snprintf(raininfozero_layer_buffer, sizeof(raininfozero_layer_buffer), "0 %s", rainUnit);

  if(rainDrops[currentPosition] == 0) {
    text_layer_set_text(s_raininfo_large_layer, raininfozero_layer_buffer);
    text_layer_set_text(s_raininfo_small_layer, "");
  }else{
    text_layer_set_text(s_raininfo_large_layer, raininfo_layer_buffer);
    text_layer_set_text(s_raininfo_small_layer, rainUnit);
  }
}

static void set_current_raintime() {
  static char raintime_layer_buffer[6];
  static char ampm_layer_buffer[3];

  snprintf(raintime_layer_buffer, sizeof(raintime_layer_buffer), rainTimes[currentPosition]);

  if (ampms[currentPosition] == 1) {
    snprintf(ampm_layer_buffer, sizeof(ampm_layer_buffer), "AM");
  } else if (ampms[currentPosition] == 2) {
    snprintf(ampm_layer_buffer, sizeof(ampm_layer_buffer), "PM");
  } else {
    snprintf(ampm_layer_buffer, sizeof(ampm_layer_buffer), " ");
  }
  
  if(currentPosition == nowPointer) {
    text_layer_set_text(s_currenttime_layer, "Now");
  }else{
    text_layer_set_text(s_time_layer, raintime_layer_buffer);
    text_layer_set_text(s_ampm_layer, ampm_layer_buffer);
  }

  update_time();
}

static void set_current_date() {  
  static char date_layer_buffer[6];
  snprintf(date_layer_buffer, sizeof(date_layer_buffer), dates[currentPosition]);
  text_layer_set_text(s_date_layer, date_layer_buffer);
}

static void set_current_temp() {
  static char temp_layer_buffer[6];
  snprintf(temp_layer_buffer, sizeof(temp_layer_buffer), temps[currentPosition]);
  text_layer_set_text(s_temp_layer, temp_layer_buffer);
}

static void getJsData() {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  if(getLongTerm) {
    dict_write_uint8(iter, 10, 1);
  }else{
    dict_write_uint8(iter, 10, 0);
  }

  dict_write_uint8(iter, 11, clock_is_24h_style() ?1:0);

  // Send the message!
  app_message_outbox_send();

  // Set loading text
  text_layer_set_text(s_currenttime_layer, "Loading");
}

static void dataProcessor(Tuple *dataDrop, Tuple *dataPerHour, Tuple *dataTime, Tuple *dataAmpm, Tuple *dataDate, Tuple *dataTemp) {

  rainDrops[currentDataIterator] = dataDrop->value->int8;
  ampms[currentDataIterator] = dataAmpm->value->int8;

  snprintf(rainPerH[currentDataIterator],  sizeof(rainPerH[0]),  "%s", dataPerHour->value->cstring);
  rainPerHour[currentDataIterator] = rainPerH[currentDataIterator];
  snprintf(rainTime[currentDataIterator],  sizeof(rainTime[0]),  "%s", dataTime->value->cstring);
  rainTimes[currentDataIterator] = rainTime[currentDataIterator];
  snprintf(date[currentDataIterator],  sizeof(date[0]),  "%s", dataDate->value->cstring);
  dates[currentDataIterator] = date[currentDataIterator];
  snprintf(temp[currentDataIterator],  sizeof(temp[0]),  "%s", dataTemp->value->cstring);
  temps[currentDataIterator] = temp[currentDataIterator];

  APP_LOG(APP_LOG_LEVEL_INFO, "current iterator: %d", currentDataIterator);
  APP_LOG(APP_LOG_LEVEL_INFO, "current amount: %d", amountOfData);
  
  currentDataIterator = currentDataIterator + 1;

  if (currentDataIterator == amountOfData) {   

    if(!positionChanged) {
      currentPosition = nowPointer;
    }
     
    set_current_raininfo();
    set_current_raintime();
    set_current_date();
    set_current_temp();   

    currentDataIterator = 0;

    APP_LOG(APP_LOG_LEVEL_INFO, "after fetching data: %d", heap_bytes_free());
  }

}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Store incoming information
  static char location_layer_buffer[32];
  static char location_buffer[32];
  static char rainunit_buffer[32];

  // Read tuples for data
  Tuple *ready_tuple = dict_find(iterator, KEY_READY);
  Tuple *location_tuple = dict_find(iterator, KEY_LOCATION);
  Tuple *rainDrops_tuple = dict_find(iterator, KEY_RAINDROPS);
  Tuple *rainTimes_tuple = dict_find(iterator, KEY_RAINTIMES);
  Tuple *rainPerHour_tuple = dict_find(iterator, KEY_RAINPERHOUR);
  Tuple *rainUnit_tuple = dict_find(iterator, KEY_RAINUNIT);
  Tuple *now_tuple = dict_find(iterator, KEY_NOW);
  Tuple *refresh_tuple = dict_find(iterator, KEY_REFRESH);
  Tuple *dates_tuple = dict_find(iterator, KEY_DATE);
  Tuple *temps_tuple = dict_find(iterator, KEY_TEMP);
  Tuple *amount_tuple = dict_find(iterator, KEY_LONGTERM);
  Tuple *ampm_tuple = dict_find(iterator, KEY_24H);

  // If location is available, use it
  if(location_tuple) {
    snprintf(location_buffer, sizeof(location_buffer), "%s", location_tuple->value->cstring);

    // Assemble full string and display
    snprintf(location_layer_buffer, sizeof(location_layer_buffer), "%s", location_buffer);
    text_layer_set_text(s_location_layer, location_layer_buffer);
  }

  // If rain unit is available, use it
  if(rainUnit_tuple) {
    snprintf(rainunit_buffer, sizeof(rainunit_buffer), "%s", rainUnit_tuple->value->cstring);

    rainUnit = rainunit_buffer;
  }

  // If now is available, use it
  if(now_tuple) {
    nowPointer = now_tuple->value->int8;
  }

  // If refresh is available, use it
  if(refresh_tuple) {
    refreshPointer = refresh_tuple->value->int8;
  }

  // If refresh is available, use it
  if(amount_tuple) {
    currentDataIterator = 0;
    amountOfData = amount_tuple->value->int8;
  }

  // if ready, request first call
  if(ready_tuple) {
    APP_LOG(APP_LOG_LEVEL_INFO, "I'm ready");

    getJsData();
  }

  // If rain drops is available, use it
  if(rainDrops_tuple && rainPerHour_tuple && rainTimes_tuple && ampm_tuple && dates_tuple && temps_tuple) {
    dataProcessor(rainDrops_tuple, rainPerHour_tuple, rainTimes_tuple, ampm_tuple, dates_tuple, temps_tuple);
  }

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Inbox send fail!");
  text_layer_set_text(s_currenttime_layer, "Connection error");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send fail!");
  text_layer_set_text(s_currenttime_layer, "Connection error");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // Get weather update every XXX minutes
  if(refreshPointer != 0) {
    if(tick_time->tm_min % refreshPointer == 0) {
      getJsData();
    }
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (amountOfData > 0) {
    if (currentPosition > 0) {
      currentPosition--;
      positionChanged = true;

      set_current_raininfo();
      set_current_raintime();
      set_current_date();
      set_current_temp();
    }    
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (amountOfData > 0) {
    currentPosition++;
    positionChanged = true;
    if (currentPosition > amountOfData - 1) {
      currentPosition = amountOfData - 1;
    }

    set_current_raininfo();
    set_current_raintime();
    set_current_date();
    set_current_temp();
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  getJsData();
}

static void select_long_click_press_handler(ClickRecognizerRef recognizer, void *context) {

  // switch long term
  if (getLongTerm) {
    getLongTerm = false;
  } else {
    getLongTerm = true;
  }

  getJsData();
}

static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  // APP_LOG(APP_LOG_LEVEL_INFO, "LONG CLICK RELEASE");
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);

  // long pressing the buttons
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down_click_handler);

  // long pressing the select button
  window_long_click_subscribe(BUTTON_ID_SELECT, 500, select_long_click_press_handler, select_long_click_release_handler);
}

static void draw_main_ui(Layer *this_layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(this_layer);

  // Get the center of the screen (non full-screen)
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

  // Draw the circles
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, center, 30);

  #if defined(PBL_COLOR)
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorWindsorTan, GColorWhite));
    graphics_draw_circle(ctx, center, 45);

    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
    graphics_draw_circle(ctx, center, PBL_IF_ROUND_ELSE(center.x-20, center.y-20));
  #endif
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create GFont
  s_unibody_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UNIBODY_FONT_12));

  // Create graph
  s_graph_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, s_graph_layer);

  // Set the update_proc
  layer_set_update_proc(s_graph_layer, draw_graph);

  // Create main ui
  s_main_ui_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, s_main_ui_layer);

  // Set the update_proc
  layer_set_update_proc(s_main_ui_layer, draw_main_ui);

  // Create line layer
  s_pointer_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, s_pointer_layer);

  // Set the update_proc
  layer_set_update_proc(s_pointer_layer, draw_line);

  // Create the current time with specific bounds
  s_currenttime_layer = text_layer_create(
      GRect(0, 10, bounds.size.w, 20));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_currenttime_layer, GColorClear);
  text_layer_set_text_color(s_currenttime_layer, GColorWhite);
  if (connection_service_peek_pebble_app_connection()) {
    text_layer_set_text(s_currenttime_layer, "Loading");
  }else{
    text_layer_set_text(s_currenttime_layer, "No connection");
  }
  text_layer_set_text_alignment(s_currenttime_layer, GTextAlignmentCenter);
  text_layer_set_font(s_currenttime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_currenttime_layer));

  // Create location Layer
  s_location_layer = text_layer_create(
      GRect(0, bounds.size.h - 30, bounds.size.w, 20));

  // Style the text
  text_layer_set_background_color(s_location_layer, GColorClear);
  text_layer_set_text_color(s_location_layer, GColorWhite);
  text_layer_set_text_alignment(s_location_layer, GTextAlignmentCenter);
  text_layer_set_text(s_location_layer, "");
  text_layer_set_font(s_location_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_location_layer));// Create location Layer
  
  // create mm/u small layer
  s_raininfo_small_layer = text_layer_create(
      GRect(0, bounds.size.h/2 + 15, bounds.size.w, 20));

  // Style the text
  text_layer_set_background_color(s_raininfo_small_layer, GColorClear);
  text_layer_set_text_color(s_raininfo_small_layer, GColorBlack);
  text_layer_set_text_alignment(s_raininfo_small_layer, GTextAlignmentCenter);
  text_layer_set_text(s_raininfo_small_layer, "");
  text_layer_set_font(s_raininfo_small_layer, s_unibody_font);

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_raininfo_small_layer));

  // create mm/u large layer
  s_raininfo_large_layer = text_layer_create(
      GRect(0, bounds.size.h/2 + 5, bounds.size.w, 20));

  // Style the text
  text_layer_set_background_color(s_raininfo_large_layer, GColorClear);
  text_layer_set_text_color(s_raininfo_large_layer, GColorBlack);
  text_layer_set_text_alignment(s_raininfo_large_layer, GTextAlignmentCenter);
  text_layer_set_text(s_raininfo_large_layer, "");
  text_layer_set_font(s_raininfo_large_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_raininfo_large_layer));

  // create time layer
  s_time_layer = text_layer_create(
      GRect(0, bounds.size.h/2 - 22, bounds.size.w, 30));

  // Style the text
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer, "");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // create date layer
  s_date_layer = text_layer_create(
      GRect(0, bounds.size.h/2 - 29, bounds.size.w, 20));

  // Style the text
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));

  // create temp layer
  s_temp_layer = text_layer_create(
      GRect(10, bounds.size.h/2 - 10, 90, 20));

  // Style the text
  text_layer_set_background_color(s_temp_layer, GColorClear);
  text_layer_set_text_color(s_temp_layer, GColorWhite);
  text_layer_set_text_alignment(s_temp_layer, GTextAlignmentLeft);
  text_layer_set_text(s_temp_layer, "");
  text_layer_set_font(s_temp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_temp_layer));

  // create ampm layer
  s_ampm_layer = text_layer_create(
      GRect(bounds.size.w - 100, bounds.size.h/2 - 10, 90, 20));

  // Style the text
  text_layer_set_background_color(s_ampm_layer, GColorClear);
  text_layer_set_text_color(s_ampm_layer, GColorWhite);
  text_layer_set_text_alignment(s_ampm_layer, GTextAlignmentRight);
  text_layer_set_text(s_ampm_layer, "");
  text_layer_set_font(s_ampm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_ampm_layer));


  APP_LOG(APP_LOG_LEVEL_INFO, "after initial launch: %d", heap_bytes_free());
}

static void main_window_unload(Window *window) {
  // Destroy current time layer
  text_layer_destroy(s_currenttime_layer);

  // Destroy time layer
  text_layer_destroy(s_time_layer);

  // Destroy date layer
  text_layer_destroy(s_date_layer);

  // Destroy temp layer
  text_layer_destroy(s_temp_layer);

  // Destroy ampm layer
  text_layer_destroy(s_ampm_layer);

  // Destroy rain info layers
  text_layer_destroy(s_raininfo_small_layer);
  text_layer_destroy(s_raininfo_large_layer);

  // Destroy location elements
  text_layer_destroy(s_location_layer);

  // Destroy main ui layer
  layer_destroy(s_main_ui_layer);

  // Destroy graph layer
  layer_destroy(s_graph_layer);

  // Unload font
  fonts_unload_custom_font(s_unibody_font);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set the background color
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Set click handlers
  window_set_click_config_provider(s_main_window, click_config_provider);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(256, 256);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
