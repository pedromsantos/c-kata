#include "receipt_sender.h"

#include <stdio.h>

ReceiptSender receipt_sender_create(void) {
    ReceiptSender sender;
    sender.sent = 0;
    return sender;
}

void receipt_sender_send_to_customer(ReceiptSender* self, const char* receipt_id) {
    self->sent = 1;
    printf("Sent receipt %s to customer\n", receipt_id);
}

/* Connascence of Execution Order: correct only when called after
 * receipt_sender_send_to_customer() -- nothing stops archive() running
 * first, it just warns and proceeds anyway. */
void receipt_sender_archive(const ReceiptSender* self, const char* receipt_id) {
    if (!self->sent) {
        printf("Warning: archiving receipt %s before it was sent\n", receipt_id);
    }
    printf("Archived receipt %s\n", receipt_id);
}
