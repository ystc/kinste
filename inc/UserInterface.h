/**
  * Filename: UserInterface.h
  *
  * Description: This class builds the UI and connects it with the Shuttle via ShuttleControl.
  *
  * Author: Christoph Steinkellner
  * Created: 21.07.2016
  * Last change: 24.08.2016
  */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QStringList>
#include <QLabel>
#include <QProgressBar>
#include <QListWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QEvent>
#include <QMouseEvent>

class MaintenanceTab;
class SequenceTab;
class ShuttleControl;
class CostumTabWidget;
class MainPage;
class SingleCommandsTab;
class Shuttle;

class UserInterface : public QWidget
{
    Q_OBJECT

  public:
    UserInterface(Shuttle *shuttleData, QWidget* parent = 0);
    ~UserInterface();

  protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *);

  private:
    void createLayout();
    void enable();
    void disable();
    void updateAllComboBoxes();
    QString simplify(QString positions);

    Shuttle *shuttle;
    MainPage *main_window;
    SingleCommandsTab *singleCommandsTab;
    SequenceTab *sequenceTab;
    MaintenanceTab *maintenanceTab;
    ShuttleControl *shuttleControl;
    QGroupBox *footerGroup, *headerGroup, *bodyGroup, *statusGroup, *controlsGroup;
    QProgressBar *capsEnergyBar, *batteryEnergyBar;
    CostumTabWidget *tabWidget;
    QTimer *commandTimer;
    QPushButton *buttonPrepare, *buttonReference, *buttonReset, *shuttleIp,
      *buttonClearBar, *buttonSaveLog, *buttonEmergencyStop, *buttonSetPos, *buttonMoveTo,
      *buttonLoad, *buttonUnload, *buttonRefreshData, *buttonCloseWindow, *buttonMinWindow, *buttonMaxWindow;
    QGridLayout *mainLayout,*footerGrid, *leftGrid, *versionsGrid, *headerGrid, *statusGrid, *controlsGrid, *bodyGrid;
    QTextEdit *log;
    QLabel *labelMoveTo, *labelSetPos, *labelLoad, *labelUnload, *labelEnergy, *labelIPAddress, *labelShuttleName, *labelPosition;
    QListWidget *listSensorData;
    QComboBox *lineMoveTo, *lineSetPos, *lineLoad, *lineUnload;
    QString tasi_order_id, shuttle_ip_address, next_position;
    QStringList combobox_position_list;
    int sub_order_id;
    bool sequence_running;

    enum timer_values {ONE_SEC = 1000, FIVE_SEC = 5000, TEN_SEC = 10000, FIFTEEN_SEC = 15000,
      TWENTY_SEC = 20000};

  public slots:
    void setConnectionStatus(bool);

  private slots:
    void getLevel();
    void commandTimeOut();
    void sendSequenceStatus(QString status_text);
    void executeMove(QStringList positions);
    void executeSetPosition(QString position);
    void executeLoad(QString rackchannel);
    void executeUnload(QString rackchannel);
    void setShuttleData(QString current_position, float benergy, float cenergy);
    void savePosition(QString last_used_position);
    void updatePositionList(QComboBox* current_combobox);
    void setSequenceStarted(bool started);
    void printShuttleStatusMessage(int status, QString status_text, bool move_finished);
    void printShuttleData(QStringList data_list);

    void mntDriveSlot(int distance, int speed, unsigned char ctrl);
    void mntFingerSlot(int finger, int angle);
    void mntLAMSlot(int position, int vmax, int acceleration, int brake);
    void mntSteerSlot(int degree, int wheels, int delay);
    void mntSpindleSlot(int spindle, int position);
    void mntCalibrate(int sub_cmd, int motor_nr, int steer_context, int diff, int opt_diff);
    void buttonRefreshDataClicked();
    void buttonEmergencyStopClicked();
    void buttonPrepareClicked();
    void buttonGetPosClicked();
    void buttonReferenceClicked();
    void buttonResetClicked();
    void buttonClearBarClicked();
    void buttonSaveLogClicked();

   signals:
    void connection(int connected, bool sequence_status);
    void userInterfaceClosed();
};
#endif // USERINTERFACE_H
