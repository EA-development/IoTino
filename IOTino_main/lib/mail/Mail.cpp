#include "Mail.h"


SMTPSession smtp;

 //https://myaccount.google.com/lesssecureapps?pli=1
/* The SMTP Session object used for Email sending */


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

bool Mail::sendMail(double lidar,double ir){

  //Serial.begin(115200);
  //Serial.println();
  //Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    //Serial.print(".");
    delay(200);
  }
  /*Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  */

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);
  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

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
  String textMsg = "Fake Papierspender hat einen Füllstand von:" + round(lidar) + "(Lidar)" + round(ir) + "(IR)";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return true;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    //Serial.println("Error sending Email, " + smtp.errorReason());
    return false;
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
