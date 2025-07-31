#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <IO/plist/plist.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
namespace color {
    Q_NAMESPACE
    enum Role
    {
        OPERATION,
        TITLE,
        LOG,
        OKAY,
        FAIL,
        WARN,
        ELAPSED,
        STATE,
        SKIP,
        TABLE_BACKGROUND,
        TABLE_READONLY,
        TABLE_BOOT,
        JOB_RUNNING,
        JOB_CANCEL,
        JOB_ERROR,
        JOB_FINISH,
        LBL_BACKGROUND,
        LBL_FOREGROUND,
        LBL_HOVERBACKGROUND,
        LBL_HOVERFOREGROUND
    };
    Q_ENUM_NS(Role)
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setLine();
    void setLog(QStringList, bool = true);
    void setState(int = 0, QString = "");
    bool ReadDeviceInfo(bool = 1);

private slots:

    void on_bt_readdevice_info_clicked();
    void setInfoLogs(plist_t dict, const char* key, const char* label,bool = 1);

public:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
