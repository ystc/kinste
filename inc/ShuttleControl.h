/**
  * Filename: ShuttleControl.h
  *
  * Description: Prepares the commands given from the UI
  *
  * Author: Christoph Steinkellner
  * Created: 21.07.2016
  * Last change: 19.08.2016
  */

#ifndef SHUTTLECONTROL_H
#define SHUTTLECONTROL_H

#include <iostream>
#include <stdint.h>
#include <QObject>
#include <QStringList>
#include <QString>
#include <QTimer>

using std::cout;
using std::endl;

class TASIHandler;
class ShuttleStatus;

class ShuttleControl : public QObject
{
    Q_OBJECT

  public:
    ShuttleControl(QObject *parent = 0);

    int setShuttleIpAddress(QString address, int port);
    void printSequenceStatus(QString status_text);
    void stopHeartBeat();

  private:
    void sendCommandStatus(QString command, QString position);

    TASIHandler *tasiHandler;
    ShuttleStatus *shuttleStatus;
    QTimer *heartBeatTimer, *connectionTimer;

    QStringList positions_list;
    int positions_index;
    bool move_finished;

    enum shuttle_status {OK, OK_BUT_SUSPECT, ERROR_OCCURED, INFO, SEQUENCE};
    enum timer_values {ONE_SEC = 1000, FIVE_SEC = 5000,
                       TEN_SEC = 10000, FIFTEEN_SEC = 15000, TWENTY_SEC = 20000};

  signals:
    void printSensorData(QStringList data);
    void printShuttleStatus(int status, QString status_text, bool move_finished);
    void changeConnectionStatus(bool shuttle_is_online);
    void setData(QString current_position, float benergy, float cenergy);

  public slots:
    void gotSensorData(QStringList data);
    void gotShuttleData(unsigned char* telegram, unsigned int telegram_length, QString shuttle_ip);
    void shuttleStatusMessage(int status, QString status_text, bool cmd_move = 0);
    void updateConnectionStatus(bool shuttle_is_online);
    void changeCurrentData(QString current_position, float benergy, float cenergy);

    int sendAckToShuttle(uint32_t sequence_number);
    int sendCmdSetPosition( QString position);
    int sendCmdMove( QString tasiOrderId, int subOrderId, QStringList param);
    int sendCmdLoad( QString tasiOrderId, QString rackchannel);
    int sendCmdUnload( QString tasiOrderId, QString rackchannel);
    int sendCmdDoubleLoad( QString tasiOrderId, QString rackchannel );
    int sendCmdDoubleUnload( QString tasiOrderId, QString rackchannel );
    int sendCmdEmptyRackchannel( QString tasiOrderId, QString param1 );

    int sendCmdPrepareToLoad( QString tasiOrderId, QString rackchannel );
    int sendCmdCompleteLoad( void );
    int sendCmdPrepareToUnload( QString tasiOrderId, QString rackchannel);
    int sendCmdCompleteUnload( void );
    int sendCmdPrepareToLoadThrough( QString tasiOrderId, QString rackchannel );
    int sendCmdCompleteLoadThrough( void );

    int sendCmdCheckRackchannelOccupation( QString tasiOrderId, QString rackchannel );
    int sendCmdCheckLHDOccupation( QString tasiOrderId );

    int sendCmdPrepareForWork( void );
    int sendCmdGetExtStatus( void  );
    int sendCmdGetStatus();

    int sendCmdMntCalibrate( uint8_t sub_cmd, uint8_t motor_nr, uint8_t steer_context, int16_t diff, int16_t opt_diff );
    int sendCmdMntReference( int motor_nr );
    int sendCmdMntDrive( int distance, int speed, int ctrl );
    int sendCmdMntMoveLAM( int position, int speed, int acc, int dec );
    int sendCmdMntFinger( int finger, int angle );
    int sendCmdMntSpindle( int spindle, int position );
    int sendCmdMntSteer( int angle, int wheels );
    int sendCmdMntQuickSteer( int angle, int wheels, int delay );

    int sendCmdMntGetSensorData( void );
    int sendCmdGetSwVersions( void );

    int sendCmdMntEmergencyStop( void );
    int sendCmdMntDelCmdList( void );
    int sendCmdMntSwReset( void );

  private slots:
    int sendHeartBeat(void);
    void shuttleUnreachable();
};

#endif // SHUTTLECONTROL_H
