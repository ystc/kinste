//------------------------------------------------------------------------------
// Filename: ShuttleStatus.cpp
// Author: Christoph Steinkellner
// Created: 21.07.2016
// Last change: 23.08.2016
//------------------------------------------------------------------------------

#include "inc/TASIProtocol2.h"
#include "inc/ShuttleStatus.h"
#include <arpa/inet.h>

//------------------------------------------------------------------------------
ShuttleStatus::ShuttleStatus(QObject *parent) : QObject(parent)
{
  first_command_flag = 1;
  sequence_started = 0;
}

//------------------------------------------------------------------------------
void ShuttleStatus::gotShuttleDataFromControl
  (unsigned char* rcv_shuttle_telegram, unsigned int telegram_length,
   QString shuttle_ip)
{
  TASI_TELEGRAM_STRUCT* telegram_struct;
  TASI_TLV_HEADER_STRUCT* tlv_header_pointer;
  unsigned int telegram_index = 0;
  shuttle_ip_address = shuttle_ip;
  pointer_offset = 0;

  /*cout << endl << "!!!!!!!!!!!!!!!!!!!!  Got datagram from TASI
   * !!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cout << "datagram len = " << telegram_length << endl;*/

  if ( telegram_length < LEN_TASI_MSG_HEADER ) {
    cout << "Telegram to short, there is no meaningful data " << endl;
    return;
  }

  telegram_struct = (TASI_TELEGRAM_STRUCT*)rcv_shuttle_telegram;

  if(telegram_struct -> msg_header.mode_control == 0x86)
  {
    emit(acknowledgeReceived(1));
  }

  // check the whole message (could have more than one TLV-structure in it)
  telegram_index = LEN_TASI_MSG_HEADER;

  //If telegram is no ACK go through TAGS to find out what command got finished
  while ( telegram_index < telegram_length )
  {
    cout << "telegram_index = " << telegram_index << ", whole_msg_len = "
         << telegram_length << endl;

    tlv_header_pointer =
        (TASI_TLV_HEADER_STRUCT*)&rcv_shuttle_telegram[telegram_index ];

    //
    // for now, we assume only one TLV per UDP-packet
    //

    switch ( ntohl( tlv_header_pointer -> tag )) {

    case TAG_AT_POSITION:
    {
      cout << "TAG > AtPosition <" << endl;
      cout << " tag length = " << ntohl( telegram_struct -> tlv_header.length )
           << endl;
      cout << " set position to \""
           << telegram_struct -> at_position.position_id << "\"" << endl;

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_GET_STATUS_FINISHED:
    {
      char* current_position = reinterpret_cast <char*>
        (telegram_struct -> status.shuttle_status.position_id);

      cenergy = ntohf(telegram_struct -> status.shuttle_status
                      .energy_status.capacitor_energy_j);
      benergy = ntohf(telegram_struct -> status.shuttle_status
                      .energy_status.battery_energy_j);

      cout << "CAPENERGY: " << cenergy << endl << "BATENERGY: "
           << benergy << endl;

      emit(setShuttleStats(QString(current_position), benergy, cenergy));
      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_SET_POSITION_FINISHED:
    {
      pointer_offset = 36 + LEN_TASI_SHUTTLE_STATUS_STRUCT;

      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram,
                   pointer_offset + 36 + LEN_TASI_SHUTTLE_STATUS_STRUCT + 2),
                   "Set position");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
      pointer_offset = 0;
    }
    break;

    case TAG_MOVE_SHUTTLE_FINISHED:
    {
      pointer_offset = 36 + 4 + LEN_TASI_SHUTTLE_STATUS_STRUCT;

      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset + 2),
                   "Move to");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
      pointer_offset = 0;
    }
    break;

    case TAG_PREPARE_FOR_WORK_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset),
                   "Prepare for work");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_LOAD_SHUTTLE_FINISHED:
    {
      pointer_offset = 36 + LEN_TASI_SHUTTLE_STATUS_STRUCT;

      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset + 2),
                   "Load");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
      pointer_offset = 0;
    }
    break;

    case TAG_UNLOAD_SHUTTLE_FINISHED:
    {
      pointer_offset = 36 + LEN_TASI_SHUTTLE_STATUS_STRUCT;

      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset + 2),
                   "Unload");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
      pointer_offset = 0;
    }
    break;

    case TAG_MAINTENANCE_REFERENCE_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram,
                   pointer_offset), "Reference");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_DRIVE_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT), "Drive");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_QUICK_STEER_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT), "Steer");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_MOVE_LAM_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT), "Move LAM");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_MOVE_SPINDLE_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT),
                   "Move Spindle");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_MOVE_FINGER_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT),
                   "Move Fingers");

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_CALIBRATE_FINISHED:
    {
      statusMaster(getCommandStatus(rcv_shuttle_telegram, pointer_offset),
                   getErrorCode(rcv_shuttle_telegram, pointer_offset
                                + LEN_TASI_COMMAND_STATUS_STRUCT),
                   "Calibrate");

       emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_MAINTENANCE_GET_SENSOR_DATA_FINISHED:
    {
      getSensorData(rcv_shuttle_telegram);

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    case TAG_GET_SW_VERSIONS_FINISHED:
    {
      getSoftwareVersions(rcv_shuttle_telegram);

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));
    }
    break;

    default:
      cout << "Unknown tag 0x" << hex << ntohl( tlv_header_pointer->tag )
           << dec << " with len " << ntohl( tlv_header_pointer->length )
           << endl;
      cout << "Quitting anyway ;-)" << endl;

      emit(sendAck(ntohl(telegram_struct -> msg_header.sequence_number)));

      break;
    }

    //
    // Move forward to next tag. Careful to count also the header itself
    //
    telegram_index += LEN_TASI_TLV_HEADER + ntohl( tlv_header_pointer->length );
  }
}

//------------------------------------------------------------------------------
float ShuttleStatus::ntohf(float x)
{
   union foo {
    float f;
    uint32_t i;
   }

   foo = {.f = x};
   foo.i = ntohl(foo.i);

   return foo.f;
}

//------------------------------------------------------------------------------
int ShuttleStatus::getCommandStatus(unsigned char* shuttle_telegram,
                                    int pointer_offset)
{
  TASI_COMMAND_STATUS_STRUCT* command_pointer =
      (TASI_COMMAND_STATUS_STRUCT*) &shuttle_telegram
      [LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + pointer_offset];
  return (int)command_pointer -> result;
}

//------------------------------------------------------------------------------
int ShuttleStatus::getErrorCode(unsigned char* shuttle_telegram,
                                int pointer_offset)
{
  TASI_ERROR_DESCRIPTOR_STRUCT* error_pointer =
      (TASI_ERROR_DESCRIPTOR_STRUCT*) &shuttle_telegram
      [LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + pointer_offset];
  return  ntohs(error_pointer -> error_code);
}

//------------------------------------------------------------------------------
void ShuttleStatus::statusMaster(int command_status, int error_code,
                                 QString command_type)
{
  cout << "Building Shuttle status..." << endl;
  cout << "command status: " << command_status << endl;
  cout << "error code: " << error_code << endl;

  switch (command_status)
  {
    case OK:
    {
      buildStatusMessage(OK, "INFO", command_type,"", "finished", 1);
    }
    break;

    case OK_BUT_SUSPECT:
    {
      buildErrorMessage(OK_BUT_SUSPECT, "WARNING", "Error while", command_type,
                        error_code);
    }
    break;

    case ERROR_OCCURED:
    {
      buildErrorMessage(OK_BUT_SUSPECT, "WARNING", "Error while", command_type,
                        error_code);
    }
    break;
  }
}

//------------------------------------------------------------------------------
void ShuttleStatus::buildStatusMessage(int status, QString severity,
                                       QString param1, QString param2,
                                       QString param3, bool cmd_move)
{
  if(status == INFO)
  {
    next_shuttle_position = param2;
  }
  else
  {
    if(next_shuttle_position.isEmpty() == 0
       && next_shuttle_position.size() > 10)
    {
      emit(setShuttleStats(next_shuttle_position, benergy, cenergy));
    }
  }

  QDateTime time;
  QString final_status_text;
  unsigned int time_diff;

  time = QDateTime::currentDateTime();
  time_diff = last_cmd_time.msecsTo(time);

  if(first_command_flag == 1)
  {
    time_diff = 0;
    first_command_flag = 0;
  }

  final_status_text = QString("%1  +%7   %2    [%3]:  %4 %5 %6")
    .arg(time.toString("yyyy-MM-dd hh:mm:ss.zzz"))
    .arg(severity).arg(shuttle_ip_address).arg(param1)
    .arg(next_shuttle_position).arg(param3).arg(time_diff);


  emit(sendStatusMessage(status, final_status_text, cmd_move));

  last_cmd_time = QDateTime::currentDateTime();
  final_status_text.clear();

    cout << "Build status message finished" << endl;
}

//------------------------------------------------------------------------------
void ShuttleStatus::buildErrorMessage(int status, QString severity,
                                      QString param1, QString param2,
                                      int error_nr)
{
  QDateTime time;
  QString final_status_text;
  unsigned int time_diff;
  int error_type = 0;

  errorDb = new QSqlDatabase();
  *errorDb = QSqlDatabase::addDatabase("QSQLITE");
  errorDb -> setDatabaseName("lib/errorNumbers.db");

  if (errorDb -> open())
  {
    cout<< "Database opened successfully!" << endl;
  }
  else
  {
    cout << "Database connection failed!" << endl;
  }


  if (error_nr < 2048)
  {
    error_nr = error_nr - 1024;
    error_type = 1;
  }
  else if (error_nr > 2048)
  {
    error_nr = error_nr - 2048;
    error_type = 2;
  }

  cout << "ERROR NR: " << error_nr << endl;

  QString query_string = tr("SELECT errorcodes, errorcheck, errornr "
    "FROM errors WHERE errorcheck like %1 and errornr like %2")
      .arg(QString::number(error_nr)).arg(error_type);

  query = new QSqlQuery(*errorDb);
  query -> exec(query_string);
  query -> next();

  QString code = query -> value(0).toString();
  int number = query -> value(1).toInt();

  if(number == 0){
    code = "Unknown error!";
  }

  time = QDateTime::currentDateTime();
  time_diff = last_cmd_time.msecsTo(time);

  if(first_command_flag == 1)
  {
    time_diff = 0;
    first_command_flag = 0;
  }

  final_status_text = QString("%1  +%7   %2    [%3]:  %4 %5 %6\n    "
                              "Error Number: %8\n    %9")
    .arg(time.toString("yyyy-MM-dd hh:mm:ss.zzz"))
    .arg(severity).arg(shuttle_ip_address).arg(param1).arg(param2)
    .arg(next_shuttle_position).arg(time_diff).arg(QString::number(number))
    .arg(code);

  query -> finish();
  errorDb -> close();
  delete errorDb;

  emit(sendStatusMessage(status, final_status_text, 0));
  last_cmd_time = QDateTime::currentDateTime();
}

//------------------------------------------------------------------------------
void ShuttleStatus::getSensorData(unsigned char *shuttle_telegram)
{
  cout << "CREATING SENSORLIST" << endl;
  QStringList sensor_label_string;

  TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED_STRUCT* sensor_pointer =
      (TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED_STRUCT*)
      &shuttle_telegram[LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER];

  TASI_LHD_SENSOR_STATUS_STRUCT* lhd_sensor_pointer =
      (TASI_LHD_SENSOR_STATUS_STRUCT*)
      &shuttle_telegram[LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + 12];

  sensor_label_string.append
      ("Edge front left: "
       + QString::number(sensor_pointer -> laser_digital_left_front));

  sensor_label_string.append
      ("Edge front right: "
       + QString::number(sensor_pointer -> laser_digital_right_front));

  sensor_label_string.append
      ("Edge rear left: "
       + QString::number(sensor_pointer -> laser_digital_left_rear));

  sensor_label_string.append
      ("Edge rear right: "
       + QString::number(sensor_pointer -> laser_digital_right_rear));

  sensor_label_string.append
      ("Box left: "
       + QString::number(lhd_sensor_pointer -> left_sensor_occupied));

  sensor_label_string.append
      ("Box left center: "
       + QString::number(lhd_sensor_pointer -> left_center_sensor_occupied));

  sensor_label_string.append
      ("Box right: "
       + QString::number(lhd_sensor_pointer -> right_sensor_occupied));

  sensor_label_string.append
      ("Box right center: "
       + QString::number(lhd_sensor_pointer -> right_center_sensor_occupied));

  emit(sensorData(sensor_label_string));
}

//------------------------------------------------------------------------------
void ShuttleStatus::getSoftwareVersions(unsigned char* shuttle_telegram)
{
  QStringList sw_versions;

  TASI_CMD_GET_SW_VERSIONS_FINISHED_STRUCT* sw_pointer =
      (TASI_CMD_GET_SW_VERSIONS_FINISHED_STRUCT*)
      &shuttle_telegram[LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER];

  sw_versions.append("uC-Version: " + QString::number(sw_pointer -> uC_major));
  sw_versions.append("FPGA-Version: "
                     + QString::number(sw_pointer -> FPGA_major));
  sw_versions.append("PMM-Version: "
                     + QString::number(sw_pointer -> PMM_major));
  sw_versions.append("HW-Version: " + QString::number(sw_pointer -> HW_major));

  emit(sensorData(sw_versions));
}
