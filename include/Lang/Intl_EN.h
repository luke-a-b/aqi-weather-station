const char INTL_LANG_ID[] PROGMEM = "en";

const char INTL_SUND[] PROGMEM = "Sun.";
const char INTL_MOND[] PROGMEM = "Mon.";
const char INTL_TUED[] PROGMEM = "Tue.";
const char INTL_WEDD[] PROGMEM = "Wed.";
const char INTL_THUD[] PROGMEM = "Thu.";
const char INTL_FRID[] PROGMEM = "Fri.";
const char INTL_SATD[] PROGMEM = "Sat.";

const char INTL_SUNDAY[] PROGMEM = "Sunday";
const char INTL_MONDAY[] PROGMEM = "Monday";
const char INTL_TUEDAY[] PROGMEM = "Tuesday";
const char INTL_WEDDAY[] PROGMEM = "Wednesday";
const char INTL_THUDAY[] PROGMEM = "Thuresday";
const char INTL_FRIDAY[] PROGMEM = "Fieday";
const char INTL_SATDAY[] PROGMEM = "Saturday";

const char INTL_JANUARY[] PROGMEM = "January";
const char INTL_FEBRUARY[] PROGMEM = "Februay";
const char INTL_MARCH[] PROGMEM = "March";
const char INTL_APRIL[] PROGMEM = "April";
const char INTL_MAY[] PROGMEM = "May";
const char INTL_JUNE[] PROGMEM = "June";
const char INTL_JULY[] PROGMEM = "July";
const char INTL_AUGUST[] PROGMEM = "August";
const char INTL_SEPTEMBER[] PROGMEM = "September";
const char INTL_OCTOBER[] PROGMEM = "October";
const char INTL_NOVEMBER[] PROGMEM = "November";
const char INTL_DECEMBER[] PROGMEM = "December";

const char INTL_MOON[] PROGMEM = "Moon";
const char INTL_SUN[] PROGMEM = "Sun";

const char INTL_PHASE_0[] PROGMEM = "New Moon";
const char INTL_PHASE_1[] PROGMEM = "Waxing Crescent";
const char INTL_PHASE_2[] PROGMEM = "First Quarter";
const char INTL_PHASE_3[] PROGMEM = "Waxing Gibbous";
const char INTL_PHASE_4[] PROGMEM = "Full Moon";
const char INTL_PHASE_5[] PROGMEM = "Waning Gibbous";
const char INTL_PHASE_6[] PROGMEM = "Last/Third Quarter";
const char INTL_PHASE_7[] PROGMEM = "Waning Crescent";

// below strings are not to be stored in PROGMEM
#define INTL_IS_METRIC_SYSTEM PSTR("Is metric system?")
#define INTL_IS_CLOCK_24H_STYLE PSTR("Is clock 24h style?")
#define INTL_IS_CLOCK_SILHOUETTE_ENABLED PSTR("Is clock silhouette enabled?")
#define INTL_BACKLIGHT_TIMEOUT PSTR("Backlight timeout - 0 = always enabled")
#define INTL_METEO_ICONS PSTR("Meteo icons")
#define INTL_OWM_SETTINGS PSTR("Open Weather Map settings")
#define INTL_OWM_API_KEY PSTR("Open Weather Map API key")
#define INTL_OWM_LATITUDE PSTR("Latitude")
#define INTL_OWM_LONGITUDE PSTR("Longitude")
#define INTL_OWM_REFRESH_INTERVAL                                              \
  PSTR("Open Weather Map refresh data interval seconds")
#define INTL_LOCATAION_NAME PSTR("Location name")
#define INTL_AQI_SETTINGS PSTR("Air Quality Index stations settings")
#define INTL_AQI_URL PSTR("AQI station URL")
#define INTL_AQI_REFRESH_INTERVAL PSTR("AQI refresh data interval seconds")
#define INTL_LOCAL_TEMP_SETTINGS                                               \
  PSTR("Local temperature/humidity sensor settings")
#define INTL_LOCAL_TEMP_SENSOR_TYPE PSTR("Select type of connected sensor: ")
#define INTL_LOCAL_TEMP_REFRESH_INTERVAL                                       \
  PSTR("Local sensor refresh data interval seconds")

#define INTL_OWM_API_KEY_ERROR PSTR("Insert correct OWM API Key")
#define INTL_LOCATION_ERROR PSTR("Insert location name")
#define INTL_AQI_URL_ERROR                                                     \
  PSTR("Insert URL in following format: http://.../data.json or "              \
       "https://.../data.json")

const char INTL_HTML_GREETINGS[] PROGMEM = "<p>Welcome to {v}!</p>\n";
const char INTL_HTML_CONFIG_LINK[] PROGMEM =
    "<p>Go to <a href='config'>configure page</a> to change settings.</p>\n";
const char INTL_HTML_FORM_END[] PROGMEM =
    "<button type='submit' style='margin-top: 10px;'>Apply</button></form>\n";
const char INTL_HTML_SAVED[] PROGMEM = "<div>Configuration saved<br />Return "
                                       "to <a href='/'>home page</a>.</div>\n";
const char INTL_HTML_UPDATE[] PROGMEM =
    "<div style='padding-top:25px;'><a href='{u}'>Firmware update</a></div>\n";
const char INTL_HTML_CONFIG_VER[] PROGMEM =
    "<div style='font-size: .6em;'>Firmware config version '{v}'</div>\n";

//////////////////////

const char INTL_IP_ADDRESS[] PROGMEM = "IP address:";
const char INTL_SUBNET_MASK[] PROGMEM = "Subnet mask:";
const char INTL_GATEWAY[] PROGMEM = "Gateway:";
const char INTL_MAC_ADDRESS[] PROGMEM = "Ades MAC:";
const char INTL_SSID[] PROGMEM = "MAC address:";
const char INTL_RSSI[] PROGMEM = "RSSI:";
const char INTL_WIFI_QALITY[] PROGMEM = "Signal:";
const char INTL_HEAP_MEM[] PROGMEM = "Heap memory:";
const char INTL_FLASH_MEM[] PROGMEM = "Flash memory:";
const char INTL_CHIP_ID[] PROGMEM = "Chip ID:";
const char INTL_VCC[] PROGMEM = "Vcc:";
const char INTL_CPU_FREQ[] PROGMEM = "CPU frequency:";
const char INTL_UPTIME[] PROGMEM = "Uptime:";
const char INTL_LAST_RESET[] PROGMEM = "Last reset:";

const char INTL_NO_DATA[] PROGMEM = "No data";
const char INTL_AQI_GOOD[] PROGMEM = "Good";
const char INTL_AQI_MODERATE[] PROGMEM = "Moderate";
const char INTL_AQI_UNHEALTHY_SENSITIVE[] PROGMEM =
    "Unhealthy for Sensitive Groups";
const char INTL_AQI_UNHEALTHY[] PROGMEM = "Unhealthy";
const char INTL_AQI_VERY_UNHEALTHY[] PROGMEM = "Very Unhealthy";
const char INTL_AQI_HAZARDOUS[] PROGMEM = "Hazardous";

const char INTL_CURRENT_CONDITIONS[] PROGMEM = "Current Conditions";
const char INTL_TEMPERATURE_MINMAX[] PROGMEM = "Temp. Min/Max:";
const char INTL_TEMPERATURE_FEELSLIKE[] PROGMEM = "Feelslike.:";
const char INTL_TEMPERATURE_DEW_POINT[] PROGMEM = "Dew Point:";
const char INTL_WIND_SPEED[] PROGMEM = "Wind Speed:";
const char INTL_WIND_DEGREE[] PROGMEM = "Wind Dir:";
const char INTL_RAIN_SNOW[] PROGMEM = "Rain/Snow:";
const char INTL_CLOUDS[] PROGMEM = "Clouds:";
const char INTL_UVI[] PROGMEM = "Index UV:";
const char INTL_VISIBILITY[] PROGMEM = "Visibility:";
const char INTL_AIR_QUALITY[] PROGMEM = "Air Quality:";

const char INTL_ORGINAL[] PROGMEM = "Oryginal: blog.squix.org, trekawek";
const char INTL_ADAPTEDBY[] PROGMEM = "Adopted by: Lukasz Baran";

const char INTL_MOUNTING_FILE_SYSTEM[] PROGMEM = "Mounting file system";
const char INTL_FORMATTING_FILE_SYSTEM[] PROGMEM = "Formatting file system";
const char INTL_SETTING_TOUCH_CALIBRATION[] PROGMEM =
    "Calibrating touch screen";
const char INTL_PREPARING_OBJECTS[] PROGMEM = "Preparing objects";
const char INTL_CONNECTED[] PROGMEM = "Connected: ";
const char INTL_UPDATING_DATA[] PROGMEM = "Fetching data";
const char INTL_DONE[] PROGMEM = "Done!";
const char INTL_CONNECTING_TO[] PROGMEM = "Connecting to ";
const char INTL_NO_CONNECTION[] PROGMEM = "No WiFi - check config";
const char INTL_TOUCH_CORNERS[] PROGMEM = "Touch corners as indicated";
const char INTL_CALIBRATION_COMPLETE[] PROGMEM = "Calibration complete!";

const char INTL_FORECASTS[] PROGMEM = "Forecasts";
