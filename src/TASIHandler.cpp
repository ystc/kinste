/*
 * TASIHandler.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: gern
 *      Last edited by: ystc
 */

#include <iostream>
#include <arpa/inet.h>
#include "inc/TASIHandler.h"

using namespace std;

TASIHandler::TASIHandler(QObject* parent) : QObject ( parent )
{
  connection_check = 0;

  mySeqNr = 0;

  udpSocket = new QUdpSocket( this );
  connect(udpSocket, SIGNAL( readyRead() ), this, SLOT( readPendingDatagrams()));
}

TASIHandler::~TASIHandler()
{

}

int TASIHandler::setShuttleIp(QString ip, int shuttle_port){
  cout << "shuttle-ip  : " << ip.toStdString() << endl;
  cout << "shuttle-port: " << shuttle_port << endl;
  udpDest.setAddress( ip );
  if (udpSocket -> bind( shuttle_port, QUdpSocket::DontShareAddress) == 1){
    cout << "Binding socket successful" << endl;
    return 1;
  } else {
    cout << "Binding socket failed!" << endl;
    return 0;
  }
}

void TASIHandler::readPendingDatagrams( void )
{
  cout << "Listen to pending datagrams..." << endl;

  while (udpSocket -> hasPendingDatagrams())
  {
    if(connection_check == 0)
    {
      QByteArray datagram;

      datagram.resize(udpSocket->pendingDatagramSize());

      udpSocket -> readDatagram( datagram.data(), datagram.size(), &sender, &senderPort );

      //Delete "::ffff:" whatever this is
      QString reworked_ip = sender.toString().split(":")[3];

      //cout << "Received Datagram" << endl;
      //cout << "sender-ip  : " << reworked_ip.toStdString() << endl;
      //cout << "sender-port: " << senderPort << endl;

      emit(gotData((unsigned char*)datagram.constData(), datagram.size(), reworked_ip));
    }
    else
    {
      emit(connectionConfirmed(1));
      connection_check = 0;
      break;
    }
  }
}

int TASIHandler::write( unsigned char* buffer, int len )
{
  //cout << "TASIHandler write..." << endl;
  qint64 qlen = udpSocket->writeDatagram( (const char*)buffer, (qint64)len, udpDest, 9877);
  //cout << "len of write = " << qlen << endl;
	return qlen;
}

int TASIHandler::sendAck( uint32_t sequence )
{
	TASI_MSG_HEADER_STRUCT ackTlg;

	ackTlg.mode_version    = 1;                            // always 1 (as of now)
	ackTlg.mode_structure  = 0;   	                       // always 0 (as of now)
	ackTlg.mode_control    = 0x86;                         // 0x07 for a message with payload, 0x850006 for an acknowledge
	ackTlg.mode_status     = 0;                            // always 0 (as of now)
	ackTlg.msg_length      = htonl( LEN_TASI_MSG_HEADER ); // length of message including data
	ackTlg.sequence_number = htonl( sequence );            // sender supplied sequence number. An ack-message should have same seq-nr
	ackTlg.segment_count   = htons( 1 );                   // always 1 (as of now)
	ackTlg.segment_total   = htons( 1 );                   // always 1 (as of now)
	ackTlg.data_length     = 0;                            // length of data following this header
	ackTlg.dst             = 0;                            // logical destination (what is it in our case ?)
	ackTlg.src             = 0;                            // logical source (what is it in our case ?)

	return( write( (unsigned char*)&ackTlg, LEN_TASI_MSG_HEADER ));
}

int TASIHandler::sendCmdGetSwVersions( void )
{
  uint8_t buffer[1024];

  TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

  tlg->msg_header.mode_version = 1;
  tlg->msg_header.mode_structure = 1;
  tlg->msg_header.mode_control = 0x07;
  tlg->msg_header.mode_status = 0;
  tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
  tlg->msg_header.sequence_number = htonl( mySeqNr++ );
  tlg->msg_header.segment_count = htons( 1 );
  tlg->msg_header.segment_total = htons( 1 );
  tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
  tlg->msg_header.src = htonl( 99 );
  tlg->msg_header.dst = htonl( 100 );

  tlg->tlv_header.tag = htonl( TAG_GET_SW_VERSIONS );
  tlg->tlv_header.length = 0;

  return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdHeartbeat(bool first_connection_check)
{
  connection_check = first_connection_check;

  uint8_t buffer[1024];

  TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

  tlg->msg_header.mode_version = 1;
  tlg->msg_header.mode_structure = 1;
  tlg->msg_header.mode_control = 0x07;
  tlg->msg_header.mode_status = 0;
  tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
  tlg->msg_header.sequence_number = htonl( mySeqNr++ );
  tlg->msg_header.segment_count = htons( 1 );
  tlg->msg_header.segment_total = htons( 1 );
  tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
  tlg->msg_header.src = htonl( 99 );
  tlg->msg_header.dst = htonl( 100 );

  tlg->tlv_header.tag = htonl( TAG_HEARTBEAT );
  tlg->tlv_header.length = 0;

  return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdGetExtStatus( void  )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_GET_EXTENDED_STATUS );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdGetStatus(void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_GET_STATUS );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdSetPosition( QString  position )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_SET_POSITION );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_SET_POSITION );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_SET_POSITION );
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_SET_POSITION );

	TASI_CMD_SET_POSITION_STRUCT* setPosPtr = (TASI_CMD_SET_POSITION_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

    strcpy( (char*)setPosPtr->position_id, position.toLocal8Bit().data() );

	return ( write((unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_SET_POSITION ));
}

int TASIHandler::sendCmdMove(QString tasiOrderId, int subOrderId, QString position )
{
  cout << "tasi handler move command" << endl;
	uint8_t buffer[4096];
  int wayPointCnt = 1;
  int offset;

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

  if ( position.isEmpty() == 0 )
  {
    //build telegram
		tlg->msg_header.mode_version = 1;
		tlg->msg_header.mode_structure = 1;
		tlg->msg_header.mode_control = 0x07;
		tlg->msg_header.mode_status = 0;
    tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER
                                        + LEN_TASI_CMD_MOVE_SHUTTLE +  LEN_TASI_WAYPOINT_STRUCT );
		tlg->msg_header.sequence_number = htonl( mySeqNr++ );
		tlg->msg_header.segment_count = htons( 1 );
		tlg->msg_header.segment_total = htons( 1 );
    tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MOVE_SHUTTLE
                                         + LEN_TASI_WAYPOINT_STRUCT );
		tlg->msg_header.src = htonl( 99 );
		tlg->msg_header.dst = htonl( 100 );

    tlg->tlv_header.tag = htonl(TAG_MOVE_SHUTTLE);
    tlg->tlv_header.length = htonl(LEN_TASI_CMD_MOVE_SHUTTLE + LEN_TASI_WAYPOINT_STRUCT);

    TASI_CMD_MOVE_SHUTTLE_STRUCT* msPtr
      = (TASI_CMD_MOVE_SHUTTLE_STRUCT*)&buffer[LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER];

    strcpy((char*)msPtr->tasi_order_id, tasiOrderId.toStdString().c_str());
		msPtr->sub_order_id = htonl( subOrderId );
    msPtr->way_point_cnt = wayPointCnt;   // Always one
		msPtr->carrier_cnt = 0;

    offset = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MOVE_SHUTTLE;

    TASI_WAYPOINT_STRUCT* wayPointPtr = (TASI_WAYPOINT_STRUCT*)&buffer [offset];

    wayPointPtr -> notify = 1;

    strcpy((char*)wayPointPtr -> position_id, position.toLocal8Bit().data());

    return (write((unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER
                   + LEN_TASI_CMD_MOVE_SHUTTLE + LEN_TASI_WAYPOINT_STRUCT));
  }
  else
  {
    cout << "syntax: position" << endl;
	}

	return 0;
}

int TASIHandler::sendCmdEmptyRackchannel( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_EMTPY_RACKCHANNEL );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_EMTPY_RACKCHANNEL );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_EMPTY_RACKCHANNEL );
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_EMTPY_RACKCHANNEL );

	strcpy( (char*)tlg->empty_rackchannel.rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)tlg->empty_rackchannel.tasi_order_id, tasiOrderId.toStdString().c_str() );

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_EMTPY_RACKCHANNEL ));
}

int TASIHandler::sendCmdLoad( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + 1 * LEN_TASI_CARRIER_STRUCT );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + 1 * LEN_TASI_CARRIER_STRUCT );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_LOAD_SHUTTLE );
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_LOAD_SHUTTLE + 1 * LEN_TASI_CARRIER_STRUCT );

	TASI_CMD_LOAD_SHUTTLE_STRUCT* loadPtr = (TASI_CMD_LOAD_SHUTTLE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	strcpy( (char*)loadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)loadPtr->tasi_order_id, tasiOrderId.toStdString().c_str() );

	loadPtr->num_carriers_to_load = 1;
	loadPtr->lhd_carrier_cnt = 0;
	loadPtr->channel_carrier_cnt = 1;

	TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE ];

	strcpy( (char*)carrierPtr->carrier_type, "FULL" );
	carrierPtr->measured_height_mm = htons( 300 );

	carrierPtr->carrier_id.flags = 0x00;
	strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + 1 * LEN_TASI_CARRIER_STRUCT ));
}

int TASIHandler::sendCmdDoubleLoad( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	int nrCarrierToLoad;
	int nrLhdCarrier;
	int nrRackchannelCarrier;
	string carrierName;

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;

	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );

	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_LOAD_SHUTTLE );

	TASI_CMD_LOAD_SHUTTLE_STRUCT* loadPtr = (TASI_CMD_LOAD_SHUTTLE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	strcpy( (char*)loadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)loadPtr->tasi_order_id, tasiOrderId.toStdString().c_str() );

	cout << "# carrier to load: ";
	cin >> nrCarrierToLoad;

	cout << "# carrier on shuttle: ";
	cin >> nrLhdCarrier;

	cout << "# carrier in rackchannel: ";
	cin >> nrRackchannelCarrier;

	// to consume "\n" from last cin
	getline( cin, carrierName );

	loadPtr->num_carriers_to_load = nrCarrierToLoad;
	loadPtr->lhd_carrier_cnt = nrLhdCarrier;
	loadPtr->channel_carrier_cnt = nrRackchannelCarrier;

	int idx = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE;

	for ( int i = 0; i < nrLhdCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of LHD-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	for ( int i = 0; i < nrRackchannelCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of Rackchannel-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	cout << "idx = " << idx << endl;

	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_LOAD_SHUTTLE +  (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_LOAD_SHUTTLE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT )));
}

int TASIHandler::sendCmdDoubleUnload( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	int nrCarrierToUnload;
	int nrLhdCarrier;

	string carrierName;

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;

	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );

	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_UNLOAD_SHUTTLE );

	TASI_CMD_UNLOAD_SHUTTLE_STRUCT* unloadPtr = (TASI_CMD_UNLOAD_SHUTTLE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	strcpy( (char*)unloadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)unloadPtr->tasi_order_id, tasiOrderId.toStdString().c_str() );

	cout << "# carrier to unload: ";
	cin >> nrCarrierToUnload;

	cout << "# carrier on shuttle: ";
	cin >> nrLhdCarrier;

	// to consume "\n" from last cin
	getline( cin, carrierName );


	unloadPtr->num_carriers_to_unload = nrCarrierToUnload;
	unloadPtr->lhd_carrier_cnt = nrLhdCarrier;

	int idx = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE;

	for ( int i = 0; i < nrLhdCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of LHD-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	cout << "idx = " << idx << endl;

	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ));
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ));
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_UNLOAD_SHUTTLE +  ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ));

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT )));
}

int TASIHandler::sendCmdUnload( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + 1 *  LEN_TASI_CARRIER_STRUCT );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + 1 *  LEN_TASI_CARRIER_STRUCT );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_UNLOAD_SHUTTLE );
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_UNLOAD_SHUTTLE + 1 *  LEN_TASI_CARRIER_STRUCT );

	//
	// A load struct is used here for unload, that is ok :-)
	//
	TASI_CMD_UNLOAD_SHUTTLE_STRUCT* unloadPtr = (TASI_CMD_UNLOAD_SHUTTLE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	unloadPtr->lhd_carrier_cnt = 1;
	unloadPtr->num_carriers_to_unload = 1;
	strcpy( (char*)unloadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)unloadPtr->tasi_order_id, tasiOrderId.toStdString().c_str());

	TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE ];

	strcpy( (char*)carrierPtr->carrier_type, "Type" );
	carrierPtr->measured_height_mm = htons( 300 );

	carrierPtr->carrier_id.flags = 0x00;
	strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_UNLOAD_SHUTTLE + 1 *  LEN_TASI_CARRIER_STRUCT ));
}


//
// Conveyer entry / exit / loadthrough
//
int TASIHandler::sendCmdPrepareToLoad( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	int nrCarrierToLoad;
	int nrLhdCarrier;
	int nrRackchannelCarrier;

	string carrierName;

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;

	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );

	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_PREPARE_TO_LOAD_FROM_ENTRANCE );

	TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT* loadPtr = (TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	strcpy( (char*)loadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)loadPtr->tasi_order_id, tasiOrderId.toStdString().c_str() );

	nrCarrierToLoad = 1;

	cout << "# carrier on shuttle: ";
	cin >> nrLhdCarrier;

	cout << "# carrier in rackchannel: ";
	cin >> nrRackchannelCarrier;

	// to consume "\n" from last cin
	getline( cin, carrierName );

	loadPtr->num_carriers_to_load = nrCarrierToLoad;
	loadPtr->lhd_carrier_cnt = nrLhdCarrier;
	loadPtr->channel_carrier_cnt = nrRackchannelCarrier;

	int idx = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE;

	for ( int i = 0; i < nrLhdCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of LHD-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	for ( int i = 0; i < nrRackchannelCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of Rackchannel-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	cout << "idx = " << idx << endl;

	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE +  (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + ( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
}

int TASIHandler::sendCmdCompleteLoad( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_COMPLETE_LOAD_FROM_ENTRANCE );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdPrepareToUnload( QString tasiOrderId, QString rackchannel)
{
	uint8_t buffer[1024];

	int nrCarrierToUnload;
	int nrLhdCarrier;
	string carrierName;


	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;

	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );

	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_PREPARE_TO_UNLOAD_TO_EXIT );

	//
	// LHD carrier settings
	//
	TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_STRUCT* unloadPtr = (TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	unloadPtr->lhd_carrier_cnt = 1;
	unloadPtr->num_carriers_to_unload = 1;
	strcpy( (char*)unloadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)unloadPtr->tasi_order_id, tasiOrderId.toStdString().c_str());


	TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT ];

	strcpy( (char*)carrierPtr->carrier_type, "Type" );
	carrierPtr->measured_height_mm = htons( 300 );

	carrierPtr->carrier_id.flags = 0x00;
	strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

	cout << "# carrier to unload: ";
	cin >> nrCarrierToUnload;

	cout << "# carrier on shuttle: ";
	cin >> nrLhdCarrier;

	// to consume "\n" from last cin
	getline( cin, carrierName );


	unloadPtr->num_carriers_to_unload = nrCarrierToUnload;
	unloadPtr->lhd_carrier_cnt = nrLhdCarrier;

	int idx = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT;

	for ( int i = 0; i < nrLhdCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of LHD-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	cout << "idx = " << idx << endl;


	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ) );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ) );
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT ) );

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT + ( nrLhdCarrier * LEN_TASI_CARRIER_STRUCT )));

}

int TASIHandler::sendCmdCompleteUnload( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_COMPLETE_UNLOAD_TO_EXIT );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));

}

int TASIHandler::sendCmdPrepareToLoadThrough( QString tasiOrderId, QString rackchannel )
{
	cout << "TASIHandler::sendCmdPrepareToLoadThrough" << endl;

	uint8_t buffer[1024];

	int nrCarrierToLoad;
	int nrLhdCarrier;
	int nrRackchannelCarrier;

	string carrierName;

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;

	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );

	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_PREPARE_TO_LOAD_THROUGH );

	TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT* loadPtr = (TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT*)&buffer[ LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ];

	strcpy( (char*)loadPtr->rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)loadPtr->tasi_order_id, tasiOrderId.toStdString().c_str() );

	nrCarrierToLoad = 1;

	cout << "# carrier on shuttle: ";
	cin >> nrLhdCarrier;

	cout << "# carrier in rackchannel: ";
	cin >> nrRackchannelCarrier;

	// to consume "\n" from last cin
	getline( cin, carrierName );

	loadPtr->num_carriers_to_load = nrCarrierToLoad;
	loadPtr->lhd_carrier_cnt = nrLhdCarrier;
	loadPtr->channel_carrier_cnt = nrRackchannelCarrier;

	int idx = LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE;

	for ( int i = 0; i < nrLhdCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of LHD-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	for ( int i = 0; i < nrRackchannelCarrier; i++ ) {

		TASI_CARRIER_STRUCT* carrierPtr = (TASI_CARRIER_STRUCT*)&buffer[ idx ];

		cout << "Type of Rackchannel-Carrier " << i+1 << ": ";
		getline( cin, carrierName );

		strcpy( (char*)carrierPtr->carrier_type, carrierName.c_str() );
		carrierPtr->measured_height_mm = htons( 300 );
		carrierPtr->carrier_id.flags = 0x00;
		strcpy( (char*)carrierPtr->carrier_id.id, "TestCarrier");

		idx += sizeof( TASI_CARRIER_STRUCT );
	}

	cout << "idx = " << idx << endl;

	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));
	tlg->tlv_header.length = htonl( LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE +  (( nrLhdCarrier + nrRackchannelCarrier) * LEN_TASI_CARRIER_STRUCT ));

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE + 1 * LEN_TASI_CARRIER_STRUCT ));


   return 0;
}

int TASIHandler::sendCmdCompleteLoadThrough( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_COMPLETE_LOAD_THROUGH );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdCheckRackchannelOccupation( QString tasiOrderId, QString rackchannel )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_CHECK_RACKCHANNEL_OCCUPATION );
	tlg->tlv_header.length = LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION;

	strcpy( (char*)tlg->check_rackchannel_occupation.rack_channel_id, rackchannel.toLocal8Bit().data() );
	strcpy( (char*)tlg->check_rackchannel_occupation.tasi_order_id, tasiOrderId.toStdString().c_str() );


	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION ));
}

int TASIHandler::sendCmdCheckLHDOccupation( QString tasiOrderId )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_CHECK_SHUTTLE_LHD_OCCUPATION );
	tlg->tlv_header.length = LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION;

	strcpy( (char*)tlg->check_shuttle_lhd_occupation.tasi_order_id, tasiOrderId.toStdString().c_str() );


	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION ));
}

int TASIHandler::sendCmdPrepareForWork( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_PREPARE_FOR_WORK );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

//
// Maintenance
//
int TASIHandler::sendCmdMntCalibrate( uint8_t sub_cmd, uint8_t motor_nr, uint8_t steer_context,
                                      int16_t diff, int16_t opt_diff )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_CALIBRATE );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_CALIBRATE);
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_CALIBRATE );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_CALIBRATE;

	tlg->maintenance_calibrate.sub_cmd = sub_cmd;
	tlg->maintenance_calibrate.motor_nr = motor_nr;
	tlg->maintenance_calibrate.steer_context = steer_context;
	tlg->maintenance_calibrate.diff = htons( diff );
	tlg->maintenance_calibrate.opt_diff = htons( opt_diff );

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_CALIBRATE ));
}

int TASIHandler::sendCmdMntReference( uint8_t motor_nr )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_REFERENCE );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_REFERENCE );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_REFERENCE );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_REFERENCE;

	tlg->maintenance_reference.motor_nr = motor_nr;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_REFERENCE ));
}

int TASIHandler::sendCmdMntDrive( int distance, int speed, int ctrl )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_DRIVE );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_DRIVE );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_DRIVE );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_DRIVE;

	tlg->maintenance_drive.distance = htonl( distance );
	tlg->maintenance_drive.vmax = htons( speed );
	tlg->maintenance_drive.ctrl = ctrl;
	tlg->maintenance_drive.acceleration = 0;  // not yet set
	tlg->maintenance_drive.brake = 0; // not yet set

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_DRIVE ));
}

int TASIHandler::sendCmdMntFinger( int8_t finger, uint8_t angle )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_FINGER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_FINGER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_MOVE_FINGER );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_MOVE_FINGER;

	tlg->maintenance_move_finger.finger_id = finger;
	tlg->maintenance_move_finger.position = angle;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_FINGER ));
}

int TASIHandler::sendCmdMntSpindle( int8_t spindle, int16_t position )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_SPINDLE );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_SPINDLE );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_MOVE_SPINDLE );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_MOVE_SPINDLE;

	tlg->maintenance_move_spindle.spindle_id = spindle;
	tlg->maintenance_move_spindle.positions = htons( position );

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_SPINDLE ));
}

int TASIHandler::sendCmdMntMoveLAM( int16_t position, uint16_t vmax, uint16_t acceleration, uint16_t brake )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_LAM );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_LAM );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_MOVE_LAM );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_MOVE_LAM;

	tlg->maintenance_move_LAM.position = htons( position );
	tlg->maintenance_move_LAM.vmax = htons( vmax );
	tlg->maintenance_move_LAM.acceleration = htons( acceleration );
	tlg->maintenance_move_LAM.brake = htons( brake );

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_MOVE_LAM ));
}

int TASIHandler::sendCmdMntSteer( int16_t degree, uint8_t wheels )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_STEER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_STEER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_STEER );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_STEER;

	tlg->maintenance_steer.degree = htons( degree );
	tlg->maintenance_steer.wheel = wheels;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_STEER ));
}

int TASIHandler::sendCmdMntQuickSteer( int16_t degree, uint8_t wheels, uint8_t delay )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_QUICKSTEER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_QUICKSTEER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_QUICK_STEER );
	tlg->tlv_header.length = LEN_TASI_CMD_MAINTENANCE_QUICKSTEER;

	tlg->maintenance_quicksteer.degree = htons( degree );
	tlg->maintenance_quicksteer.wheel = wheels;
	tlg->maintenance_quicksteer.delay = delay;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER + LEN_TASI_CMD_MAINTENANCE_QUICKSTEER ));
}

int TASIHandler::sendCmdMntGetSensorData( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_GET_SENSOR_DATA );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdMntEmergencyStop( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_EMERGENCY_STOP );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdMntDelCmdList( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_DEL_CMD_LIST );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}

int TASIHandler::sendCmdMntSwReset( void )
{
	uint8_t buffer[1024];

	TASI_TELEGRAM_STRUCT* tlg = (TASI_TELEGRAM_STRUCT*)buffer;

	tlg->msg_header.mode_version = 1;
	tlg->msg_header.mode_structure = 1;
	tlg->msg_header.mode_control = 0x07;
	tlg->msg_header.mode_status = 0;
	tlg->msg_header.msg_length = htonl( LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER );
	tlg->msg_header.sequence_number = htonl( mySeqNr++ );
	tlg->msg_header.segment_count = htons( 1 );
	tlg->msg_header.segment_total = htons( 1 );
	tlg->msg_header.data_length = htonl( LEN_TASI_TLV_HEADER );
	tlg->msg_header.src = htonl( 99 );
	tlg->msg_header.dst = htonl( 100 );

	tlg->tlv_header.tag = htonl( TAG_MAINTENANCE_SW_RESET );
	tlg->tlv_header.length = 0;

	return ( write( (unsigned char*)buffer, LEN_TASI_MSG_HEADER + LEN_TASI_TLV_HEADER ));
}
