#pragma once

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <vector>

// Maximum payload size
#define MAX_PAYLOAD 1012

// Retransmission time (seconds, is this useful?)
#define RETRANSMISSION_TIME 1

// Window size
#define MAX_WINDOW 20 * MAX_PAYLOAD
#define DUP_ACKS 3

// States: feel free to make your own, or even use an enum
// This will hopefully be helpful to understand we are making a state machine
#define SERVER_AWAIT 0
#define CLIENT_START 1
#define SERVER_SYN 2
#define ClIENT_AWAIT 3
#define SERVER_AWAIT_2 4
#define CLIENT_AWAIT_2 5
#define NORMAL 6

// Diagnostic messages, this is helpful for debugging, but you can remove it if you want
#define RECV 0
#define SEND 1
#define RTOD 2
#define DUPA 3

// Structs
typedef struct {
    uint32_t ack;
    uint32_t seq;
    uint16_t length;
    uint8_t flags; // LSb 0 SYN, LSb 1 ACK
    uint8_t unused;
    uint8_t payload[MAX_PAYLOAD];
} packet;

#define PACKET_HEADER_SIZE offsetof(packet, payload)

typedef struct {
    std::vector<packet> packets;
    unsigned int bytes;
} buffer;

// Helpers
static inline void print(char* txt) {
    fprintf(stderr, "%s\n", txt);
}

static inline void print_diag(packet* pkt, int diag) {
    switch (diag) {
    case RECV:
        fprintf(stderr, "RECV");
        break;
    case SEND:
        fprintf(stderr, "SEND");
        break;
    case RTOD:
        fprintf(stderr, "RTOS");
        break;
    case DUPA:
        fprintf(stderr, "DUPS");
        break;
    }

    bool syn = pkt->flags & 0b01;
    bool ack = pkt->flags & 0b10;
    fprintf(stderr, " %u ACK %u SIZE %hu FLAGS ", ntohl(pkt->seq),
            ntohl(pkt->ack), ntohs(pkt->length));
    if (!syn && !ack) {
        fprintf(stderr, "NONE");
    } else {
        if (syn) {
            fprintf(stderr, "SYN ");
        }
        if (ack) {
            fprintf(stderr, "ACK ");
        }
    }
    fprintf(stderr, "\n");
}

static inline void print_buf(buffer* buf) {
    fprintf(stderr, "BUF ");

    if (buf != NULL) {
        for (const packet& pkt : buf->packets) {
            fprintf(stderr, "%u ", ntohl(pkt.seq));
        }
    }
    fprintf(stderr, "\n");
}
