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

static char rainPerH0[6];
static char rainPerH1[6];
static char rainPerH2[6];
static char rainPerH3[6];
static char rainPerH4[6];
static char rainPerH5[6];
static char rainPerH6[6];
static char rainPerH7[6];
static char rainPerH8[6];
static char rainPerH9[6];
static char rainPerH10[6];
static char rainPerH11[6];
static char rainPerH12[6];
static char rainPerH13[6];
static char rainPerH14[6];
static char rainPerH15[6];
static char rainPerH16[6];
static char rainPerH17[6];
static char rainPerH18[6];
static char rainPerH19[6];
static char rainPerH20[6];
static char rainPerH21[6];
static char rainPerH22[6];
static char rainPerH23[6];
static char rainPerH24[6];
static char rainPerH25[6];
static char rainPerH26[6];
static char rainPerH27[6];
static char rainPerH28[6];
static char rainPerH29[6];
static char rainPerH30[6];
static char rainPerH31[6];
static char rainPerH32[6];
static char rainPerH33[6];
static char rainPerH34[6];
static char rainPerH35[6];
static char rainPerH36[6];
static char rainPerH37[6];
static char rainPerH38[6];
static char rainPerH39[6];
static char rainPerH40[6];
static char rainPerH41[6];
static char rainPerH42[6];
static char rainPerH43[6];
static char rainPerH44[6];
static char rainPerH45[6];
static char rainPerH46[6];
static char rainPerH47[6];
static char rainPerH48[6];
static char rainPerH49[6];
static char rainPerH50[6];
static char rainPerH51[6];
static char rainPerH52[6];
static char rainPerH53[6];
static char rainPerH54[6];
static char rainPerH55[6];
static char rainPerH56[6];
static char rainPerH57[6];
static char rainPerH58[6];
static char rainPerH59[6];
static char rainPerH60[6];

static char rainTime0[6];
static char rainTime1[6];
static char rainTime2[6];
static char rainTime3[6];
static char rainTime4[6];
static char rainTime5[6];
static char rainTime6[6];
static char rainTime7[6];
static char rainTime8[6];
static char rainTime9[6];
static char rainTime10[6];
static char rainTime11[6];
static char rainTime12[6];
static char rainTime13[6];
static char rainTime14[6];
static char rainTime15[6];
static char rainTime16[6];
static char rainTime17[6];
static char rainTime18[6];
static char rainTime19[6];
static char rainTime20[6];
static char rainTime21[6];
static char rainTime22[6];
static char rainTime23[6];
static char rainTime24[6];
static char rainTime25[6];
static char rainTime26[6];
static char rainTime27[6];
static char rainTime28[6];
static char rainTime29[6];
static char rainTime30[6];
static char rainTime31[6];
static char rainTime32[6];
static char rainTime33[6];
static char rainTime34[6];
static char rainTime35[6];
static char rainTime36[6];
static char rainTime37[6];
static char rainTime38[6];
static char rainTime39[6];
static char rainTime40[6];
static char rainTime41[6];
static char rainTime42[6];
static char rainTime43[6];
static char rainTime44[6];
static char rainTime45[6];
static char rainTime46[6];
static char rainTime47[6];
static char rainTime48[6];
static char rainTime49[6];
static char rainTime50[6];
static char rainTime51[6];
static char rainTime52[6];
static char rainTime53[6];
static char rainTime54[6];
static char rainTime55[6];
static char rainTime56[6];
static char rainTime57[6];
static char rainTime58[6];
static char rainTime59[6];
static char rainTime60[6];

static char temp0[6];
static char temp1[6];
static char temp2[6];
static char temp3[6];
static char temp4[6];
static char temp5[6];
static char temp6[6];
static char temp7[6];
static char temp8[6];
static char temp9[6];
static char temp10[6];
static char temp11[6];
static char temp12[6];
static char temp13[6];
static char temp14[6];
static char temp15[6];
static char temp16[6];
static char temp17[6];
static char temp18[6];
static char temp19[6];
static char temp20[6];
static char temp21[6];
static char temp22[6];
static char temp23[6];
static char temp24[6];
static char temp25[6];
static char temp26[6];
static char temp27[6];
static char temp28[6];
static char temp29[6];
static char temp30[6];
static char temp31[6];
static char temp32[6];
static char temp33[6];
static char temp34[6];
static char temp35[6];
static char temp36[6];
static char temp37[6];
static char temp38[6];
static char temp39[6];
static char temp40[6];
static char temp41[6];
static char temp42[6];
static char temp43[6];
static char temp44[6];
static char temp45[6];
static char temp46[6];
static char temp47[6];
static char temp48[6];
static char temp49[6];
static char temp50[6];
static char temp51[6];
static char temp52[6];
static char temp53[6];
static char temp54[6];
static char temp55[6];
static char temp56[6];
static char temp57[6];
static char temp58[6];
static char temp59[6];
static char temp60[6];

static char date0[6];
static char date1[6];
static char date2[6];
static char date3[6];
static char date4[6];
static char date5[6];
static char date6[6];
static char date7[6];
static char date8[6];
static char date9[6];
static char date10[6];
static char date11[6];
static char date12[6];
static char date13[6];
static char date14[6];
static char date15[6];
static char date16[6];
static char date17[6];
static char date18[6];
static char date19[6];
static char date20[6];
static char date21[6];
static char date22[6];
static char date23[6];
static char date24[6];
static char date25[6];
static char date26[6];
static char date27[6];
static char date28[6];
static char date29[6];
static char date30[6];
static char date31[6];
static char date32[6];
static char date33[6];
static char date34[6];
static char date35[6];
static char date36[6];
static char date37[6];
static char date38[6];
static char date39[6];
static char date40[6];
static char date41[6];
static char date42[6];
static char date43[6];
static char date44[6];
static char date45[6];
static char date46[6];
static char date47[6];
static char date48[6];
static char date49[6];
static char date50[6];
static char date51[6];
static char date52[6];
static char date53[6];
static char date54[6];
static char date55[6];
static char date56[6];
static char date57[6];
static char date58[6];
static char date59[6];
static char date60[6];

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

  switch(currentDataIterator) {
     case 0  :
        snprintf(rainPerH0,  sizeof(rainPerH1),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH0;
        snprintf(rainTime0,  sizeof(rainTime0),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime0;
        snprintf(date0,  sizeof(date0),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date0;
        snprintf(temp0,  sizeof(temp0),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp0;
        break; 
    
     case 1  :
        snprintf(rainPerH1,  sizeof(rainPerH1),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH1;
        snprintf(rainTime1,  sizeof(rainTime1),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime1;
        snprintf(date1,  sizeof(date1),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date1;
        snprintf(temp1,  sizeof(temp1),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp1;
        break; 
    
     case 2  :
        snprintf(rainPerH2,  sizeof(rainPerH2),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH2;
        snprintf(rainTime2,  sizeof(rainTime2),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime2;
        snprintf(date2,  sizeof(date2),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date2;
        snprintf(temp2,  sizeof(temp2),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp2;
        break; 
    
     case 3  :
        snprintf(rainPerH3,  sizeof(rainPerH3),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH3;
        snprintf(rainTime3,  sizeof(rainTime3),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime3;
        snprintf(date3,  sizeof(date3),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date3;
        snprintf(temp3,  sizeof(temp3),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp3;
        break; 
    
     case 4  :
        snprintf(rainPerH4,  sizeof(rainPerH4),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH4;
        snprintf(rainTime4,  sizeof(rainTime4),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime4;
        snprintf(date4,  sizeof(date4),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date4;
        snprintf(temp4,  sizeof(temp4),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp4;
        break; 
    
     case 5  :
        snprintf(rainPerH5,  sizeof(rainPerH5),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH5;
        snprintf(rainTime5,  sizeof(rainTime5),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime5;
        snprintf(date5,  sizeof(date5),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date5;
        snprintf(temp5,  sizeof(temp5),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp5;
        break; 
    
     case 6  :
        snprintf(rainPerH6,  sizeof(rainPerH6),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH6;
        snprintf(rainTime6,  sizeof(rainTime6),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime6;
        snprintf(date6,  sizeof(date6),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date6;
        snprintf(temp6,  sizeof(temp6),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp6;
        break; 
    
     case 7  :
        snprintf(rainPerH7,  sizeof(rainPerH7),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH7;
        snprintf(rainTime7,  sizeof(rainTime7),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime7;
        snprintf(date7,  sizeof(date7),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date7;
        snprintf(temp7,  sizeof(temp7),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp7;
        break; 
    
     case 8  :
        snprintf(rainPerH8,  sizeof(rainPerH8),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH8;
        snprintf(rainTime8,  sizeof(rainTime8),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime8;
        snprintf(date8,  sizeof(date8),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date8;
        snprintf(temp8,  sizeof(temp8),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp8;
        break; 
    
     case 9  :
        snprintf(rainPerH9,  sizeof(rainPerH9),  "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH9;
        snprintf(rainTime9,  sizeof(rainTime9),  "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime9;
        snprintf(date9,  sizeof(date9),  "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date9;
        snprintf(temp9,  sizeof(temp9),  "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp9;
        break; 
    
     case 10 :
        snprintf(rainPerH10, sizeof(rainPerH10), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH10;
        snprintf(rainTime10, sizeof(rainTime10), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime10;
        snprintf(date10, sizeof(date10), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date10;
        snprintf(temp10, sizeof(temp10), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp10;
        break; 
    
     case 11 :
        snprintf(rainPerH11, sizeof(rainPerH11), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH11;
        snprintf(rainTime11, sizeof(rainTime11), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime11;
        snprintf(date11, sizeof(date11), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date11;
        snprintf(temp11, sizeof(temp11), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp11;
        break; 
    
     case 12 :
        snprintf(rainPerH12, sizeof(rainPerH12), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH12;
        snprintf(rainTime12, sizeof(rainTime12), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime12;
        snprintf(date12, sizeof(date12), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date12;
        snprintf(temp12, sizeof(temp12), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp12;
        break; 
    
     case 13 :
        snprintf(rainPerH13, sizeof(rainPerH13), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH13;
        snprintf(rainTime13, sizeof(rainTime13), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime13;
        snprintf(date13, sizeof(date13), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date13;
        snprintf(temp13, sizeof(temp13), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp13;
        break; 
    
     case 14 :
        snprintf(rainPerH14, sizeof(rainPerH14), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH14;
        snprintf(rainTime14, sizeof(rainTime14), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime14;
        snprintf(date14, sizeof(date14), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date14;
        snprintf(temp14, sizeof(temp14), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp14;
        break; 
    
     case 15 :
        snprintf(rainPerH15, sizeof(rainPerH15), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH15;
        snprintf(rainTime15, sizeof(rainTime15), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime15;
        snprintf(date15, sizeof(date15), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date15;
        snprintf(temp15, sizeof(temp15), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp15;
        break; 
    
     case 16 :
        snprintf(rainPerH16, sizeof(rainPerH16), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH16;
        snprintf(rainTime16, sizeof(rainTime16), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime16;
        snprintf(date16, sizeof(date16), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date16;
        snprintf(temp16, sizeof(temp16), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp16;
        break; 
    
     case 17 :
        snprintf(rainPerH17, sizeof(rainPerH17), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH17;
        snprintf(rainTime17, sizeof(rainTime17), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime17;
        snprintf(date17, sizeof(date17), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date17;
        snprintf(temp17, sizeof(temp17), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp17;
        break; 
    
     case 18 :
        snprintf(rainPerH18, sizeof(rainPerH18), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH18;
        snprintf(rainTime18, sizeof(rainTime18), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime18;
        snprintf(date18, sizeof(date18), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date18;
        snprintf(temp18, sizeof(temp18), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp18;
        break; 
    
     case 19 :
        snprintf(rainPerH19, sizeof(rainPerH19), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH19;
        snprintf(rainTime19, sizeof(rainTime19), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime19;
        snprintf(date19, sizeof(date19), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date19;
        snprintf(temp19, sizeof(temp19), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp19;
        break; 
    
     case 20 :
        snprintf(rainPerH20, sizeof(rainPerH20), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH20;
        snprintf(rainTime20, sizeof(rainTime20), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime20;
        snprintf(date20, sizeof(date20), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date20;
        snprintf(temp20, sizeof(temp20), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp20;
        break; 
    
     case 21 :
        snprintf(rainPerH21, sizeof(rainPerH21), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH21;
        snprintf(rainTime21, sizeof(rainTime21), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime21;
        snprintf(date21, sizeof(date21), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date21;
        snprintf(temp21, sizeof(temp21), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp21;
        break; 
    
     case 22 :
        snprintf(rainPerH22, sizeof(rainPerH22), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH22;
        snprintf(rainTime22, sizeof(rainTime22), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime22;
        snprintf(date22, sizeof(date22), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date22;
        snprintf(temp22, sizeof(temp22), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp22;
        break; 
    
     case 23 :
        snprintf(rainPerH23, sizeof(rainPerH23), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH23;
        snprintf(rainTime23, sizeof(rainTime23), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime23;
        snprintf(date23, sizeof(date23), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date23;
        snprintf(temp23, sizeof(temp23), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp23;
        break; 
    
     case 24 :
        snprintf(rainPerH24, sizeof(rainPerH24), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH24;
        snprintf(rainTime24, sizeof(rainTime24), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime24;
        snprintf(date24, sizeof(date24), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date24;
        snprintf(temp24, sizeof(temp24), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp24;
        break; 
    
     case 25 :
        snprintf(rainPerH25, sizeof(rainPerH25), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH25;
        snprintf(rainTime25, sizeof(rainTime25), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime25;
        snprintf(date25, sizeof(date25), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date25;
        snprintf(temp25, sizeof(temp25), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp25;
        break; 
    
     case 26 :
        snprintf(rainPerH26, sizeof(rainPerH26), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH26;
        snprintf(rainTime26, sizeof(rainTime26), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime26;
        snprintf(date26, sizeof(date26), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date26;
        snprintf(temp26, sizeof(temp26), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp26;
        break; 
    
     case 27 :
        snprintf(rainPerH27, sizeof(rainPerH27), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH27;
        snprintf(rainTime27, sizeof(rainTime27), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime27;
        snprintf(date27, sizeof(date27), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date27;
        snprintf(temp27, sizeof(temp27), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp27;
        break; 
    
     case 28 :
        snprintf(rainPerH28, sizeof(rainPerH28), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH28;
        snprintf(rainTime28, sizeof(rainTime28), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime28;
        snprintf(date28, sizeof(date28), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date28;
        snprintf(temp28, sizeof(temp28), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp28;
        break; 
    
     case 29 :
        snprintf(rainPerH29, sizeof(rainPerH29), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH29;
        snprintf(rainTime29, sizeof(rainTime29), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime29;
        snprintf(date29, sizeof(date29), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date29;
        snprintf(temp29, sizeof(temp29), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp29;
        break; 
    
     case 30 :
        snprintf(rainPerH30, sizeof(rainPerH30), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH30;
        snprintf(rainTime30, sizeof(rainTime30), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime30;
        snprintf(date30, sizeof(date30), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date30;
        snprintf(temp30, sizeof(temp30), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp30;
        break; 
    
     case 31 :
        snprintf(rainPerH31, sizeof(rainPerH31), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH31;
        snprintf(rainTime31, sizeof(rainTime31), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime31;
        snprintf(date31, sizeof(date31), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date31;
        snprintf(temp31, sizeof(temp31), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp31;
        break; 
    
     case 32 :
        snprintf(rainPerH32, sizeof(rainPerH32), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH32;
        snprintf(rainTime32, sizeof(rainTime32), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime32;
        snprintf(date32, sizeof(date32), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date32;
        snprintf(temp32, sizeof(temp32), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp32;
        break; 
    
     case 33 :
        snprintf(rainPerH33, sizeof(rainPerH33), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH33;
        snprintf(rainTime33, sizeof(rainTime33), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime33;
        snprintf(date33, sizeof(date33), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date33;
        snprintf(temp33, sizeof(temp33), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp33;
        break; 
    
     case 34 :
        snprintf(rainPerH34, sizeof(rainPerH34), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH34;
        snprintf(rainTime34, sizeof(rainTime34), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime34;
        snprintf(date34, sizeof(date34), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date34;
        snprintf(temp34, sizeof(temp34), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp34;
        break; 
    
     case 35 :
        snprintf(rainPerH35, sizeof(rainPerH35), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH35;
        snprintf(rainTime35, sizeof(rainTime35), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime35;
        snprintf(date35, sizeof(date35), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date35;
        snprintf(temp35, sizeof(temp35), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp35;
        break; 
    
     case 36 :
        snprintf(rainPerH36, sizeof(rainPerH36), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH36;
        snprintf(rainTime36, sizeof(rainTime36), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime36;
        snprintf(date36, sizeof(date36), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date36;
        snprintf(temp36, sizeof(temp36), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp36;
        break; 
    
     case 37 :
        snprintf(rainPerH37, sizeof(rainPerH37), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH37;
        snprintf(rainTime37, sizeof(rainTime37), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime37;
        snprintf(date37, sizeof(date37), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date37;
        snprintf(temp37, sizeof(temp37), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp37;
        break; 
    
     case 38 :
        snprintf(rainPerH38, sizeof(rainPerH38), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH38;
        snprintf(rainTime38, sizeof(rainTime38), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime38;
        snprintf(date38, sizeof(date38), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date38;
        snprintf(temp38, sizeof(temp38), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp38;
        break; 
    
     case 39 :
        snprintf(rainPerH39, sizeof(rainPerH39), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH39;
        snprintf(rainTime39, sizeof(rainTime39), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime39;
        snprintf(date39, sizeof(date39), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date39;
        snprintf(temp39, sizeof(temp39), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp39;
        break; 
    
     case 40 :
        snprintf(rainPerH40, sizeof(rainPerH40), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH40;
        snprintf(rainTime40, sizeof(rainTime40), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime40;
        snprintf(date40, sizeof(date40), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date40;
        snprintf(temp40, sizeof(temp40), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp40;
        break; 
    
     case 41 :
        snprintf(rainPerH41, sizeof(rainPerH41), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH41;
        snprintf(rainTime41, sizeof(rainTime41), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime41;
        snprintf(date41, sizeof(date41), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date41;
        snprintf(temp41, sizeof(temp41), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp41;
        break; 
    
     case 42 :
        snprintf(rainPerH42, sizeof(rainPerH42), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH42;
        snprintf(rainTime42, sizeof(rainTime42), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime42;
        snprintf(date42, sizeof(date42), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date42;
        snprintf(temp42, sizeof(temp42), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp42;
        break; 
    
     case 43 :
        snprintf(rainPerH43, sizeof(rainPerH43), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH43;
        snprintf(rainTime43, sizeof(rainTime43), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime43;
        snprintf(date43, sizeof(date43), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date43;
        snprintf(temp43, sizeof(temp43), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp43;
        break; 
    
     case 44 :
        snprintf(rainPerH44, sizeof(rainPerH44), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH44;
        snprintf(rainTime44, sizeof(rainTime44), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime44;
        snprintf(date44, sizeof(date44), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date44;
        snprintf(temp44, sizeof(temp44), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp44;
        break; 
    
     case 45 :
        snprintf(rainPerH45, sizeof(rainPerH45), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH45;
        snprintf(rainTime45, sizeof(rainTime45), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime45;
        snprintf(date45, sizeof(date45), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date45;
        snprintf(temp45, sizeof(temp45), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp45;
        break; 
    
     case 46 :
        snprintf(rainPerH46, sizeof(rainPerH46), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH46;
        snprintf(rainTime46, sizeof(rainTime46), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime46;
        snprintf(date46, sizeof(date46), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date46;
        snprintf(temp46, sizeof(temp46), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp46;
        break; 
    
     case 47 :
        snprintf(rainPerH47, sizeof(rainPerH47), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH47;
        snprintf(rainTime47, sizeof(rainTime47), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime47;
        snprintf(date47, sizeof(date47), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date47;
        snprintf(temp47, sizeof(temp47), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp47;
        break; 
    
     case 48 :
        snprintf(rainPerH48, sizeof(rainPerH48), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH48;
        snprintf(rainTime48, sizeof(rainTime48), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime48;
        snprintf(date48, sizeof(date48), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date48;
        snprintf(temp48, sizeof(temp48), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp48;
        break; 
    
     case 49 :
        snprintf(rainPerH49, sizeof(rainPerH49), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH49;
        snprintf(rainTime49, sizeof(rainTime49), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime49;
        snprintf(date49, sizeof(date49), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date49;
        snprintf(temp49, sizeof(temp49), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp49;
        break; 
    
     case 50 :
        snprintf(rainPerH50, sizeof(rainPerH50), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH50;
        snprintf(rainTime50, sizeof(rainTime50), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime50;
        snprintf(date50, sizeof(date50), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date50;
        snprintf(temp50, sizeof(temp50), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp50;
        break; 
    
     case 51 :
        snprintf(rainPerH51, sizeof(rainPerH51), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH51;
        snprintf(rainTime51, sizeof(rainTime51), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime51;
        snprintf(date51, sizeof(date51), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date51;
        snprintf(temp51, sizeof(temp51), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp51;
        break; 
    
     case 52 :
        snprintf(rainPerH52, sizeof(rainPerH52), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH52;
        snprintf(rainTime52, sizeof(rainTime52), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime52;
        snprintf(date52, sizeof(date52), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date52;
        snprintf(temp52, sizeof(temp52), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp52;
        break; 
    
     case 53 :
        snprintf(rainPerH53, sizeof(rainPerH53), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH53;
        snprintf(rainTime53, sizeof(rainTime53), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime53;
        snprintf(date53, sizeof(date53), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date53;
        snprintf(temp53, sizeof(temp53), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp53;
        break; 
    
     case 54 :
        snprintf(rainPerH54, sizeof(rainPerH54), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH54;
        snprintf(rainTime54, sizeof(rainTime54), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime54;
        snprintf(date54, sizeof(date54), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date54;
        snprintf(temp54, sizeof(temp54), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp54;
        break; 
    
     case 55 :
        snprintf(rainPerH55, sizeof(rainPerH55), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH55;
        snprintf(rainTime55, sizeof(rainTime55), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime55;
        snprintf(date55, sizeof(date55), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date55;
        snprintf(temp55, sizeof(temp55), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp55;
        break; 
    
     case 56 :
        snprintf(rainPerH56, sizeof(rainPerH56), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH56;
        snprintf(rainTime56, sizeof(rainTime56), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime56;
        snprintf(date56, sizeof(date56), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date56;
        snprintf(temp56, sizeof(temp56), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp56;
        break; 
    
     case 57 :
        snprintf(rainPerH57, sizeof(rainPerH57), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH57;
        snprintf(rainTime57, sizeof(rainTime57), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime57;
        snprintf(date57, sizeof(date57), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date57;
        snprintf(temp57, sizeof(temp57), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp57;
        break; 
    
     case 58 :
        snprintf(rainPerH58, sizeof(rainPerH58), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH58;
        snprintf(rainTime58, sizeof(rainTime58), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime58;
        snprintf(date58, sizeof(date58), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date58;
        snprintf(temp58, sizeof(temp58), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp58;
        break; 
    
     case 59 :
        snprintf(rainPerH59, sizeof(rainPerH59), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH59;
        snprintf(rainTime59, sizeof(rainTime59), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime59;
        snprintf(date59, sizeof(date59), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date59;
        snprintf(temp59, sizeof(temp59), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp59;
        break; 
    
     case 60 :
        snprintf(rainPerH60, sizeof(rainPerH60), "%s", dataPerHour->value->cstring);
        rainPerHour[currentDataIterator] = rainPerH60;
        snprintf(rainTime60, sizeof(rainTime60), "%s", dataTime->value->cstring);
        rainTimes[currentDataIterator] = rainTime60;
        snprintf(date60, sizeof(date60), "%s", dataDate->value->cstring);
        dates[currentDataIterator] = date60;
        snprintf(temp60, sizeof(temp60), "%s", dataTemp->value->cstring);
        temps[currentDataIterator] = temp60;
        break; 
  }
  

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
