#include "emailService.h"
const uint64_t ms_minimum_alert_time = 0;
EMailSender emailSender;
JsonArray recipients;


namespace EmailService{
    bool init() {
        LOG_TRACE("EmailService::init");
        JsonDocument config;
        if (!WebFileServer::readConfig(config, "/configs/wifi.json")) {
            LOG_ERROR("Unable to read email config files.");
            return false;
        }
        
        String smtpServer = config["sender-details"]["smtp-server"].as<String>();
        LOG_DEBUG("smtpServer = " + smtpServer);
        uint16_t smtpPort = config["sender-details"]["smtp-port"].as<uint16_t>();
        LOG_DEBUG("smtpPort = " + smtpPort);
        String senderLogin = config["sender-details"]["email-login"].as<String>();
        LOG_DEBUG("senderLogin = " + senderLogin);
        String senderPassword = config["sender-details"]["email-password"].as<String>();
        LOG_DEBUG("senderPassword = " + senderPassword);
        String senderEmailAddress = config["sender-details"]["email-address"].as<String>();
        LOG_DEBUG("senderEmailAddress = " + senderEmailAddress);
        String senderName = config["sender-details"]["sender-name"].as<String>();
        LOG_DEBUG("senderName = " + senderName);

        emailSender = EMailSender(
            senderLogin.c_str(), 
            senderPassword.c_str(), 
            senderEmailAddress.c_str(), 
            senderName.c_str(), 
            smtpServer.c_str(), 
            smtpPort
        );

        recipients = config["recipient-address"];
    }

    EMailSender::EMailMessage generateStartupMessage() {
        LOG_TRACE("EmailService::generateStartupMessage");
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
        LOG_TRACE("EmailService::sendMessage");

        for (uint8_t i = 0; i < recipients.size(); i++) {
            String recipient = recipients[0].as<String>();
            emailSender.send(recipient, message);
        }
    }

    void testEmailService1() {
        EmailService::init();
        EMailSender::EMailMessage m = EmailService::generateStartupMessage();
        EmailService::sendMessage(m);
    }
};