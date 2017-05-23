/*
 * TASIHandler.h
 *
 *  Created on: Jul 29, 2014
 *      Author: gern
 *      Last edited by: ystc
 */

#ifndef TASIHANDLER_H_
#define TASIHANDLER_H_

#include <stdint.h>
#include <iostream>
#include <string>
#include <QString>
#include <QUdpSocket>
#include <QStringList>
#include <QTimer>
#include "inc/TASIProtocol2.h"

using std::cout;
using std::endl;

class TASIHandler : public QObject
{
    Q_OBJECT

  public:
    TASIHandler(QObject* parent = 0);
    ~TASIHandler();

    int setShuttleIp(QString ip, int shuttle_port);

    int sendCmdSetPosition( QString position );
    int sendCmdMove(QString tasiOrderId, int subOrderId, QString position );
    int sendCmdLoad( QString tasiOrderId, QString rackchannel );
    int sendCmdDoubleLoad( QString tasiOrderId, QString rackchannel );
    int sendCmdUnload( QString tasiOrderId, QString rackchannel );
    int sendCmdDoubleUnload( QString tasiOrderId, QString rackchannel );
    int sendCmdEmptyRackchannel( QString tasiOrderId, QString rackchannel );

    int sendCmdPrepareToLoad( QString tasiOrderId, QString rackchannel );
    int sendCmdCompleteLoad( void );
    int sendCmdPrepareToUnload( QString tasiOrderId, QString rackchannel);
    int sendCmdCompleteUnload( void );
    int sendCmdPrepareToLoadThrough( QString tasiOrderId, QString rackchannel );
    int sendCmdCompleteLoadThrough( void );
    int sendCmdCheckRackchannelOccupation( QString tasiOrderId, QString rackchannel );
    int sendCmdCheckLHDOccupation( QString tasiOrderId );

    int sendCmdGetExtStatus( void  );
    int sendCmdGetStatus( void  );
    int sendCmdPrepareForWork( void );
    int sendCmdHeartbeat(bool first_connection_check );
    int sendCmdGetSwVersions( void );

    //
    // Maintenance
    //
    int sendCmdMntCalibrate( uint8_t sub_cmd, uint8_t motor_nr, uint8_t steer_context,
                             int16_t diff, int16_t opt_diff );
    int sendCmdMntReference( uint8_t motor_nr );
    int sendCmdMntDrive( int distance, int speed, int ctrl );
    int sendCmdMntMoveLAM( int16_t position, uint16_t vmax, uint16_t acceleration,
                           uint16_t brake );
    int sendCmdMntFinger( int8_t finger, uint8_t angle );
    int sendCmdMntSpindle( int8_t spindle, int16_t position );
    int sendCmdMntSteer( int16_t degree, uint8_t wheels );
    int sendCmdMntQuickSteer( int16_t angle, uint8_t wheels, uint8_t delay );
    int sendCmdMntGetSensorData( void );

    int sendCmdMntEmergencyStop( void );
    int sendCmdMntDelCmdList( void );
    int sendCmdMntSwReset( void );

    int sendAck( uint32_t sequence );

  private:
    int write( unsigned char* buffer, int len );

    QUdpSocket*  udpSocket;
    QTimer *checkConnectionTimer;

    QHostAddress udpDest;
    QHostAddress sender;
    quint16      senderPort;

    uint32_t mySeqNr;
    bool connection_check;
    int counter;

  signals:
     void gotData(unsigned char* buffer, unsigned int len, QString shuttle_ip);
     void connectionConfirmed(bool is_confirmed);

  public slots:
    void readPendingDatagrams( void );

};

#endif /* TASIHANDLER_H_ */
