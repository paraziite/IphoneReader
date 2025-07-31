#ifndef IPHONEREADERUSB_H
#define IPHONEREADERUSB_H


#include <QObject>
#include <QWidget>
#include <IO/libimobiledevice/libimobiledevice.h>
#include <IO/libimobiledevice/lockdown.h>
#include <IO/libimobiledevice/diagnostics_relay.h>
#include <IO/plist/plist.h>
#include <mainwindow.h>
class IphoneReaderUsb : public QObject
{
    Q_OBJECT
public:
    explicit IphoneReaderUsb(QObject *parent = nullptr);
    ~IphoneReaderUsb();

    bool connectToDevice(QString &error);

    QString getBatteryInfo(const QString &udid);
    void disconnect();

private:
    idevice_t device = nullptr;
    lockdownd_client_t lockdown = nullptr;
    diagnostics_relay_client_t diag = nullptr;
    lockdownd_service_descriptor_t diag_service = nullptr;
    QMap<QString, QString> parsePlist(plist_t node);




};

#endif // IPHONEREADERUSB_H
