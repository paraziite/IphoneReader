#ifndef RECOVERY_H
#define RECOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libirecovery.h>

/**
 * Reboot an iOS device from Recovery or DFU mode.
 * This sends necessary commands and reboots.
 *
 * @return 0 on success, non-zero on error
 */
static inline int recovery_reboot_device() {
    irecovery_client_t client = NULL;

    // Open device
    if (irecovery_open(&client) != IRECV_E_SUCCESS) {
        return -1; // No device
    }

    // Set auto-boot true (some devices require this)
    irecovery_send_command(client, "setenv auto-boot true");
    irecovery_send_command(client, "saveenv");

    // Reboot the device
    int result = irecovery_reboot(client);

    // Close the connection
    irecovery_close(client);

    return (result == IRECV_E_SUCCESS) ? 0 : -2;
}

#ifdef __cplusplus
}
#endif

#endif /* RECOVERY_H */
