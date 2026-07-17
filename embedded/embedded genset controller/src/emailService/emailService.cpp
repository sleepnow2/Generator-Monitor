#line 1 "emailService.cpp"
#include "emailService.h"
const uint64_t ms_minimum_alert_time = 0;

namespace EmailService{
    static JsonArray recipients;
    static EMailSender emailSender("","");

    bool init() {
        LOG_TRACE();
        JsonDocument config;
        if (!WebFileServer::readConfig(config, "/configs/email.json")) {
            LOG_ERROR("Unable to read email config files.");
            return false;
        }

        String tmp;
        
        tmp = config["sender-details"]["smtp-server"].as<String>();
        LOG_DEBUG("smtp-server = " + tmp);
        emailSender.setSMTPServer(tmp.c_str()); 

        uint16_t smtpPort = config["sender-details"]["smtp-port"].as<uint16_t>();
        LOG_DEBUG("smtp-port = " + (String)smtpPort);
        emailSender.setSMTPPort(smtpPort); 

        tmp = config["sender-details"]["email-login"].as<String>();
        LOG_DEBUG("senderLogin = " + tmp);
        emailSender.setEMailLogin(tmp.c_str()); 

        tmp = config["sender-details"]["email-password"].as<String>();
        LOG_DEBUG("senderPassword = " + tmp);
        emailSender.setEMailPassword(tmp.c_str()); 

        tmp = config["sender-details"]["email-address"].as<String>();
        LOG_DEBUG("senderEmailAddress = " + tmp);
        emailSender.setEMailFrom(tmp.c_str()); 

        tmp = config["sender-details"]["sender-name"].as<String>();
        LOG_DEBUG("senderName = " + tmp);
        emailSender.setNameFrom(tmp.c_str()); 
        

        recipients = config["recipient-address"];
        LOG_DEBUG(recipients.size()); 
        return 1;
    }

    EMailSender::EMailMessage generateStartupMessage() {
        LOG_TRACE();
        static EMailSender::EMailMessage message;

        // so we only load this item into memory once. 
        static bool initialized = false;
        if (!initialized) {
            message.subject = "Startup message";
            message.mime = MIME_TEXT_PLAIN;
            message.message = "this is an example startup message";
            initialized = true;
        }

        return message;
    }

    void sendMessage(EMailSender::EMailMessage message) {
        LOG_TRACE();
        EMailSender::Response resp = emailSender.send("sleepnow2@gmail.com", message);
        LOG_DEBUG(resp.code);
        LOG_DEBUG(resp.status);
        LOG_DEBUG(resp.desc);

        //for (uint8_t i = 0; i < recipients.size(); i++) {
        //    String recipient = recipients[0].as<String>();
        //    emailSender.send("sleepnow2@gmail.com", message);
        //}
    }

    void testEmailService1() {
        LOG_TRACE();
        EmailService::init();
        EMailSender::EMailMessage m = EmailService::generateStartupMessage();
        EmailService::sendMessage(m);
    }
};