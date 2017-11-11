#ifndef _LOGO_H_
#define _LOGO_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint8_t XbmWidth;    /*!< Xbm width in pixels */
    uint8_t XbmHeight;   /*!< Xbm height in pixels */
    const uint8_t *data; /*!< Pointer to data font data array */
} XbmDef_t;


extern XbmDef_t Wifi_Logo;

#ifdef __cplusplus
}
#endif

#endif
