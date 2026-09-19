#pragma once

/*
 * Connascence Kata - Connascence of Position, C port of
 * NotificationSystem.ts.
 *
 * Position violation: sendEmail takes three same-typed strings whose
 * meaning is conveyed only by their argument position -- callers and the
 * function must silently agree on the order.
 */

void notification_system_send_email(const char* recipient, const char* sender, const char* message);
