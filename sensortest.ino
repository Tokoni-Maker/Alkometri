// change the analog pin according to your circuit

unsigned int raw_value = 0;
unsigned int analog_pin = 0;

void setup() {
    Serial.begin(9600);
    pinMode(analog_pin, INPUT);
}

void loop() {
    raw_value = analogRead(analog_pin);
    Serial.println(raw_value);
    delay(500);
}