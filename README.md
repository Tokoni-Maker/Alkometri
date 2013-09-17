ArduBreathalyzer
================

Arduino based wireless breathalyzer which posts data to your social media accounts.

Mikä tämä on?
-------------
ArduBreathalyzer on Arduinoon pohjautuva alkometri, joka toimii langattomasti nettiä hyödyntäen. 
Veren alkoholipitoisuuden ohella alkometri välittää GPRS:n avulla erilliselle rajapinnalle myös käyttäjän sijainnin. 
Rajapinta lähettää tiedot edelleen eteenpäin sosiaalisen median palveluihin, mikäli käyttäjä niin haluaa.

Projektin taustasta sekä koodista on juttu __MikroPC 9/2013__:ssa. Koodin toinen puoli eli rajapinnan osuus löytyy reposta [ArduBreathalyzerAPI](https://github.com/skvark/ArduBreathalyzerAPI).

Sensorin testaus ja kalibrointi
-------------------------------
Sensorin voi testata oheisella sensortest.ino-tiedostolla. Serial monitorista näkee sensorin antamat arvot, kunhan
muokkaa tiedostoon sen analogipinnin, johon sensori on kytketty. Kalibroinnin tuloksia hyödyntämällä kannattaa tehdä muutoksia funktioon calculate_bac.


Käyttäminen
-----------
Ennen käyttämistä seuraavat arvot täytyy lisätä ArduBreathalyzer.ino-tiedoston alkuun. 
User, token ja url ovat rajapinnasta riippuvaisia. Apn riippuu liittymästä eli se on operaattorikohtainen.

    String user = String("");
    String token = String("");
    String url = String("");
    String apn = String("");
    
Tarkemmat ohjeet löytyvät MikroPC:stä. Toimiakseen täydellisesti vaatii koodi käynnissä olevan rajapinnan ja
siihen linkitetyt sosiaaliseen mediaan tehdyt sovellukset. Koodin voi tietysti aina muokata omiin tarpeisiin sopivaksi.

Osat
----

Käytin prototyypissä seuraavia osia. Eri osia käytettäessä koodia joutuu todennäköisesti muokkaamaan.

- DFRobot Arduino GSM / GPRS / GPS –shield
- DFRobot LCD Keypad Shield for Arduino
- MQ-3 –sensori
- MQ-3 breakout –levy
- 2.1mm male barrel jack kaapelilla
- Rocker Switch (virtakytkin)
- LiPo 7,4 V 1000 mAh (+ehkä akulle omat liittimet)
- Prepaid dataliittymä
- Johtoa

What is this?
-------------
ArduBreathalyzer is a social breathalyzer based on Arduino. It operates via GSM network and uses GPS for location.

The system has a separate REST API. The data is sent via the API to social media services like Facebook or Twitter. 
You can found the sources for it from [ArduBreathalyzerAPI](https://github.com/skvark/ArduBreathalyzerAPI).

The background story and more information about this project can be found from a Finnish IT magazine called __[MikroPC](http://www.mikropc.net/) (PC World Finland) 9/2013__.

Testing/calibrating the sensor
------------------------------

You can test the sensor and calibrate it with __sensortest.ino__. Change the analog pin according to your setup. 
Function calculate_bac needs probably some modifications.

Usage
-----------
Before using the this you have to set up the API and get user, token and url from it. 
After that add them to the code to the following lines. APN value depends on your operator.

    String user = String("");
    String token = String("");
    String url = String("");
    String apn = String("");
    
Parts
-----

Following parts were used to build the prototype. If you use different parts (shields), code may need changes.

- DFRobot Arduino GSM / GPRS / GPS –shield
- DFRobot LCD Keypad Shield for Arduino
- MQ-3 sensor
- MQ-3 breakout board 
- 2.1mm male barrel jack with a cable
- Rocker Switch 
- LiPo 7,4 V 1000 mAh
- Prepaid sim card 
- Wire
