#ifndef EMBEDDED_GENSET_CONTROLLER_EMAIL_SERVER_H
#define EMBEDDED_GENSET_CONTROLLER_EMAIL_SERVER_H
#include <time.h>
#define EMAIL_ENABLE_INTERNAL_SSLCLIENT 1
#include "main.h"


namespace EmailService {
    bool init();
    EMailSender::EMailMessage generateStartupMessage();
    void sendMessage(EMailSender::EMailMessage message);
    void testEmailService1();
};

#endif // EMBEDDED_GENSET_CONTROLLER_EMAIL_SERVER_H