// usart0_async.h
#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#ifndef UART0_TX_BUFSZ
#define UART0_TX_BUFSZ 256
#endif

static volatile uint8_t tx_buf[UART0_TX_BUFSZ];
static volatile uint16_t tx_head = 0, tx_tail = 0;

static inline void usart0_init_async(uint32_t baud)
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
    UCSR0A = 0;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);               // enable TX,RX
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);             // 8N1
}

static inline void usart0_tx_start(void)
{
    // Enable Data Register Empty interrupt to kick TX ISR
    UCSR0B |= _BV(UDRIE0);
}

ISR(USART0_UDRE_vect)
{
    if (tx_head == tx_tail) {
        // Buffer empty; stop interrupt until new data arrives
        UCSR0B &= ~_BV(UDRIE0);
        return;
    }
    UDR0 = tx_buf[tx_tail];
    tx_tail = (tx_tail + 1) % UART0_TX_BUFSZ;
}

static inline void usart0_write_async(const char *s)
{
    uint16_t next;
    while (*s) {
        char c = *s++;
        if (c == '\n') { usart0_write_async("\r"); } // CRLF
        next = (tx_head + 1) % UART0_TX_BUFSZ;
        // Busy-wait if buffer full (or you could drop/return)
        while (next == tx_tail) { /* buffer full */ }
        tx_buf[tx_head] = (uint8_t)c;
        tx_head = next;
    }
    usart0_tx_start();
}

