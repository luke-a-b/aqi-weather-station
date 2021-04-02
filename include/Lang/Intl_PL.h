const char INTL_LANG_ID[] PROGMEM = "pl";

const char INTL_SUND[] PROGMEM = "Niedz.";
const char INTL_MOND[] PROGMEM = "Pon.";
const char INTL_TUED[] PROGMEM = "Wt.";
const char INTL_WEDD[] PROGMEM = "Śr.";
const char INTL_THUD[] PROGMEM = "Czw.";
const char INTL_FRID[] PROGMEM = "Pt.";
const char INTL_SATD[] PROGMEM = "Sob.";

const char INTL_SUNDAY[] PROGMEM = "Niedziela";
const char INTL_MONDAY[] PROGMEM = "Poniedziałek";
const char INTL_TUEDAY[] PROGMEM = "Wtorek";
const char INTL_WEDDAY[] PROGMEM = "Środa";
const char INTL_THUDAY[] PROGMEM = "Czwartek";
const char INTL_FRIDAY[] PROGMEM = "Piątek";
const char INTL_SATDAY[] PROGMEM = "Sobota";

const char INTL_JANUARY[] PROGMEM = "Styczeń";
const char INTL_FEBRUARY[] PROGMEM = "Luty";
const char INTL_MARCH[] PROGMEM = "Marzec";
const char INTL_APRIL[] PROGMEM = "Kwiecień";
const char INTL_MAY[] PROGMEM = "Maj";
const char INTL_JUNE[] PROGMEM = "Czerwiec";
const char INTL_JULY[] PROGMEM = "Lipiec";
const char INTL_AUGUST[] PROGMEM = "Sierpień";
const char INTL_SEPTEMBER[] PROGMEM = "Wrzesień";
const char INTL_OCTOBER[] PROGMEM = "Październik";
const char INTL_NOVEMBER[] PROGMEM = "Listopad";
const char INTL_DECEMBER[] PROGMEM = "Grudzień";

const char INTL_MOON[] PROGMEM = "Księżyc";
const char INTL_SUN[] PROGMEM = "Słońce";

const char INTL_PHASE_0[] PROGMEM = "Nów";
const char INTL_PHASE_1[] PROGMEM = "Wzrastający Sierp";
const char INTL_PHASE_2[] PROGMEM = "Pierwsza Kwadra";
const char INTL_PHASE_3[] PROGMEM = "Poszerzony";
const char INTL_PHASE_4[] PROGMEM = "Pełnia";
const char INTL_PHASE_5[] PROGMEM = "Garbaty";
const char INTL_PHASE_6[] PROGMEM = "Trzecia Kwadra";
const char INTL_PHASE_7[] PROGMEM = "Malejący Sierp";

// below strings are used web config
#define INTL_IS_METRIC_SYSTEM PSTR("Czy używać system metrzyczny?")
#define INTL_IS_CLOCK_24H_STYLE PSTR("Czy używać stylu zegara 24h?")
#define INTL_IS_CLOCK_SILHOUETTE_ENABLED PSTR("Czy włączyć poświatę zegara?")
#define INTL_BACKLIGHT_TIMEOUT                                                 \
  PSTR("Czas podświetlenia ekranu - 0 = zawsze właczony")
#define INTL_METEO_ICONS PSTR("Ikony meteo")
#define INTL_OWM_SETTINGS PSTR("Ustawienia Open Weather Map")
#define INTL_OWM_API_KEY PSTR("Klucz Open Weather Map API")
#define INTL_OWM_LATITUDE PSTR("Szerokość geograficzna")
#define INTL_OWM_LONGITUDE PSTR("Długość geograficzna")
#define INTL_OWM_REFRESH_INTERVAL PSTR("Interwał odświeżania danych OWM")
#define INTL_LOCATAION_NAME PSTR("Nazwa lokalizacji")
#define INTL_AQI_SETTINGS PSTR("Ustawienia Air Quality Index")
#define INTL_AQI_URL PSTR("Adres URL AQI")
#define INTL_AQI_REFRESH_INTERVAL PSTR("Interwał odświeżania danych AQI")
#define INTL_LOCAL_TEMP_SETTINGS                                               \
  PSTR("Ustawienie lokalnego sensora temperatury/wilgotności")
#define INTL_LOCAL_TEMP_SENSOR_TYPE PSTR("Typ podłączonego sensora")
#define INTL_LOCAL_TEMP_REFRESH_INTERVAL                                       \
  PSTR("Interwał odświeżania lokalnego sensora")

#define INTL_OWM_API_KEY_ERROR PSTR("Wprowadź prawidłowy klucz OWM API")
#define INTL_LOCATION_ERROR PSTR("Wprowadź nazwę lokalizacji")
#define INTL_AQI_URL_ERROR                                                     \
  PSTR("Wprowadź URL w formacie http://.../data.json lub "                    \
       "https://.../data.json")

const char INTL_HTML_GREETINGS[] PROGMEM = "<p>Witaj na stronie {v}!</p>\n";
const char INTL_HTML_CONFIG_LINK[] PROGMEM =
    "<p>Kliknij <a href='config'>TUTAJ</a> aby przejść do zmiany "
    "ustawień.</p>\n";
const char INTL_HTML_FORM_END[] PROGMEM = "<button type='submit' "
                                          "style='margin-top: "
                                          "10px;'>Zastosuj</button></form>\n";
const char INTL_HTML_SAVED[] PROGMEM = "<div>Konfiguracja zapisana<br />Wróć "
                                       "do <a href='/'>strony "
                                       "głównej</a>.</div>\n";
const char INTL_HTML_UPDATE[] PROGMEM = "<div style='padding-top:25px;'><a "
                                        "href='{u}'>Aktualizacja "
                                        "oprogramowania</a></div>\n";
const char INTL_HTML_CONFIG_VER[] PROGMEM =
    "<div style='font-size: .6em;'>Wersja konfiguracji '{v}'</div>\n";

//////////////////////

const char INTL_IP_ADDRESS[] PROGMEM = "Adres IP:";
const char INTL_SUBNET_MASK[] PROGMEM = "Maska podsieci:";
const char INTL_GATEWAY[] PROGMEM = "Brama:";
const char INTL_MAC_ADDRESS[] PROGMEM = "Adres MAC:";
const char INTL_SSID[] PROGMEM = "SSID:";
const char INTL_RSSI[] PROGMEM = "Siła WiFi:";
const char INTL_WIFI_QALITY[] PROGMEM = "Jakość Wifi:";
const char INTL_HEAP_MEM[] PROGMEM = "Pamięć sterty:";
const char INTL_FLASH_MEM[] PROGMEM = "Pamięć flash:";
const char INTL_CHIP_ID[] PROGMEM = "ID układu:";
const char INTL_VCC[] PROGMEM = "Napięcie:";
const char INTL_CPU_FREQ[] PROGMEM = "Częstotliwość CPU:";
const char INTL_UPTIME[] PROGMEM = "Czas pracy:";
const char INTL_LAST_RESET[] PROGMEM = "Ostatni restart:";

const char INTL_NO_DATA[] PROGMEM = "Brak danych";
const char INTL_AQI_GOOD[] PROGMEM = "Dobra";
const char INTL_AQI_MODERATE[] PROGMEM = "Średnia";
const char INTL_AQI_UNHEALTHY_SENSITIVE[] PROGMEM = "Niezdrowa dla wrażliwych";
const char INTL_AQI_UNHEALTHY[] PROGMEM = "Niezdrowa";
const char INTL_AQI_VERY_UNHEALTHY[] PROGMEM = "Bardzo niezdrowa";
const char INTL_AQI_HAZARDOUS[] PROGMEM = "Zagrażająca życiu";

const char INTL_CURRENT_CONDITIONS[] PROGMEM = "Aktualne warunki";
const char INTL_TEMPERATURE_MINMAX[] PROGMEM = "Temp. min/max:";
const char INTL_TEMPERATURE_FEELSLIKE[] PROGMEM = "Temp. odczuwalna:";
const char INTL_TEMPERATURE_DEW_POINT[] PROGMEM = "Temp. punkt rosy:";
const char INTL_WIND_SPEED[] PROGMEM = "Prędkość wiatru:";
const char INTL_WIND_DEGREE[] PROGMEM = "Kierunek wiatru:";
const char INTL_RAIN_SNOW[] PROGMEM = "Opad deszcz/śnieg:";
const char INTL_CLOUDS[] PROGMEM = "Zachmurzenie:";
const char INTL_UVI[] PROGMEM = "Indeks UV:";
const char INTL_VISIBILITY[] PROGMEM = "Widoczność:";
const char INTL_AIR_QUALITY[] PROGMEM = "Jakość powietrza:";

const char INTL_ORGINAL[] PROGMEM = "Oryginał: blog.squix.org, trekawek";
const char INTL_ADAPTEDBY[] PROGMEM = "Zadoptował: Łukasz Baran";

const char INTL_MOUNTING_FILE_SYSTEM[] PROGMEM = "Montowanie systemu plików";
const char INTL_FORMATTING_FILE_SYSTEM[] PROGMEM =
    "Formatowanie systemu plików";
const char INTL_SETTING_TOUCH_CALIBRATION[] PROGMEM = "Kalibracja dotyku";
const char INTL_PREPARING_OBJECTS[] PROGMEM = "Przygotowanie obiektów";
const char INTL_CONNECTED[] PROGMEM = "Podłączony: ";
const char INTL_UPDATING_DATA[] PROGMEM = "Aktualizacja danych";
const char INTL_DONE[] PROGMEM = "Gotowe!";
const char INTL_CONNECTING_TO[] PROGMEM = "Łączenie z ";
const char INTL_NO_CONNECTION[] PROGMEM = "Brak WiFi - sprawdź konfigurację!";
const char INTL_TOUCH_CORNERS[] PROGMEM = "Dodknij wskazane narożniki";
const char INTL_CALIBRATION_COMPLETE[] PROGMEM = "Kalibracja zakończona!";

const char INTL_FORECASTS[] PROGMEM = "Prognoza pogody";
