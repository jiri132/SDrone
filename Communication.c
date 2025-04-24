#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Communication.h"

static void write_uint16(uint8_t *buf, uint16_t value)
{
    buf[0] = (uint8_t)(value >> 8);
    buf[1] = (uint8_t)(value & 0xff);
}

static void write_float(uint8_t *buf, float value)
{
    memcpy(buf, &value, sizeof(float));
}

static size_t toMessage(uint8_t *buf, size_t cap,
                        uint16_t addr, uint16_t cmd,
                        const char *arg1, const char *arg2,
                        const char *arg3, const char *arg4)
{
    uint8_t cmdId = (uint8_t)(cmd >> 8);
    uint8_t len = (cmd & 0xFFu) / 2;

    if (cap < 4u + len) return 0u;

    write_uint16(buf, addr);
    buf[2] = cmdId;
    buf[3] = len;

    if (len == 0u) return 4u;


    memset(buf + 4, 0, len);

    switch (cmd)
    {
    case CMD_STATUS:
        buf[4] = (uint8_t)strtoul(arg1, NULL, 0);
        break;

    case CMD_APITCH:
    case CMD_AYAW:
    case CMD_AROLL:
    case CMD_ATHROTTLE:
        buf[4] = (uint8_t)(int8_t)strtol(arg1, NULL, 0);
        break;

    case CMD_TSPEED:
    {
        float r = strtof(arg1, NULL), v = strtof(arg2, NULL);
        write_float(buf + 4, r);
        write_float(buf + 8, v);
        break;
    }

    case CMD_TAP:
    {
        uint16_t acc = (uint16_t)strtoul(arg1, NULL, 0);
        float p = strtof(arg2, NULL);
        write_uint16(buf + 4, acc);
        write_float(buf + 6, p);
        break;
    }

    case CMD_TRPM:
    case CMD_TCOM:
    {
        uint16_t f1 = (uint16_t)strtoul(arg1, NULL, 0);
        uint16_t f2 = (uint16_t)strtoul(arg2, NULL, 0);
        uint16_t f3 = (uint16_t)strtoul(arg3, NULL, 0);
        uint16_t f4 = (uint16_t)strtoul(arg4, NULL, 0);
        write_uint16(buf + 4, f1);
        write_uint16(buf + 6, f2);
        write_uint16(buf + 8, f3);
        write_uint16(buf + 10, f4);
        break;
    }

    case CMD_TERROR:
    {
        uint16_t err = (uint16_t)strtoul(arg1, NULL, 0);
        write_uint16(buf + 4, err);
        break;
    }

    case CMD_TPOWER:
    {
        uint16_t batt = (uint16_t)strtoul(arg1, NULL, 0);
        uint16_t curr = (uint16_t)strtoul(arg2, NULL, 0);
        write_uint16(buf + 4, batt);
        write_uint16(buf + 6, curr);
        break;
    }

    case CMD_TPITCH:
    case CMD_TROLL:
    case CMD_TYAW:
    case CMD_TSOC:
    {
        float val = strtof(arg1, NULL);
        write_float(buf + 4, val);
        break;
    }

    case CMD_TEMF12:
    case CMD_TEMF34:
    case CMD_TIMOTOR12:
    case CMD_TIMOTOR34:
    {
        float f1 = strtof(arg1, NULL);
        float f2 = strtof(arg2, NULL);
        write_float(buf + 4, f1);
        write_float(buf + 8, f2);
        break;
    }

    default:
        return 0u;
    }
    return 4u + len;
}

static struct { const char *name; uint16_t cmd; } map[] =
{
    { "ACK",       CMD_ACK },
    { "NACK",      CMD_NACK },
    { "STATUS",    CMD_STATUS },
    { "START",     CMD_START },
    { "STOP",      CMD_STOP },
    { "APitch",    CMD_APITCH },
    { "AYaw",      CMD_AYAW },
    { "ARoll",     CMD_AROLL },
    { "AThrottle", CMD_ATHROTTLE },
    { "TSpeed",    CMD_TSPEED },
    { "TAP",       CMD_TAP },
    { "TRPM",      CMD_TRPM },
    { "TCOM",      CMD_TCOM },
    { "TError",    CMD_TERROR },
    { "TPower",    CMD_TPOWER },
    { "TPitch",    CMD_TPITCH },
    { "TRoll",     CMD_TROLL },
    { "TYaw",      CMD_TYAW },
    { "TSOC",      CMD_TSOC },
    { "TEMF12",    CMD_TEMF12 },
    { "TEMF34",    CMD_TEMF34 },
    { "TIMotor12", CMD_TIMOTOR12 },
    { "TIMotor34", CMD_TIMOTOR34 },
    { NULL, 0 }
};

static uint16_t findCmd(const char *s)
{
    for (int i = 0; map[i].name; ++i)
        if (!strcasecmp(s, map[i].name))
            return map[i].cmd;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        puts("usage: msggen <COMMAND> [args...]");
        return 1;
    }

    uint16_t cmd = findCmd(argv[1]);
    if (!cmd) { puts("unknown command"); return 1; }

    uint8_t frame[32];
    size_t n = toMessage(frame, sizeof frame,
                         ADR_SELF, cmd,
                         argc > 2 ? argv[2] : NULL,
                         argc > 3 ? argv[3] : NULL,
                         argc > 4 ? argv[4] : NULL,
                         argc > 5 ? argv[5] : NULL);

    if (!n) { puts("build error"); return 1; }

    for (size_t i = 0; i < n; ++i) printf("%02X", frame[i]);
    putchar('\n');
    return 0;
}