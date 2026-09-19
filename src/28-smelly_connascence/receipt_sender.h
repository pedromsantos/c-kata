#pragma once

/*
 * Connascence Kata - Connascence of Execution Order, C port of
 * ReceiptSender.ts.
 *
 * Execution Order violation: archive() is only correct after
 * send_to_customer() has run, but nothing enforces that order -- calling
 * archive() first is accepted, just wrong.
 */

typedef struct {
    int sent;
} ReceiptSender;

ReceiptSender receipt_sender_create(void);
void receipt_sender_send_to_customer(ReceiptSender* self, const char* receipt_id);
void receipt_sender_archive(const ReceiptSender* self, const char* receipt_id);
