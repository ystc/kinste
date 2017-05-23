/**
  * Filename: ShuttleStatus.h
  *
  * Description: Receives the shuttle status and prepares the message for the UI
  *
  * Author: Christoph Steinkellner
  * Created: 21.07.2016
  * Last change: 23.08.2016
  */

#ifndef SHUTTLESTATUS_H
#define SHUTTLESTATUS_H

#include <QObject>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <iostream>
#include <stdint.h>
#include <QStringList>

using std::cout;
using std::endl;
using std::hex;
using std::dec;

class QString;

class ShuttleStatus : public QObject
{
    Q_OBJECT

  public:
    ShuttleStatus(QObject *parent = 0);

    void gotShuttleDataFromControl(unsigned char* telegram, unsigned int telegram_length,
                                   QString shuttle_ip);
    void buildStatusMessage(int status, QString severity, QString param1,
                            QString param2, QString param3, bool cmd_move);
    void buildErrorMessage(int status, QString severity, QString param1,
                           QString param2, int error_nr);
  private:
    float ntohf(float x);
    int getCommandStatus(unsigned char* shuttle_telegram, int pointer_offset);
    int getErrorCode(unsigned char* shuttle_telegram, int pointer_offset);
    void getSensorData(unsigned char* shuttle_telegram);
    void getSoftwareVersions(unsigned char* shuttle_telegram);
    void statusMaster(int command_status, int error_code, QString command_type);

    enum shuttle_status {OK, OK_BUT_SUSPECT, ERROR_OCCURED, INFO};
    int pointer_offset;
    bool first_command_flag, sequence_started;
    float benergy, cenergy;
    QDateTime last_cmd_time;
    QString shuttle_ip_address, next_shuttle_position;
    QSqlDatabase *errorDb;
    QSqlQuery *query;

  signals:
    void sensorData(QStringList sensor_data);
    void setShuttleStats(QString, float, float);
    void sendAck(uint32_t sequence);
    void sendStatusMessage(int status, QString final_status_text, bool cmd_move);
    void acknowledgeReceived(bool shuttle_con_status);
};

#endif // SHUTTLESTATUS_H
