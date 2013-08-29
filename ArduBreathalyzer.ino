#include <LiquidCrystal.h>

// pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// set these according to the ArduBreathalyzerAPI
String user = String("");
String token = String("");

int gps = 0;
int post_mode = 0;
int8_t answer;
int x = 0;
bool first_run = true;
bool fix = false;

char* GPS_modes[] = {"OFF", "ON"};
char* post_modes[] = {"All", "Facebook", "Twitter", "None"};

String lat = String("");
String lon = String("");

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons() {

    adc_key_in = analogRead(0);

    if (adc_key_in < 100)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 900)  return btnSELECT;

    return btnNONE;
}

// see http://www.dfrobot.com/wiki/index.php/GPS/GPRS/GSM_Module_V3.0_%28SKU:TEL0051%29#How_to_drive_the_GPS_Mode_via_Arduino_board
// for info about driving the shield

char ID() {

    char i = 0;
    char value[6] = {'$','G','P','G','G','A'};
    char val[6] = {'0','0','0','0','0','0'};

    while(1) {

        if (Serial.available()) {

            val[i] = Serial.read();

            if (val[i]==value[i]) {
                i++;

                if(i == 6) {
                    i = 0;
                    return 1;
                }
            } else
                i = 0;
        }
    }
}

void get_comma(char num) {

    char val;
    char count=0;

    while(1) {

        if (Serial.available()) {

            val = Serial.read();
            if(val==',')
            count++;
            }

        if (count==num)
        return;
    }
}

void latitude(String &lat) {

    char i;
    char lati[10] = {'0','0','0','0','0','0','0','0','0','0'};

    if( ID())
    {
        get_comma(2);

        while(1) {

            if (Serial.available()) {
                lati[i] = Serial.read();
                i++;
            }
            if (i == 10) {
                i = 0;
                lat = lati;
                return;
            }
        }
    }
}

void longitude(String &lon) {

    char i;
    char loni[11] = {'0','0','0','0','0','0','0','0','0','0','0'};

    if(ID()) {

        get_comma(4);

        while(1) {

            if (Serial.available()) {
                loni[i] = Serial.read();
                i++;
            }
            if (i == 11) {
                i = 0;
                lon = loni;
                return;
            }
        }
    }
}

int8_t ATcommand(char* command, unsigned int timeout,
                      char* expected_answer1, char* expected_answer2 = NULL ) {

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);
    delay(100);

    while( Serial.available() > 0) Serial.read();

    Serial.println(command);

    x = 0;
    previous = millis();

    while ((answer == 0) && ((millis() - previous) < timeout) ) {

        if (Serial.available() != 0) {

            response[x] = Serial.read();
            x++;

            if (strstr(response, expected_answer1) != NULL) {
                answer = 1;
            }

            if (expected_answer2 != NULL) {
                if (strstr(response, expected_answer2) != NULL) {
                    answer = 2;
                }
            }
        }
    }
    return answer;
}

bool initGPRS() {

    while (ATcommand("AT+CREG?", 2000, "+CREG: 0,1", "+CREG: 0,5") == 0);

    ATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", 2000, "OK");
    ATcommand("AT+SAPBR=3,1,\"APN\",\"internet\"", 2000, "OK");

    while (ATcommand("AT+SAPBR=1,1",  20000, "OK") == 0)
    {
        delay(5000);
    }
    return true;

}

void sendBACData(String service, String bac) {

    // create string
    String uri = String("http://relativity.fi:5050/api/"+user+"/"+token+"/"+bac+"/"+service+"/"+lat+"/"+lon);

    answer = ATcommand("AT+HTTPINIT", 10000, "OK");

    if (answer == 1)
    {
        answer = ATcommand("AT+HTTPPARA=\"CID\",1", 5000, "OK");

        if (answer == 1)
        {

            String command = String("\"AT+HTTPPARA=\"URL\",\""+uri+"\"");
            char charBuf[command.length()+1];
            command.toCharArray(charBuf, command.length()+1);

            answer = ATcommand(charBuf, 5000, "OK");

            if (answer == 1)
            {
                // Starts POST action
                answer = ATcommand("AT+HTTPACTION=1", 10000, "+HTTPACTION:0,200");

                if (answer == 1)
                {
                   // to be done
                }
                else
                {
                   // to be done
                }
            }
            else
            {
                // to be done
            }
        }
        else
        {
            // to be done
        }
    }
    else
    {
        // to be done
    }

    ATcommand("AT+HTTPTERM", 5000, "OK");
}


void startGPS()
{

    ATcommand("AT", 2000, "OK");
    // turn on GPS power supply
    ATcommand("AT+CGPSPWR=1", 2000, "OK");

    // shut the NMEA meassages or Arduino will shut down because of flood
    ATcommand("AT+CGPSOUT=0", 2000, "OK");
    // reset GPS to cold start mode
    ATcommand("AT+CGPSRST=0", 5000, "OK");

    answer = ATcommand("AT", 2000, "OK");
    if (answer == 0)
    {
        // waits for an answer from the module
        while(answer == 0){
            // Send AT every two seconds and wait for the answer
            answer = ATcommand("AT", 2000, "OK");
        }
    }
    // select GPS
    digitalWrite(11, LOW);
    digitalWrite(2, HIGH);
    delay(3000);
}

void stopGPS()
{
    // reset to cold start state
    ATcommand("AT+CGPSRST=0", 2000, "OK");
    // turn off GPS power supply
    ATcommand("AT+CGPSPWR=0", 2000, "OK");
    digitalWrite(2, LOW); // enable GSM
    digitalWrite(11, HIGH);
    delay(2000);

    // check the status of the module
    answer = ATcommand("AT", 2000, "OK");
    if (answer == 0)
    {
        // waits for an answer from the module
        while(answer == 0){
            // Send AT every two seconds and wait for the answer
            answer = ATcommand("AT", 2000, "OK");
        }
    }
}


void setup()
{

    lcd.begin(16, 2);
    lcd.print("Initializing...");

    //Init the driver pins for the shield
    pinMode(2,OUTPUT);
    pinMode(11,OUTPUT);
    pinMode(12,OUTPUT);

    digitalWrite(12,HIGH);  // Output GSM Timing
    delay(1500);
    digitalWrite(12, LOW);
    delay(1000);
    digitalWrite(2, LOW);   // Enable the GSM mode
    digitalWrite(11, HIGH); // Disable the GPS mode

    delay(4000);
    Serial.begin(9600); // set the baud rate
    delay(5000);
    delay(5000);
    delay(5000);

    // check the status of the module
    answer = ATcommand("AT", 2000, "OK");
    if (answer == 0)
    {
        // waits for an answer from the module
        while(answer == 0){
            // Send AT every two seconds and wait for the answer
            answer = ATcommand("AT", 2000, "OK");
        }
    }

}

void loop()
{

    if (first_run == true) {

        lcd.clear();
        lcd.print("Use GPS: ");
        lcd.setCursor(9,0);
        lcd.print(GPS_modes[0]);
        lcd.setCursor(0,1);
        lcd.print("Mode: ");
        lcd.setCursor(6,1);
        lcd.print(post_modes[0]);

        while (read_LCD_buttons() != 4) {

            int press = read_LCD_buttons();

            if (press == 0) {
                if (gps == 0) {
                    gps = 1;
                } else if (gps == 1) {
                    gps = 0;
                }
            }
            else if (press == 3) {
                if (post_mode == 3) {
                    post_mode = 0;
                }
                else {
                    post_mode += 1;
                }
            }

            lcd.clear();
            lcd.print("Use GPS: ");
            lcd.setCursor(9,0);
            lcd.print(GPS_modes[gps]);
            lcd.setCursor(0,1);
            lcd.print("Mode: ");
            lcd.setCursor(6,1);
            lcd.print(post_modes[post_mode]);
            delay(500);
        }
        first_run = false;
    }

    delay(1500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ready to start!");
    lcd.setCursor(0,1);
    lcd.print("Press select.");

    bool locate = true;
    bool measuring = false;

    delay(500);

    int button = read_LCD_buttons();

    if (button == 4) {

        fix = false;
        measuring = true;
        while (measuring == true) {

            if (gps == 0 && locate == true) {

                lcd.clear();
                lcd.print("Starting GPS...");
                startGPS();
                lcd.clear();
                lcd.print("Locating...");

                unsigned long previous;
                previous = millis();

                // waits for fix GPS
                while((ATcommand("AT+CGPSSTATUS?", 5000, "Location 2D Fix", "Location 3D Fix") == 0 ) &&
                      ((millis() - previous) < 300000));

                if ((millis() - previous) < 300000)
                {
                    fix = true;
                }

                if (fix) {

                  ATcommand("AT+CGPSOUT=2", 2000, "OK");
                  delay(500);

                  latitude(lat);
                  longitude(lon);

                  delay(500);

                  ATcommand("AT+CGPSOUT=0", 2000, "OK");
                  lcd.clear();
                  lcd.print("GPS fix.");

                } else {

                  lcd.clear();
                  lcd.print("No GPS fix.");

                }

                locate = false;
                delay(1000);
                stopGPS();
            }

            lcd.clear();
            lcd.home();
            lcd.print("Press select to");
            lcd.setCursor(0,1);
            lcd.print("start measuring.");

            delay(1000);

            unsigned long meas = 0;
            int button2 = read_LCD_buttons();

            if (button2 == 4) {

                delay(1000);
                lcd.clear();
                lcd.home();
                lcd.print("Measuring in 1s.");

                delay(1000);
                lcd.clear();
                lcd.print("Measuring...");

                for (int i = 0; i < 100; i++) {
                    meas += analogRead(1);
                }

                meas = meas / 100;

            lcd.clear();
            lcd.home();
            lcd.print("Value: ");
            lcd.setCursor(8,0);
            lcd.print(meas);

            delay(2000);

            initGPRS();
            String bac = String(meas);

            sendBACData(post_modes[post_mode], bac);

            measuring = false;
            }
        }
        button = 5;
        first_run = false;
    }
}
