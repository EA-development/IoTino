#include "Mail.h"
#include <string>


SMTPSession smtp;
ESP_Mail_Session Mail::session;
SMTP_Message Mail::message;





 //https://myaccount.google.com/lesssecureapps?pli=1
/* The SMTP Session object used for Email sending */


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void Mail::setup() {
 //Serial.begin(115200);
  //Serial.println();
  //Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //probably better in a setup function
  while (WiFi.status() != WL_CONNECTED){
    //Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

   /* Declare the session config data */

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */

  /* Set the message headers */
  message.sender.name = "IoTino";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Papierstatus!";
  message.addRecipient("Hausmeister", RECIPIENT_EMAIL);

  
/*
  //Send HTML message
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  */
  //Send raw text message
  
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");
  if (!smtp.connect(&session)) {
    Serial.println("NO session!!!!");
    // return false;
  }

}

bool Mail::sendMail(double lidar,double ir){
   
String textMsg = "FAKE Papierspender hat einen Stand von:  " + String(lidar) + "cm (Lidar) " + (String) ir + "cm (IR)";
  message.text.content = textMsg.c_str();
  /* Connect to server with the session config */
  Serial.println("start sending");
  unsigned long benchmark = millis();
  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email, " + smtp.errorReason());
    return false;
  }
  benchmark = millis()-benchmark;
  Serial.println(benchmark);
  Serial.println("end sending");

  int value = session.time.ntp_server;

}


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  //Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    //Serial.println("----------------");
    //ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    //ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    //Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      /*ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
      */
    }
    //Serial.println("----------------\n");
  }
  
}
int getTime(){
    int teset = ESPTimeHelper
  }
