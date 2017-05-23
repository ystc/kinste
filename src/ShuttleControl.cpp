//------------------------------------------------------------------------------
// Filename: ShuttleControl.cpp
// Author: Christoph Steinkellner
// Created: 21.07.2016
// Last change: 23.08.2016
//------------------------------------------------------------------------------

#include "inc/ShuttleControl.h"
#include "inc/ShuttleStatus.h"
#include "inc/TASIHandler.h"

//------------------------------------------------------------------------------
ShuttleControl::ShuttleControl(QObject *parent) : QObject(parent)
{
  positions_index = 0;
  move_finished = 1;

  tasiHandler  = new TASIHandler(this);
  shuttleStatus = new ShuttleStatus(this);

  //Timer for sending heartbeat
  heartBeatTimer = new QTimer();
  connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
  heartBeatTimer -> start(FIVE_SEC);

  //Connection status timer
  connectionTimer = new QTimer();
  connect(connectionTimer, SIGNAL(timeout()), this, SLOT(shuttleUnreachable()));
  connectionTimer -> start(TEN_SEC);

  connect(tasiHandler, SIGNAL(gotData(unsigned char*, unsigned int, QString)),
    this, SLOT(gotShuttleData(unsigned char*, unsigned int, QString)));

  connect(shuttleStatus, SIGNAL(sendAck(uint32_t)),
          this, SLOT(sendAckToShuttle(uint32_t)));

  connect(shuttleStatus, SIGNAL(sendStatusMessage(int, QString, bool)),
    this, SLOT(shuttleStatusMessage(int, QString, bool)));

  connect(shuttleStatus, SIGNAL(acknowledgeReceived(bool)), this,
    SLOT(updateConnectionStatus(bool)));

  connect(shuttleStatus, SIGNAL(setShuttleStats(QString, float, float)), this,
    SLOT(changeCurrentData(QString, float, float)));

  connect(shuttleStatus, SIGNAL(sensorData(QStringList)),
          this, SLOT(gotSensorData(QStringList)));
}

//------------------------------------------------------------------------------
void ShuttleControl::gotSensorData(QStringList data)
{
  emit(printSensorData(data));
}

//------------------------------------------------------------------------------
void ShuttleControl::changeCurrentData(QString current_position, float benergy,
                                       float cenergy)
{
  emit(setData(current_position, benergy, cenergy));
}

//------------------------------------------------------------------------------
void ShuttleControl::shuttleUnreachable()
{
  sendCommandStatus("No connection to Shuttle!", "");
  emit(changeConnectionStatus(0));
}

//------------------------------------------------------------------------------
void ShuttleControl::updateConnectionStatus(bool shuttle_is_online)
{
  connectionTimer -> start(TEN_SEC);

  emit(changeConnectionStatus(shuttle_is_online));
}

//------------------------------------------------------------------------------
void ShuttleControl::shuttleStatusMessage(int status, QString status_text,
                                          bool cmd_move)
{
  if(status == 3)
  {
    emit(printShuttleStatus(status, status_text, move_finished));
  }
  else if(positions_index == positions_list.size() || status == 2
          || status == 1)
  {
    move_finished = 1;
    positions_index = 0;
    positions_list.clear();
    emit(printShuttleStatus(status, status_text, move_finished));
  }
  else if(cmd_move && positions_index < positions_list.size())
  {
    move_finished = 0;
    emit(printShuttleStatus(status, status_text, move_finished));
    sendCmdMove("NaDa", 0, positions_list);
  }
}

//------------------------------------------------------------------------------
void ShuttleControl::gotShuttleData(unsigned char *telegram,
                                    unsigned int telegram_length,
                                    QString shuttle_ip)
{
  shuttleStatus -> gotShuttleDataFromControl(telegram, telegram_length,
                                             shuttle_ip);
}

//------------------------------------------------------------------------------
int ShuttleControl::setShuttleIpAddress(QString address, int port)
{
  return(tasiHandler -> setShuttleIp(address, port));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendAckToShuttle(uint32_t sequence_number)
{

  return(tasiHandler -> sendAck(sequence_number));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendHeartBeat(void)
{
  sendCmdGetStatus();
  return(tasiHandler -> sendCmdHeartbeat(0));
}

//------------------------------------------------------------------------------
void ShuttleControl::stopHeartBeat()
{
  if(heartBeatTimer -> isActive())
  {
    heartBeatTimer -> stop();
  }

  if(connectionTimer -> isActive())
  {
    connectionTimer -> stop();
  }
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdSetPosition(QString position)
{
  if(position.isEmpty() == 1)
  {
    sendCommandStatus("No Position given! Set position aborted!", " ");
    return 0;
  }

  sendCommandStatus("Set position", position);
  return( tasiHandler->sendCmdSetPosition( position ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMove(QString tasiOrderId, int subOrderId,
                                QStringList param)
{
  if(param[0].isEmpty() == 1)
  {
    sendCommandStatus("No Position given! Move aborted!", " ");
    return 0;
  }

  positions_list = param;

  if(positions_list.size() < 2)
  {
    positions_index = 0;
  }

  sendCommandStatus("Move to", param[positions_index]);

  return( tasiHandler->sendCmdMove(tasiOrderId, subOrderId,
                                   param[positions_index++]));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdLoad(QString tasiOrderId, QString rackchannel)
{
  if(rackchannel.isEmpty() == 1)
  {
    sendCommandStatus("No Rackchannel given! Load aborted!", " ");
    return 0;
  }
  else
  {
    sendCommandStatus("Loading carrier", rackchannel);
    return( tasiHandler->sendCmdLoad( tasiOrderId, rackchannel ));
  }
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdUnload(QString tasiOrderId, QString rackchannel)
{
  if(rackchannel.isEmpty() == 1)
  {
    sendCommandStatus("No Rackchannel given! Unload aborted!", " ");
    return 0;
  }
  else
  {
    sendCommandStatus("Unloading carrier", rackchannel);
    return( tasiHandler->sendCmdUnload( tasiOrderId, rackchannel ));
  }
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdDoubleLoad(QString tasiOrderId, QString rackchannel)
{
  return( tasiHandler->sendCmdDoubleLoad( tasiOrderId, rackchannel ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdDoubleUnload(QString tasiOrderId,
                                        QString rackchannel)
{
  return( tasiHandler->sendCmdDoubleUnload( tasiOrderId, rackchannel ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdEmptyRackchannel(QString tasiOrderId,
                                            QString rackchannel)
{
  return( tasiHandler->sendCmdEmptyRackchannel( tasiOrderId, rackchannel ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdPrepareToLoad(QString tasiOrderId,
                                         QString rackchannel)
{
  return( tasiHandler->sendCmdPrepareToLoad( tasiOrderId, rackchannel ));
}

int ShuttleControl::sendCmdCompleteLoad(void)
{
  return( tasiHandler->sendCmdCompleteLoad() );
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdPrepareToUnload( QString tasiOrderId,
                                            QString rackchannel)
{
  return( tasiHandler->sendCmdPrepareToUnload( tasiOrderId, rackchannel ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdCompleteUnload(void)
{
  return( tasiHandler->sendCmdCompleteUnload());
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdPrepareToLoadThrough(QString tasiOrderId,
                                                QString rackchannel)
{
  return( tasiHandler->sendCmdPrepareToLoadThrough( tasiOrderId, rackchannel ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdCompleteLoadThrough(void)
{
  return( tasiHandler->sendCmdCompleteLoadThrough());
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdCheckRackchannelOccupation( QString tasiOrderId,
                                                       QString rackchannel)
{
  return( tasiHandler->sendCmdCheckRackchannelOccupation(tasiOrderId,
                                                          rackchannel));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdCheckLHDOccupation( QString tasiOrderId )
{
  return( tasiHandler->sendCmdCheckLHDOccupation( tasiOrderId ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdGetExtStatus( void  )
{
  return( tasiHandler->sendCmdGetExtStatus());
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdGetStatus()
{
  return(tasiHandler -> sendCmdGetStatus());
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntCalibrate( uint8_t sub_cmd, uint8_t motor_nr,
                                         uint8_t steer_context,int16_t diff,
                                         int16_t opt_diff )
{
  switch (sub_cmd){
    case 0:
    {
      switch (motor_nr){
        case 0:
          sendCommandStatus("Calibrate drive motor front", "");
          break;

        case 7:
          sendCommandStatus("Calibrate drive motor rear", "");
          break;

        case 1:
          sendCommandStatus("Calibrate LAM", "");
          break;

        case 3:
          sendCommandStatus("Calibrate finger front", "");
          break;

        case 10:
          sendCommandStatus("Calibrate finger rear", "");
          break;

        case 4:
          sendCommandStatus("Calibrate spindle front", "");
          break;

        case 11:
          sendCommandStatus("Calibrate spindle rear", "");
          break;

        case 2:
          sendCommandStatus("Calibrate steering wheel left front", "");
          break;

        case 5:
          sendCommandStatus("Calibrate steering wheel right front", "");
          break;

        case 9:
          sendCommandStatus("Calibrate steering wheel right rear", "");
          break;

        case 12:
          sendCommandStatus("Calibrate steering wheel left rear", "");
          break;
      }
    }
    break;

    case 1:
      sendCommandStatus("Set actual steering position to 0 degree (straight)",
                        "");
      break;

    case 2:
      sendCommandStatus("Set actual steering position to 90 degree (cross)",
                        "");
      break;

    case 3:
      sendCommandStatus("Save actual data to flash", "");
      break;

    case 4:
      sendCommandStatus("Load default configuration", "");
      break;
  }

  return(tasiHandler->sendCmdMntCalibrate( sub_cmd, motor_nr, steer_context,
                                            diff, opt_diff));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntReference( int motor_nr )
{
  sendCommandStatus("Reference", "");
  return( tasiHandler->sendCmdMntReference( ( uint8_t)motor_nr ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntDrive( int distance, int speed, int ctrl )
{
  sendCommandStatus("Drive", QString::number(distance));
  return( tasiHandler->sendCmdMntDrive( distance, speed, ctrl ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntFinger( int finger, int angle )
{
  sendCommandStatus("Move Fingers", QString::number(angle));
  return( tasiHandler->sendCmdMntFinger( (int8_t)finger, (uint8_t)angle ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntSpindle( int spindle, int position )
{
  sendCommandStatus("Move Spindle", QString::number(position));
  return( tasiHandler->sendCmdMntSpindle( (int8_t)spindle, (int16_t)position ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntMoveLAM( int position, int vmax,
                                       int acceleration, int brake )
{
  sendCommandStatus("Move LAM", QString::number(position));

  return( tasiHandler->sendCmdMntMoveLAM((int16_t)position, (uint16_t)vmax,
                                         (uint16_t)acceleration,
                                         (uint16_t)brake));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntQuickSteer( int degree, int wheels, int delay )
{
  sendCommandStatus("Steer", QString::number(degree));
  return( tasiHandler->sendCmdMntQuickSteer( (int16_t)degree, (uint8_t)wheels,
                                             (uint8_t)delay ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntSteer( int degree, int wheels )
{
  return( tasiHandler->sendCmdMntSteer( (int16_t)degree, (uint8_t)wheels ));
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntGetSensorData( void )
{
  return( tasiHandler->sendCmdMntGetSensorData() );
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntEmergencyStop( void )
{
  sendCommandStatus("EMERGENCY STOP PRESSED", " ");
  return( tasiHandler->sendCmdMntEmergencyStop() );
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntDelCmdList( void )
{
  return( tasiHandler->sendCmdMntDelCmdList() );
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdMntSwReset( void )
{
  sendCommandStatus("Deregulate motors", " ");
  return( tasiHandler->sendCmdMntSwReset() );
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdPrepareForWork( void )
{
  sendCommandStatus("Prepare for work", " ");
  return( tasiHandler->sendCmdPrepareForWork() ) ;
}

//------------------------------------------------------------------------------
void ShuttleControl::sendCommandStatus(QString command, QString position)
{
  shuttleStatus -> buildStatusMessage(INFO, "INFO", command, position, "", 0);
}

//------------------------------------------------------------------------------
void ShuttleControl::printSequenceStatus(QString status_text)
{
  shuttleStatus -> buildStatusMessage(3, "SEQUENCE", status_text, "", "", 0);
}

//------------------------------------------------------------------------------
int ShuttleControl::sendCmdGetSwVersions( void )
{
  return(tasiHandler -> sendCmdGetSwVersions());
}
