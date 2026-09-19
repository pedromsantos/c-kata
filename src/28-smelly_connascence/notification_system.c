#include "notification_system.h"

#include <stdio.h>

/* Connascence of Position: recipient, sender and message are all `const
 * char*`, distinguished only by their position in the parameter list. */
void notification_system_send_email(const char* recipient, const char* sender, const char* message) {
    printf("To: %s\n", recipient);
    printf("From: %s\n", sender);
    printf("Message: %s\n", message);
}
