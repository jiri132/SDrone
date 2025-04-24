#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

/* ── node addresses ───────────────────────────── */
#define ADR_SELF      0x1234u

/* ── commands (lo‑byte =id, hi‑byte = length) ─── */
#define CMD_ACK        0x0100u
#define CMD_NACK       0x0200u
#define CMD_STATUS     0x0302u
#define CMD_START      0x0400u
#define CMD_STOP       0x0500u
#define CMD_APITCH     0x0602u
#define CMD_AYAW       0x0702u
#define CMD_AROLL      0x0802u
#define CMD_ATHROTTLE  0x0902u
#define CMD_TSPEED     0x0a10u
#define CMD_TAP        0x0b0cu
#define CMD_TRPM       0x0c10u
#define CMD_TCOM       0x0d10u
#define CMD_TERROR     0x0e04u
#define CMD_TPOWER     0x0f08u
#define CMD_TPITCH     0x1008u
#define CMD_TROLL      0x1108u
#define CMD_TYAW       0x1208u
#define CMD_TSOC       0x1304u
#define CMD_TEMF12     0x1410u
#define CMD_TEMF34     0x1510u
#define CMD_TIMOTOR12  0x1610u
#define CMD_TIMOTOR34  0x1710u

/* ── common header (addr | cmd | len) ─────────── */
typedef struct __attribute__((packed))
{
    uint16_t addr;
    uint8_t  cmd;
    uint8_t  len;
} Header;

/* ── payload structures ───────────────────────── */
typedef struct __attribute__((packed)){ uint8_t state;          } PL_STATUS;
typedef struct __attribute__((packed)){ int8_t  speed;          } PL_MOV_I8;   /* Pitch/Yaw/Roll/Throttle */
typedef struct __attribute__((packed)){ float   rel; float vel; } PL_TSPEED;
typedef struct __attribute__((packed)){ uint16_t acc; float p;  } PL_TAP;
typedef struct __attribute__((packed)){ uint16_t f1,f2,f3,f4;   } PL_TRPM,
                                       PL_TCOM;
typedef struct __attribute__((packed)){ uint16_t err;           } PL_TERROR;
typedef struct __attribute__((packed)){ uint16_t batt; uint16_t cur; } PL_TPOWER;
typedef struct __attribute__((packed)){ float val;              } PL_F32;
typedef struct __attribute__((packed)){ float f1,f2;            } PL_F32x2;

/* ── helper to pack a 32‑bit header quickly ───── */
static inline uint32_t packHdr(uint16_t a, uint8_t c, uint8_t l)
{
    return ((uint32_t)a << 16) | ((uint32_t)c) | ((uint32_t)l << 8);
}

#endif