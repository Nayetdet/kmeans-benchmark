#ifndef PGM_H
#define PGM_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char maxValue;
    unsigned char *data;
} PGM;

PGM *readPGM(const char *const path);
void freePGM(PGM *pgm);

#ifdef __cplusplus
}
#endif

#endif
