#ifndef Mail_h
#define Mail_h

#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <cmath>


#define WIFI_SSID "JCBS-Schüler"
#define WIFI_PASSWORD "S1,16DismdEn;deieKG!"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "isajah.tappe@gmail.com"
#define AUTHOR_PASSWORD "KoenigsKind777"
/* Recipient's email*/
#define RECIPIENT_EMAIL "jonas.na.braun@web.de"
#define RECIPIENT_EMAIL_TWO "hausmeister@jcbs.de"


class Mail {
    public:
    static void setup();
    static bool sendMail(double lidar, double ir);

    private:
    static ESP_Mail_Session session;
    static SMTP_Message message;

    
};

#endif