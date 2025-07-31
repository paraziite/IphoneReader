#ifndef IDEVICEPAIR_H
#define IDEVICEPAIR_H

#include <IO/libimobiledevice/libimobiledevice.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error codes returned by idevicepair functions.
 */
typedef enum {
    IDEVICEPAIR_E_SUCCESS = 0,
    IDEVICEPAIR_E_UNKNOWN_ERROR = -1,
    IDEVICEPAIR_E_NO_DEVICE = -2,
    IDEVICEPAIR_E_PAIRING_FAILED = -3,
    IDEVICEPAIR_E_INVALID_ARGUMENT = -4,
    IDEVICEPAIR_E_ALREADY_PAIRED = -5,
    IDEVICEPAIR_E_NOT_PAIRED = -6,
    IDEVICEPAIR_E_PASSWORD_PROTECTED = -7,
    IDEVICEPAIR_E_INVALID_PASSWORD = -8,
    IDEVICEPAIR_E_SSL_ERROR = -9,
    IDEVICEPAIR_E_MUX_ERROR = -10,
    IDEVICEPAIR_E_COMMAND_TIMEOUT = -11,
    IDEVICEPAIR_E_SSL_NOT_VERIFIED = -12,
    IDEVICEPAIR_E_SERVICE_NOT_AVAILABLE = -13,
    IDEVICEPAIR_E_FILE_NOT_FOUND = -14
} idevicepair_error_t;

/**
 * Attempts to pair with the device.
 * Returns IDEVICEPAIR_E_SUCCESS on success.
 * Returns IDEVICEPAIR_E_ALREADY_PAIRED if device is already paired.
 * Returns other errors on failure.
 */
LIBIMOBILEDEVICE_API idevicepair_error_t idevicepair_pair(idevice_t device);

/**
 * Unpair the device.
 */
LIBIMOBILEDEVICE_API idevicepair_error_t idevicepair_unpair(idevice_t device);

/**
 * Validate the pairing.
 * Returns IDEVICEPAIR_E_SUCCESS if pairing is valid.
 */
LIBIMOBILEDEVICE_API idevicepair_error_t idevicepair_validate(idevice_t device);

#ifdef __cplusplus
}
#endif

#endif /* IDEVICEPAIR_H */
