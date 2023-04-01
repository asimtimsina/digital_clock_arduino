#include "LIS3DHTR.h"
LIS3DHTR<TwoWire> lis; //SPI

#include "math.h"
#include"TFT_eSPI.h"
TFT_eSPI tft;

int count = 0;

void print_display();
//void reset_display();


int hours = 23;
int minutes = 55;
int seconds = 50;

bool last_pressed_A = false;
bool last_pressed_B = false;

bool newdata_A = false;
bool newdata_B = false;

unsigned long int nowtime = 0;
int time_print_interval = 1000;

unsigned long int nowtime_changetime = 0;
int time_change_interval = 1000;

unsigned long int nowtime_changeled = 0;
int time_led_interval = 200;

unsigned long int nowtime_buzzer = 0;
int time_buzzer_interval = 200;
bool buzzer_status = LOW;

unsigned long int acclerometer_time = 0;
int acclerometer_interval = 5000;

bool ledstatus = LOW;
bool changesecond = LOW;
bool ledcomplete = LOW;
bool int_change = LOW;
bool play_buzzer = LOW;
bool off_mode = LOW;


float x_values, y_values, z_values;
int xangle;


void setup() {

  tft.begin();
  tft.setRotation(3);
  tft.setCursor(0, 10);
  tft.fillScreen(TFT_GREEN);
  tft.setTextColor(TFT_BLACK);          //sets the text colour to black
  tft.setTextSize(6);

  lis.begin(Wire1);
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // Setting output data rage to 25Hz, can be set up tp 5kHz
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); // Setting scale range to 2g, select from 2,4,8,16g

  digitalWrite(LED_BUILTIN, ledstatus);



  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIO_KEY_A), change_minute, LOW);
  attachInterrupt(digitalPinToInterrupt(WIO_KEY_B), change_hour, LOW);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}


void loop() {

  last_pressed_A = false;
  last_pressed_B = false;

  if (!off_mode && newdata_A) {
    minutes++;
    newdata_A = false;
    int_change = HIGH;
    play_buzzer = true;
    //    acclerometer_time = millis();
    //    digitalWrite(LCD_BACKLIGHT, HIGH);

  } // if

  if (!off_mode && newdata_B) {
    hours++;
    newdata_B = false;
    int_change = HIGH;
    play_buzzer = true;
    //    acclerometer_time = millis();
    //    digitalWrite(LCD_BACKLIGHT, HIGH);

  } // if

  /////////////////////////////////
  //acclerometer

  x_values = lis.getAccelerationX();
  y_values = lis.getAccelerationY();
  z_values = lis.getAccelerationZ();

  xangle = abs(asin(x_values) * 180 / 3.1415);
  
  if (xangle > 43 && xangle < 48) {
    acclerometer_time = millis();

    digitalWrite(LCD_BACKLIGHT, HIGH);
 
    off_mode = LOW;
  }

  if (millis() - acclerometer_time >= acclerometer_interval) {
  
    digitalWrite(LCD_BACKLIGHT, LOW);
    off_mode = HIGH;
    newdata_A = false;
    newdata_B = false;

  }

  ////////////////////////////////////

  if (millis() - nowtime >= time_print_interval) {
    tft.fillScreen(TFT_GREEN);

    seconds  += (millis() - nowtime) / 1000;
    changesecond = HIGH;
    if (seconds / 60 == 1) {
      seconds = 0;
      minutes += 1;
    }

    if (minutes / 60 == 1) {
      minutes = 0;
      if (!int_change) {
        hours += 1;
      }
    }

    if (hours / 24 == 1) {
      hours = 0;
    }


    print_display();

    nowtime = millis();

    int_change = LOW;
  } // if

  if (changesecond && (millis() - nowtime_changeled >= time_led_interval)) {

    ledstatus = !ledstatus;
    digitalWrite(LED_BUILTIN, ledstatus);
    if (ledstatus == LOW) {
      changesecond = LOW;
    }
    nowtime_changeled = millis();
  }


  if (play_buzzer && (millis() - nowtime_buzzer >= time_buzzer_interval)) {

    buzzer_status = !buzzer_status;
    if (buzzer_status) {
      analogWrite(WIO_BUZZER, 128);
    }
    else {
      analogWrite(WIO_BUZZER, 0);
      play_buzzer = LOW;

    }
    nowtime_buzzer = millis();
  }


}


////////////////////////////////////

void print_display() {
  if (hours < 10) {
    Serial.print(0); Serial.print(hours); Serial.print(":");
    tft.drawString("0" + String(hours) + ":", 20, 90);
  }
  else {
    Serial.print(hours); Serial.print(":");
    tft.drawString(String(hours) + ":", 20, 90);

  }


  if (minutes < 10) {
    Serial.print(0); Serial.print(minutes); Serial.print(":");
    tft.drawString("0" + String(minutes) + ":", 120, 90);

  }
  else {
    Serial.print(minutes); Serial.print(":");
    tft.drawString(String(minutes) + ":", 120, 90);

  }

  if (seconds < 10) {
    Serial.print(0); Serial.print(seconds);
    tft.drawString("0" + String(seconds) , 220, 90);

  }
  else {
    Serial.print(seconds);
    tft.drawString(String(seconds) , 220, 90);

  }
  Serial.println();

}
///////////

void blink_led() {

  if (millis() - nowtime_changeled >= time_led_interval) {
    ledstatus = !ledstatus;
    digitalWrite(LED_BUILTIN, ledstatus);

  }
}

/////////////////////////////

void change_hour() {
  if (!last_pressed_B) {
    last_pressed_B = true;
    newdata_B = true;
    //    play_buzzer = true;
  } // if
}


void change_minute() {
  if (!last_pressed_A) {
    last_pressed_A = true;
    newdata_A = true;
    //    play_buzzer = true;
  } // if
}
