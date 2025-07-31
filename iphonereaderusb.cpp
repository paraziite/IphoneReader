#include "iphonereaderusb.h"
#include <QMap>
#include <QDebug>
#include <ui_mainwindow.h>


IphoneReaderUsb::IphoneReaderUsb(QObject *parent) : QObject(parent) {

}
IphoneReaderUsb::~IphoneReaderUsb()
{

    disconnect();
}
bool IphoneReaderUsb::connectToDevice(QString &error)
{
    if (idevice_new(&device, nullptr) != IDEVICE_E_SUCCESS) {
        error = "No device found.";
        return false;
    }

    if (lockdownd_client_new_with_handshake(device, &lockdown, "IphoneReader") != LOCKDOWN_E_SUCCESS) {
        error = "Failed to connect to lockdownd.";
        idevice_free(device);
        device = nullptr;
        return false;
    }

    if (lockdownd_start_service(lockdown, "com.apple.mobile.diagnostics_relay", &diag_service) != LOCKDOWN_E_SUCCESS) {
        error = "Failed to start diagnostics relay service.";
        lockdownd_client_free(lockdown);
        idevice_free(device);
        device = nullptr;
        lockdown = nullptr;
        return false;
    }

    if (diagnostics_relay_client_new(device, diag_service, &diag) != DIAGNOSTICS_RELAY_E_SUCCESS) {
        error = "Failed to connect to diagnostics relay.";
        lockdownd_service_descriptor_free(diag_service);
        lockdownd_client_free(lockdown);
        idevice_free(device);
        device = nullptr;
        lockdown = nullptr;
        diag_service = nullptr;
        return false;
    }

    return true;
}
QString IphoneReaderUsb::getBatteryInfo(const QString &udid)
{
    idevice_t device = nullptr;
    diagnostics_relay_client_t diag = nullptr;
    plist_t key = nullptr;
    plist_t result = nullptr;
    QString output;

    // Connect to device
    if (idevice_new(&device, udid.toUtf8().constData()) != IDEVICE_E_SUCCESS)
        return "Failed to connect to device.";

    lockdownd_service_descriptor_t service = nullptr;
    if (lockdownd_start_service(lockdown, "com.apple.mobile.diagnostics_relay", &service) != LOCKDOWN_E_SUCCESS) {
//        error = "Failed to start diagnostics relay service";
//        return;
    }


    if (diagnostics_relay_client_new(device, service, &diag) != DIAGNOSTICS_RELAY_E_SUCCESS) {
//        error = "Failed to create diagnostics relay client";
//        return;
    }

    // Create a plist with a single key
    key = plist_new_array();
    plist_array_append_item(key, plist_new_string("BatteryCurrentCapacity"));

    // Query battery info
    if (diagnostics_relay_query_mobilegestalt(diag, key, &result) == DIAGNOSTICS_RELAY_E_SUCCESS && result) {
        char *xml = nullptr;
        uint32_t len = 0;
        plist_to_xml(result, &xml, &len);
        if (xml) {
            output = QString::fromUtf8(xml);
            free(xml);
        }
    } else {
        output = "Failed to query battery info.";
    }

    // Cleanup
    plist_free(key);
    plist_free(result);
    diagnostics_relay_client_free(diag);
    idevice_free(device);

    return output;
}
void IphoneReaderUsb::disconnect()
{
    if (diag) {
        diagnostics_relay_client_free(diag);
        diag = nullptr;
    }

    if (diag_service) {
        lockdownd_service_descriptor_free(diag_service);
        diag_service = nullptr;
    }

    if (lockdown) {
        lockdownd_client_free(lockdown);
        lockdown = nullptr;
    }

    if (device) {
        idevice_free(device);
        device = nullptr;
    }
}




