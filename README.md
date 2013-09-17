ArduBreathalyzer
================

Arduino based wireless breathalyzer which posts data to your social media accounts.

Mikä tämä on?
-------------
ArduBreathalyzer on Arduinoon pohjautuva alkometri, joka toimii langattomasti sekä nettiä hyödyntäen. 
Veren alkoholipitoisuuden ohella alkometri välittää GPRS:n avulla erilliselle rajapinnalle myös käyttäjän sijainnin. 
Rajapinta lähettää tiedot edelleen eteenpäin sosiaalisen median palveluihin, mikäli käyttäjä niin haluaa.

Projektin taustasta sekä koodista on juttu __MikroPC 9/2013__:ssa. Koodin toinen puoli eli rajapinnan osuus löytyy reposta [ArduBreathalyzerAPI](https://github.com/skvark/ArduBreathalyzerAPI).

Sensorin testaus
----------------
Sensorin voi testata oheiselle sensortest.ino-tiedostolla. Serial monitorista näkee sensorin antamat arvot, kunhan
muokkaa tiedostoon sen analogipinnin, johon sensori on kytketty.


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
