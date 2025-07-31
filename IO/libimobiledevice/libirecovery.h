/**
 * libirecovery.h
 * Public API for libirecovery - communicates with iOS devices in DFU or recovery mode
 */

#ifndef LIBIRECOVERY_H
#define LIBIRECOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Version number */
#define LIBIRECOVERY_VERSION_MAJOR 1
#define LIBIRECOVERY_VERSION_MINOR 0
#define LIBIRECOVERY_VERSION_PATCH 0

/* Error codes */
typedef enum {
    IRECV_E_SUCCESS = 0,
    IRECV_E_NO_DEVICE,
    IRECV_E_OUT_OF_MEMORY,
    IRECV_E_UNSUPPORTED,
    IRECV_E_INVALID_INPUT,
    IRECV_E_PIPE,
    IRECV_E_TIMEOUT,
    IRECV_E_UNKNOWN_ERROR
} irecovery_error_t;

/* Recovery mode constants */
typedef enum {
    IRECV_K_RECOVERY_MODE_1 = 0x1280,
    IRECV_K_RECOVERY_MODE_2 = 0x1281,
    IRECV_K_RECOVERY_MODE_3 = 0x1282,
    IRECV_K_RECOVERY_MODE_4 = 0x1283,
    IRECV_K_DFU_MODE        = 0x1227
} irecovery_mode_t;

/* Client context for an open device */
typedef struct irecovery_client {
    int usb_handle;
    int mode;
    char* serial_number;
    uint32_t cpid;
    uint32_t bdid;
    uint64_t ecid;
    uint8_t* ibss;
    int ibss_length;
    void* internal;
} *irecovery_client_t;

/* Open recovery/DFU device */
irecovery_error_t irecovery_open(irecovery_client_t* client);

/* Open by specific device ID */
irecovery_error_t irecovery_open_device(irecovery_client_t* client, int device_id);

/* Close device connection */
irecovery_error_t irecovery_close(irecovery_client_t client);

/* Get device mode (DFU, Recovery, etc.) */
irecovery_error_t irecovery_get_mode(irecovery_client_t client, int* mode);

/* Send recovery command (e.g., "reboot", "setenv", "saveenv") */
irecovery_error_t irecovery_send_command(irecovery_client_t client, const char* command);

/* Send file to device (e.g., iBSS) */
irecovery_error_t irecovery_send_file(irecovery_client_t client, const char* filename);

/* Receive data from device */
irecovery_error_t irecovery_receive(irecovery_client_t client, char* data, uint32_t size);

/* Set debug output level (0 = none, 1 = verbose) */
void irecovery_set_debug_level(int level);

/* Get string value from device environment (recovery mode only) */
irecovery_error_t irecovery_getenv(irecovery_client_t client, const char* var, char* value, int value_size);

/* Set string value in device environment */
irecovery_error_t irecovery_setenv(irecovery_client_t client, const char* var, const char* value);

/* Save environment variables */
irecovery_error_t irecovery_saveenv(irecovery_client_t client);

/* Reboot device from recovery or DFU */
irecovery_error_t irecovery_reboot(irecovery_client_t client);

#ifdef __cplusplus
}
#endif

#endif /* LIBIRECOVERY_H */
