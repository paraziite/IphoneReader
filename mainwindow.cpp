#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <IO/libimobiledevice/libimobiledevice.h>
#include <IO/libimobiledevice/lockdown.h>
#include <IO/libimobiledevice/diagnostics_relay.h>
#include <IO/libimobiledevice/idevicepair.h>
#include <IO/plist/plist.h>

#include <QScrollBar>
#include <QTextCursor>
#include <QMetaEnum>
#include <QColor>
#include <QtCore>

#include <iphonereaderusb.h>

QPalette m_palette;
IphoneReaderUsb reader;
QStringList logs;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}
MainWindow::~MainWindow()
{
    delete ui;
}




//Log Functions
void MainWindow::setLog(QStringList l, bool needDot)
{

    m_palette.setBrush((QPalette::ColorRole)color::Role::OPERATION, QColor("#252525"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::TITLE, QColor("#1751e3"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::LOG, QColor("#252525"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::OKAY, QColor("#1751e3"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::FAIL, QColor("#f23f4e"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::WARN, QColor("#80BFCD"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::ELAPSED, QColor("#7070ff"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::STATE, QColor("#81BFFF"));
    m_palette.setBrush((QPalette::ColorRole)color::Role::SKIP, QColor("#277312"));
    QTextCursor cursor(ui->textBrowser->textCursor());
    cursor.movePosition(QTextCursor::End);

    int start = 0;
    QTextCharFormat format;
    format.setForeground(m_palette.brush((QPalette::ColorRole)color::Role::LOG));

    QString lattr = l.at(0);
    if (lattr.startsWith("$"))
    {
        QStringList lattrs = lattr.split('$', QString::SkipEmptyParts);
        if (lattrs.count())
        {
            for (int i = 0; i < lattrs.count(); i++)
            {
                QString attr = lattrs.at(i).toLower();
                if (attr == "b")
                {
                    format.setFontWeight(QFont::Bold);
                }
                else if (attr == "i")
                {
                    format.setFontItalic(true);
                }
                else if (attr.startsWith("c"))
                {
                    attr = attr.mid(1);
                    if (attr.startsWith("#"))
                    {
                        QColor color(attr);
                        format.setForeground(QBrush(color));
                    }
                    else
                    {
                        color::Role role = static_cast<color::Role>
                                (QMetaEnum::fromType<color::Role>().keyToValue
                                 (attr.toUpper().toStdString().c_str()));
                        format.setForeground(m_palette.brush((QPalette::ColorRole)role));
                    }
                }
                else if (attr.startsWith("f"))
                {
                    attr = attr.mid(1);
                    if (attr.startsWith("#"))
                    {
                        QColor color(attr);
                        format.setBackground(QBrush(color));
                    }
                    else
                    {
                        color::Role role = static_cast<color::Role>
                                (QMetaEnum::fromType<color::Role>().keyToValue
                                 (attr.toUpper().toStdString().c_str()));
                        format.setBackground(m_palette.brush((QPalette::ColorRole)role));
                    }
                }
                else if (attr.startsWith("s"))
                {
                    float size = attr.remove("s").toFloat();
                    if (size)
                        format.setFontPointSize(size);
                }
            }
        }

        needDot = false;
        start = 1;
    }

    for (int j = start; j < l.count(); j++)
    {
        bool with_space = true;
        QString line = l.at(j);
        if (line.isEmpty())
        {
           // cursor.insertText(" ", format);
            continue;
        }

        QStringList attrs = line.split('$', QString::SkipEmptyParts);
        if (attrs.count() > 1)
        {
            for (int i = 1; i < attrs.count(); i++)
            {
                QString attr = attrs.at(i).toLower();

                if (attr == "b")
                {
                    format.setFontWeight(QFont::Bold);
                }
                else if (attr == "i")
                {
                    format.setFontItalic(true);
                }
                else if (attr == "l")
                {
                    format.setForeground(m_palette.brush((QPalette::ColorRole)color::Role::TITLE));
                    format.setAnchor(true);
                    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                    format.setAnchorHref(attrs.at(0));
                    format.setAnchorNames(QStringList() << attrs.at(0));
                }
                else if (attr == "n")
                {
                    with_space = false;
                }
                else if (attr == "d")
                {
                    needDot = false;
                }
                else if (attr.startsWith("c"))
                {
                    attr = attr.mid(1);
                    if (attr.startsWith("#"))
                    {
                        QColor color(attr);
                        format.setForeground(QBrush(color));
                    }
                    else
                    {
                        color::Role role = static_cast<color::Role>
                                (QMetaEnum::fromType<color::Role>().keyToValue
                                 (attr.remove("c").toUpper().toStdString().c_str()));
                        format.setForeground(m_palette.brush((QPalette::ColorRole)role));
                    }
                }
                else if (attr.startsWith("f"))
                {
                    attr = attr.mid(1);
                    if (attr.startsWith("#"))
                    {
                        QColor color(attr);
                        format.setBackground(QBrush(color));
                    }
                    else
                    {
                        color::Role role = static_cast<color::Role>
                                (QMetaEnum::fromType<color::Role>().keyToValue
                                 (attr.toUpper().toStdString().c_str()));
                        format.setBackground(m_palette.brush((QPalette::ColorRole)role));
                    }
                }
                else if (attr.startsWith("s"))
                {
                    attr = attr.mid(1);
                    float size = attr.toFloat();
                    if (size)
                        format.setFontPointSize(size);
                }
            }
        }

        cursor.insertText(attrs.at(0), format);
        if (j < (l.count() - 1))
        {
            if (with_space)
                cursor.insertText(" ", format);
        }
    }
    if (needDot)
    {
        QTextCharFormat format;
        format.setForeground(m_palette.brush((QPalette::ColorRole)color::Role::LOG));
        cursor.insertText("... ", format);
     }

    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
 }
void MainWindow::setLine()
{
    QTextCursor cursor(ui->textBrowser->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n");
    cursor.movePosition(QTextCursor::End);

    QScrollBar *scroll = ui->textBrowser->verticalScrollBar();
    scroll->setValue(scroll->maximum());
}
void MainWindow::setState(int state, QString warning)
{
    switch (state)
    {
    case 0: setLog(QStringList() << "Okay$b$n$cokay", false); break;
    case 1: setLog(QStringList() << "Failed$b$n$cfail", false); break;
    case 2: setLog(QStringList() << "Skip$b$n$cskip", false); break;
    case 3: setLog(QStringList() << "", false); break;
    case 4: setLog(QStringList() << "$b$n$cskip", false); break;
    default: break;
    }

    if (!warning.isEmpty())
    {
        warning = warning.toUpper();
        int index = warning.indexOf("0X");
        if (index != -1)
            warning[index+1] = warning[index+1].toLower();

        if (state < 4)
            setLog(QStringList() << QString("\n  %1$b$cwarn").arg(warning), false);
        else
            setLog(QStringList() << warning, false);
    }
}


//buttons
void MainWindow::on_bt_readdevice_info_clicked()
{
   if(ReadDeviceInfo())
       emit setState(3,"Operation Finished.");
}

//IO FUNCTIONS
void MainWindow::setInfoLogs(plist_t dict, const char* key, const char* label,bool log) {



    logs.clear();
    plist_t node = plist_dict_get_item(dict, key);

    if (node && plist_get_node_type(node) == PLIST_STRING) {
        char* value = nullptr;
        plist_get_string_val(node, &value);

         if(log){
         setLog(logs << QString("%0 :").arg(label), false);
         logs.clear();

         setLog(logs << QString("%0$b$cokay").arg(value), false);
         setLine();
        }
    }
}
bool MainWindow::ReadDeviceInfo(bool log)
{
   bool ret = false;
   logs.clear();
      ui->textBrowser->clear();
    setLog(logs<<"Checking device$b");

    idevice_t device = nullptr;
    if (idevice_new(&device, nullptr) != IDEVICE_E_SUCCESS) {
        if(log)
       setState(1,"device not connected!");

        if(log)
        setLine();

     return false;
    }

   if(log){
   setState();
   setLine();
   }
    lockdownd_client_t lockdown = nullptr;
    if (lockdownd_client_new_with_handshake(device, &lockdown, "qt_reader") != LOCKDOWN_E_SUCCESS) {
        qCritical() << "Could not connect to lockdown.";
        idevice_free(device);

    }

    char* device_name = nullptr;
    if (lockdownd_get_device_name(lockdown, &device_name) == LOCKDOWN_E_SUCCESS) {


    }

    plist_t value = nullptr;
    if (lockdownd_get_value(lockdown, nullptr, nullptr, &value) == LOCKDOWN_E_SUCCESS) {
        if (value && plist_get_node_type(value) == PLIST_DICT) {


            //Get device info
            {
            setInfoLogs(value, "UniqueDeviceID", "UDID",log);
            setInfoLogs(value, "BasebandMasterKeyHash", "BasebandMasterKeyHash",log);
            setInfoLogs(value, "ActivationState", "Active State",log);
            setInfoLogs(value, "BasebandRegionSKU", "BasebandRegionSKU",log);
            setInfoLogs(value, "BasebandCertId", "BasebandCertId",log);
            setInfoLogs(value, "BasebandChipID", "BasebandChipID",log);
            setInfoLogs(value, "BasebandSerialNumber", "BasebandSerialNumber",log);
            setInfoLogs(value, "BasebandStatus", "BasebandStatus",log);
            setInfoLogs(value, "BasebandVersion", "BasebandVersion",log);
            setInfoLogs(value, "BuildVersion", "BuildVersion",log);
            setInfoLogs(value, "HardwareModel", "HardwareModel",log);
            setInfoLogs(value, "HardwarePlatform", "HardwarePlatform",log);
            setInfoLogs(value, "ModelNumber", "ModelNumber",log);
            setInfoLogs(value, "fm-activation-locked", "fm-activation-locked",log);
            setInfoLogs(value, "obliteration", "obliteration",log);
            setInfoLogs(value, "PasswordProtected", "PasswordProtected",log);
            setInfoLogs(value, "PkHash", "PkHash",log);
            setInfoLogs(value, "RegionInfo", "RegionInfo",log);
            setInfoLogs(value, "SIMStatus", "SIMStatus",log);
            setInfoLogs(value, "SoftwareBehavior", "SoftwareBehavior",log);
            setInfoLogs(value, "TelephonyCapability", "TelephonyCapability",log);
            setInfoLogs(value, "TimeIntervalSince1970", "TimeIntervalSince1970",log);
            setInfoLogs(value, "TimeZone", "TimeZone",log);
            setInfoLogs(value, "UniqueChipID", "UniqueChipID",log);
            setInfoLogs(value, "UniqueDeviceID", "UniqueDeviceID");
            setInfoLogs(value, "kCTPostponementStatus", "kCTPostponementStatus",log);
            setInfoLogs(value, "ProximitySensorCalibration", "ProximitySensorCalibration",log);
            setInfoLogs(value, "NonVolatileRAM", "NonVolatileRAM",log);
            setInfoLogs(value, "InternationalMobileEquipmentIdentity", "IMEI",log);
            setInfoLogs(value, "InternationalMobileSubscriberIdentity", "IMSI",log);
            setInfoLogs(value, "ProductType", "Model",log);
            setInfoLogs(value, "ProductVersion", "iOS Version",log);
            setInfoLogs(value, "SerialNumber", "Serial Number",log);
            setInfoLogs(value, "MLBSerialNumber", "MLB Serial",log);
            setInfoLogs(value, "DeviceClass", "Device Class",log);
            setInfoLogs(value, "PhoneNumber", "Phone Number",log);
            setInfoLogs(value, "BluetoothAddress", "Bluetooth",log);
            setInfoLogs(value, "WiFiAddress", "Wi-Fi",log);
            setInfoLogs(value, "BuildVersion", "Build",log);
            setInfoLogs(value, "ICCID", "SIM ICCID",log);
            }


        }
        plist_free(value);
    } else {

        emit setState(1,("Failed to get lockdown device values."));
        ret = false;
    }

    lockdownd_client_free(lockdown);
    idevice_free(device);
    ret = true;

    return ret;
}
