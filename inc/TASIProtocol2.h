/*
 * TASIProtocol.h
 *
 *  Created on: Aug 25, 2014
 *      Author: gern
 *      Last edited by: ystc
 */

#ifndef TASIPROTOCOL_H_
#define TASIPROTOCOL_H_


#include <stdint.h>

//
// Constants
//
int const LEN_POSITION = 36;
int const LEN_CARRIER_TYPE = 36;

int const TASI_CALIBRATE_SUB_CMD_CALIBRATE          = 0;
int const TASI_CALIBRATE_SUB_CMD_SET_0              = 1;
int const TASI_CALIBRATE_SUB_CMD_SET_90             = 2;
int const TASI_CALIBRATE_SUB_CMD_SAVE               = 3;
int const TASI_CALIBRATE_SUB_CMD_DEFAULT_FLASH      = 4;

int const TASI_CALIBRATE_STEER_CONTEXT_0            = 0;
int const TASI_CALIBRATE_STEER_CONTEXT_90           = 1;

int const TASI_RELOAD_CONF_GRAPH                    = 1;
int const TASI_RELOAD_CONF_BOXHANDLING              = 2;


//
// Tag values (maybe put them into a separate file)
//
int const TAG_SET_POSITION                          = 0x90100;
int const TAG_SET_POSITION_FINISHED                 = 0x90101;
int const TAG_AT_POSITION                           = 0x90103;
int const TAG_MOVE_SHUTTLE                          = 0x90104;
int const TAG_MOVE_SHUTTLE_FINISHED                 = 0x90105;
int const TAG_TRUNCATE_MOVE_SHUTTLE                 = 0x90106;
int const TAG_TRUNCATE_MOVE_SHUTTLE_FINISHED        = 0x90107;
int const TAG_GET_EXACT_POSITION                    = 0x90108;
int const TAG_GET_EXACT_POSITION_FINISHED           = 0x90109;

int const TAG_LOAD_SHUTTLE                          = 0x90120;
int const TAG_LOAD_SHUTTLE_FINISHED                 = 0x90121;
int const TAG_UNLOAD_SHUTTLE                        = 0x90122;
int const TAG_UNLOAD_SHUTTLE_FINISHED               = 0x90123;
int const TAG_CHECK_SHUTTLE_LHD_OCCUPATION          = 0x90130;
int const TAG_CHECK_SHUTTLE_LHD_OCCUPATION_FINISHED = 0x90131;
int const TAG_CHECK_RACKCHANNEL_OCCUPATION          = 0x90132;
int const TAG_CHECK_RACKCHANNEL_OCCUPATION_FINISHED = 0x90133;
int const TAG_EMPTY_RACKCHANNEL                     = 0x90134;
int const TAG_EMPTY_RACKCHANNEL_FINISHED            = 0x90135;
int const TAG_GET_STATUS                            = 0x90140;
int const TAG_GET_STATUS_FINISHED                   = 0x90141;
int const TAG_GET_EXTENDED_STATUS                   = 0x90142;
int const TAG_GET_EXTENDED_STATUS_FINISHED          = 0x90143;
int const TAG_ENERGY_STATUS_CHANGED                 = 0x90145;
int const TAG_LOCAL_MODE_CHANGED                    = 0x90147;
int const TAG_CLEAR_CMD_QUEUE                       = 0x90150;
int const TAG_CLEAR_CMD_QUEUE_FINISHED              = 0x90151;
int const TAG_PERFORM_AUTO_LOCATE                   = 0x90154;
int const TAG_PERFORM_AUTO_LOCATE_FINISHED          = 0x90155;
int const TAG_RELOAD_CONFIG                         = 0x90160;
int const TAG_RELOAD_CONFIG_FINISHED                = 0x90161;
int const TAG_READ_STATISTICS                       = 0x90164;
int const TAG_READ_STATISTICS_FINISHED              = 0x90165;
int const TAG_CLEAR_STATISTICS                      = 0x90166;
int const TAG_CLEAR_STATISTICS_FINISHED             = 0x90167;
int const TAG_FLASH_FPGA                            = 0x90170;
int const TAG_FLASH_FPGA_FINISHED                   = 0x90171;
int const TAG_FLASH_UC                              = 0x90172;
int const TAG_FLASH_UC_FINISHED                     = 0x90173;
int const TAG_GET_SW_VERSIONS                       = 0x90180;
int const TAG_GET_SW_VERSIONS_FINISHED              = 0x90181;
int const TAG_PREPARE_FOR_WORK                      = 0x90182;
int const TAG_PREPARE_FOR_WORK_FINISHED             = 0x90183;

// Entry- and exit commands
int const TAG_PREPARE_TO_LOAD_FROM_ENTRANCE          = 0x90200;
int const TAG_PREPARE_TO_LOAD_FROM_ENTRANCE_FINISHED = 0x90201;
int const TAG_COMPLETE_LOAD_FROM_ENTRANCE            = 0x90202;
int const TAG_COMPLETE_LOAD_FROM_ENTRANCE_FINISHED   = 0x90203;
int const TAG_PREPARE_TO_UNLOAD_TO_EXIT              = 0x90204;
int const TAG_PREPARE_TO_UNLOAD_TO_EXIT_FINISHED     = 0x90205;
int const TAG_COMPLETE_UNLOAD_TO_EXIT                = 0x90206;
int const TAG_COMPLETE_UNLOAD_TO_EXIT_FINISHED       = 0x90207;
int const TAG_PREPARE_TO_LOAD_THROUGH                = 0x90208;
int const TAG_PREPARE_TO_LOAD_THROUGH_FINISHED       = 0x90209;
int const TAG_COMPLETE_LOAD_THROUGH                  = 0x9020A;
int const TAG_COMPLETE_LOAD_THROUGH_FINISHED         = 0x9020B;

// Maintenance messages
int const TAG_MAINTENANCE_CALIBRATE                              = 0x90300;
int const TAG_MAINTENANCE_CALIBRATE_FINISHED                     = 0x90301;
int const TAG_MAINTENANCE_REFERENCE                              = 0x90302;
int const TAG_MAINTENANCE_REFERENCE_FINISHED                     = 0x90303;
int const TAG_MAINTENANCE_DRIVE                                  = 0x90304;
int const TAG_MAINTENANCE_DRIVE_FINISHED                         = 0x90305;
int const TAG_MAINTENANCE_STEER                                  = 0x90306;
int const TAG_MAINTENANCE_STEER_FINISHED                         = 0x90307;
int const TAG_MAINTENANCE_QUICK_STEER                            = 0x90308;
int const TAG_MAINTENANCE_QUICK_STEER_FINISHED                   = 0x90309;
int const TAG_MAINTENANCE_ROTATE                                 = 0x9030A;
int const TAG_MAINTENANCE_ROTATE_FINISHED                        = 0x9030B;
int const TAG_MAINTENANCE_MOVE_LAM                               = 0x9030C;
int const TAG_MAINTENANCE_MOVE_LAM_FINISHED                      = 0x9030D;
int const TAG_MAINTENANCE_MOVE_SPINDLE                           = 0x9030E;
int const TAG_MAINTENANCE_MOVE_SPINDLE_FINISHED                  = 0x9030F;
int const TAG_MAINTENANCE_MOVE_FINGER                            = 0x90310;
int const TAG_MAINTENANCE_MOVE_FINGER_FINISHED                   = 0x90311;
int const TAG_MAINTENANCE_GET_SENSOR_DATA                        = 0x90312;
int const TAG_MAINTENANCE_GET_SENSOR_DATA_FINISHED               = 0x90313;
int const TAG_MAINTENANCE_DEL_CMD_LIST                           = 0x90314;
int const TAG_MAINTENANCE_DEL_CMD_LIST_FINISHED                  = 0x90315;
int const TAG_MAINTENANCE_SW_RESET                               = 0x90316;
int const TAG_MAINTENANCE_SW_RESET_FINISHED                      = 0x90317;
int const TAG_MAINTENANCE_EMERGENCY_STOP                         = 0x90318;
int const TAG_MAINTENANCE_EMERGENCY_STOP_FINISHED                = 0x90319;

int const TAG_INTERNAL_STOP_MOTORS                   = 0x99998;
int const TAG_INTERNAL_STATUS_POLL                   = 0x99999;

// Common messages
int const TAG_HEARTBEAT                             = 0x00080044;
int const TAG_RESET                                 = 0x0009101E;
int const TAG_RESET_FINISHED                        = 0x0009101F;
int const TAG_POWER_UP                              = 0x00091020;
int const TAG_GET_SAFETY_AREA_ACCESS_STATE          = 0x00091016;
int const TAG_GET_SAFETY_AREA_ACCESS_STATE_FINISHED = 0x00091017;
int const TAG_SAFEFTY_AREA_ACCESS_STATE_UPDATE      = 0x00091018;

//
// Flags and stuff...
//
uint8_t const FLAG_CARRIER_UNKNOWN        = 0x01;
uint8_t const FLAG_CARRIER_ISCANONICAL    = 0x02;


uint8_t const TASI_COMMAND_OK             = 0x00;
uint8_t const TASI_COMMAND_SUSP_OK        = 0x01;
uint8_t const TASI_COMMAND_ERROR          = 0x02;


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Telegram structures
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
// Header
//
typedef struct {
	uint8_t   mode_version;        // always 1 (as of now)
	uint8_t   mode_structure;      // always 0 (as of now)
	uint8_t   mode_control;        // 0x07 for a message with payload, 0x86 for an acknowledge
	uint8_t   mode_status;         // always 0 (as of now)
	uint32_t  msg_length;          // length of message including data
	uint32_t  sequence_number;     // sender supplied sequence number. An ack-message should have same seq-nr
	uint16_t  segment_count;       // always 1 (as of now)
	uint16_t  segment_total;       // always 1 (as of now)
	uint32_t  data_length;         // length of data followign this header
	uint32_t  dst;                 // logical destination (what is it in our case ?)
	uint32_t  src;                 // logical source (what is it in our case ?)
} TASI_MSG_HEADER_STRUCT;


typedef struct {
	uint32_t  tag;
	uint32_t  length;
} TASI_TLV_HEADER_STRUCT;



////////////////////////////////////////////////////////////////////////////
//
// Helpers
//
////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t   position_id[ LEN_POSITION ];
	uint8_t   notify;
} TASI_WAYPOINT_STRUCT;

// for internal use
typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint32_t  sub_order_id;
	uint8_t   visited;
	TASI_WAYPOINT_STRUCT way_point;
} EXTENDED_WAYPOINT_STRUCT;

typedef struct {
	uint8_t   flags;
	uint8_t   id[ LEN_POSITION ];
} TASI_CARRIER_ID_STRUCT;

typedef struct {
	uint8_t   carrier_type[ LEN_CARRIER_TYPE ];
	uint16_t  measured_height_mm;
	TASI_CARRIER_ID_STRUCT carrier_id;
} TASI_CARRIER_STRUCT;

typedef struct {
	uint8_t   left_sensor_state_known;
	uint8_t   left_sensor_occupied;
	uint8_t   left_center_sensor_state_known;
	uint8_t   left_center_sensor_occupied;
	uint8_t   right_center_sensor_state_known;
	uint8_t   right_center_sensor_occupied;
	uint8_t   right_sensor_state_known;
	uint8_t   right_sensor_occupied;
}  TASI_LHD_SENSOR_STATUS_STRUCT;


typedef struct {
	uint8_t   capacitor_energy_known;
	float     capacitor_energy_j;
	float     max_capacitor_energy_j;

	uint8_t   battery_energy_known;
	float     battery_energy_j;
	float     max_battery_energy_j;
} TASI_ENERGY_STATUS_STRUCT;

typedef struct {
	uint32_t  status_flags;
	uint8_t   position_id[ LEN_POSITION ];             // to be included
	uint8_t   local_mode;
	TASI_LHD_SENSOR_STATUS_STRUCT lhd_sensor_status;
	TASI_ENERGY_STATUS_STRUCT     energy_status;
} TASI_SHUTTLE_STATUS_STRUCT;

typedef struct {
	uint8_t   ready_for_lift_movement;
	int16_t   sword_position;
	int16_t   spindle_front_position;
	int16_t   spindle_rear_position;
	int16_t   finger_front_angle;
	int16_t   finger_rear_angle;
} TASI_LSF_POSITION_STRUCT;   // LAM, spindle, finger positions

////////////////////////////////////////////////////////////////////////////
//
// Error Descriptors
//
////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t  error_cnt;
	uint16_t error_codes[ 3 ];   // three max
} TASI_ERROR_RETURN_PARAMS_STRUCT;



typedef struct {
	uint16_t  error_code;
	uint8_t   param_count;      // should always be 1 for now
	// uint16_t params[ param_count]                     // list of params
} TASI_ERROR_DESCRIPTOR_STRUCT;


typedef struct {
	uint8_t   result;                                    // return-value of the command
	uint8_t   error_descriptor_count;                    // #errors in the following list
	TASI_ERROR_DESCRIPTOR_STRUCT error_descriptor;  // this is a dynamic array containing TASI_ERROR_DESCRIPTOR_STRUCTS which contains an dynamic array itself
} TASI_COMMAND_STATUS_STRUCT;




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                                                Commands and their structures
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_STD_FINISHED_STRUCT;

typedef struct {
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_MAINTENANCE_STD_FINISHED_STRUCT;


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Positioning
//
//////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t   position_id[ LEN_POSITION ];
} TASI_CMD_SET_POSITION_STRUCT;

typedef struct {
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_SET_POSITION_FINISHED_STRUCT;

typedef struct {
	uint8_t   position_id[ LEN_POSITION ];
} TASI_CMD_AT_POSITION_STRUCT;


typedef struct {
      uint8_t   last_position_id[ LEN_POSITION ];
      uint8_t   next_position_id[ LEN_POSITION ];
      uint32_t  distance_from_last_position;
      TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
      TASI_COMMAND_STATUS_STRUCT command_status;
      // followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_GET_EXACT_POSITION_FINISHED_STRUCT;


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Move / Truncate / Finished...
//
//////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint32_t  sub_order_id;
	uint8_t   way_point_cnt;
	uint8_t   carrier_cnt;
	// followed by wayPointCount x TASI_WAYPOINT_STRUCT + carrierCount x TASI_CARRIER_STRUCT
} TASI_CMD_MOVE_SHUTTLE_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint32_t  sub_order_id;
  TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_MOVE_SHUTTLE_FINISHED_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint8_t   position_id[ LEN_POSITION ];
} TASI_CMD_TRUNCATE_MOVE_SHUTTLE_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_TRUNCATE_MOVE_SHUTTLE_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Load / Load finished
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint8_t   rack_channel_id[ LEN_POSITION ];    // kanal position
	uint8_t   num_carriers_to_load;
	uint8_t   lhd_carrier_cnt;
	uint8_t   channel_carrier_cnt;
	// followed by (lhd_carrier_cnt + channel_carrier_cnt) x TASI_CARRIER_STRUCT
} TASI_CMD_LOAD_SHUTTLE_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_LOAD_SHUTTLE_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Unload / Unload finished
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  uint8_t   tasi_order_id[ LEN_POSITION ];
	uint8_t   rack_channel_id[ LEN_POSITION ];    // kanal position
	uint8_t   num_carriers_to_unload;
	uint8_t   lhd_carrier_cnt;
	// followed by lhd_carrier_cnt x TASI_CARRIER_STRUCT. Attention: Load has channel carrier set as well, unload does not (as defined in TASI protocol)
} TASI_CMD_UNLOAD_SHUTTLE_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_UNLOAD_SHUTTLE_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LHD occupation
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t tasi_order_id[ LEN_POSITION ];
} TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	TASI_LHD_SENSOR_STATUS_STRUCT lhd_sensor_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Get status / send status / extended status
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_STATUS_STRUCT;

typedef struct {
	TASI_LSF_POSITION_STRUCT   positions;
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_EXTENDED_STATUS_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Load/unload from/to conveyer entrance/exit
// The "complete" commands are pure TLV-commands and don't need a struct here.
// So you are going to miss "COMPLETE_TO_LOAD_FROM_ENTRANCE", "COMLETE_TO_UNLOAD_TO_EXIT",....
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef TASI_CMD_LOAD_SHUTTLE_STRUCT    TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_FINISHED_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_COMPLETE_LOAD_FROM_ENTRANCE_FINISHED_STRUCT;

typedef TASI_CMD_UNLOAD_SHUTTLE_STRUCT  TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_FINISHED_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_COMPLETE_UNLOAD_TO_EXIT_FINISHED_STRUCT;

typedef TASI_CMD_LOAD_SHUTTLE_STRUCT    TASI_CMD_PREPARE_TO_LOAD_THROUGH_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_PREPARE_TO_LOAD_THROUGH_FINISHED_STRUCT;
typedef TASI_CMD_STD_FINISHED_STRUCT    TASI_CMD_COMPLETE_LOAD_THROUGH_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Check rackchannel occupation
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t  tasi_order_id[ LEN_POSITION ];
	uint8_t  rack_channel_id[ LEN_POSITION ];
} TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_STRUCT;

typedef struct {
	uint8_t   tasi_order_id[ LEN_POSITION ];
	uint8_t   occupation;
	TASI_SHUTTLE_STATUS_STRUCT shuttle_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Empty rackchannel
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  uint8_t  tasi_order_id[ LEN_POSITION ];
	uint8_t  rack_channel_id[ LEN_POSITION ];
} TASI_CMD_EMTPY_RACKCHANNEL_STRUCT;

// Empty rackchannel finished has same structure as load


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrepareForWork
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_PREPARE_FOR_WORK_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Clear command queue
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_CLEAR_CMD_QUEUE_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Get SW-Versions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t uC_major;
	uint8_t uC_minor;
	uint8_t FPGA_major;
	uint8_t FPGA_minor;
	uint8_t PMM_major;
	uint8_t PMM_minor;
	uint8_t HW_major ;
  uint8_t HW_minor ;
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_GET_SW_VERSIONS_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Read statistics
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint32_t driven_mms;
	uint32_t nr_loads;
	uint32_t nr_unloads;
	uint32_t nr_error_drive;
	uint32_t nr_error_load;
	uint32_t nr_error_unload;

	TASI_COMMAND_STATUS_STRUCT command_status;

	//uint32_t uptimeOS;
	//uint32_t uptimeShuttle;
	//uint8_t  rs_ratio;
} TASI_CMD_READ_STATISTICS_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Reload config
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t bitmask;
} TASI_CMD_RELOAD_CONFIG_STRUCT;

typedef struct {
	TASI_COMMAND_STATUS_STRUCT command_status;
	// followed by error_descriptor_count * TASI_ERROR_DESCRIPTOR_STRUCT (dynamic in size)
} TASI_CMD_RELOAD_CONFIG_FINISHED_STRUCT;


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Maintenance commands
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	uint8_t  sub_cmd;       // 0 = calibrate, 1 = SetDegree0, 2 = SetDegree90
	uint8_t  motor_nr;
	uint8_t  steer_context; // 0 = values valid for zero degree, 1 = values valid for 90 degree, context in case of steering calibration
	int16_t  diff;          // valueu relative to the existing one (e.g. LHD offset = LHD offset + diff)
	int16_t  opt_diff;      // e.g. for inner/outer offset in case of spindle-calibration
} TASI_CMD_MAINTENANCE_CALIBRATE_STRUCT;

typedef struct {
	uint8_t  motor_nr;
} TASI_CMD_MAINTENANCE_REFERENCE_STRUCT;

typedef struct {
	int32_t  distance;
	uint16_t vmax;
	uint8_t  ctrl;
	uint16_t acceleration;
	uint16_t brake;
} TASI_CMD_MAINTENANCE_DRIVE_STRUCT;

typedef struct {
      int16_t  degree;
      uint8_t  wheel;
} TASI_CMD_MAINTENANCE_STEER_STRUCT;

typedef struct {
	int16_t  degree;
	uint8_t  wheel;
	uint8_t  delay;
} TASI_CMD_MAINTENANCE_QUICKSTEER_STRUCT;

typedef struct {
      int16_t  distance;
      uint16_t vmax;
} TASI_CMD_MAINTENANCE_ROTATE_STRUCT;

typedef struct {
      int16_t  position;
      uint16_t vmax;
      uint16_t acceleration;
      uint16_t brake;
} TASI_CMD_MAINTENANCE_MOVE_LAM_STRUCT;

typedef struct {
      int8_t   spindle_id;   // 0 = both, -1 = read, +1 = front
      int16_t  positions; // mm
} TASI_CMD_MAINTENANCE_MOVE_SPINDLE_STRUCT;

typedef struct {
	int8_t   finger_id;  // 0 = both, -1 = rear, +1 = front
	uint8_t  position;   // 90 = up, ~45 = down
} TASI_CMD_MAINTENANCE_MOVE_FINGER_STRUCT;

typedef struct {
	uint16_t laser_analog_right_front_mm;
	uint16_t laser_analog_right_rear_mm;
	uint16_t laser_analog_left_front_mm;
	uint16_t laser_analog_left_rear_mm ;
	uint8_t  laser_digital_right_front;
	uint8_t  laser_digital_right_rear;
	uint8_t  laser_digital_left_front;
	uint8_t  laser_digital_left_rear;
	TASI_LHD_SENSOR_STATUS_STRUCT lhd_sensor_status;
	TASI_COMMAND_STATUS_STRUCT command_status;
} TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED_STRUCT;


/*
typedef struct {

} TASI_CMD_MAINTENANCE_xxx_STRUCT;
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The overall telegram struct as an onion :-)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	TASI_MSG_HEADER_STRUCT  msg_header;
	TASI_TLV_HEADER_STRUCT  tlv_header;

  union {
		TASI_CMD_STD_FINISHED_STRUCT                           std_finished;

		TASI_CMD_SET_POSITION_STRUCT                           set_position;
		TASI_CMD_SET_POSITION_FINISHED_STRUCT                  set_position_finished;
		TASI_CMD_AT_POSITION_STRUCT                            at_position;
		TASI_CMD_GET_EXACT_POSITION_FINISHED_STRUCT            get_exact_position_finished;

		TASI_CMD_MOVE_SHUTTLE_STRUCT                           move_shuttle;
		TASI_CMD_MOVE_SHUTTLE_FINISHED_STRUCT                  move_shuttle_finished;
		TASI_CMD_TRUNCATE_MOVE_SHUTTLE_STRUCT                  truncate_move_shuttle;
		TASI_CMD_TRUNCATE_MOVE_SHUTTLE_FINISHED_STRUCT         truncate_move_shuttle_finished;

		TASI_CMD_LOAD_SHUTTLE_STRUCT                           load_shuttle;
		TASI_CMD_LOAD_SHUTTLE_FINISHED_STRUCT                  load_shuttle_finished;

		TASI_CMD_UNLOAD_SHUTTLE_STRUCT                         unload_shuttle;
		TASI_CMD_UNLOAD_SHUTTLE_FINISHED_STRUCT                unload_shuttle_finished;

		TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_STRUCT           check_shuttle_lhd_occupation;
		TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_FINISHED_STRUCT  check_shuttle_lhd_occupation_finished;

		TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT          prepare_to_load_from_entrance;
		TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_FINISHED_STRUCT prepare_to_load_from_entrance_finished;
		TASI_CMD_COMPLETE_LOAD_FROM_ENTRANCE_FINISHED_STRUCT   complete_load_from_entrance_finished;

		TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_STRUCT              prepare_to_unload_to_exit;
		TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_FINISHED_STRUCT     prepare_to_unload_to_exit_finished;
		TASI_CMD_COMPLETE_UNLOAD_TO_EXIT_FINISHED_STRUCT       complete_unload_to_exit;

		TASI_CMD_PREPARE_TO_LOAD_THROUGH_STRUCT                prepare_to_load_through;
		TASI_CMD_PREPARE_TO_LOAD_THROUGH_FINISHED_STRUCT       prepare_to_load_through_finished;
		TASI_CMD_COMPLETE_LOAD_THROUGH_FINISHED_STRUCT         complete_load_through_finished;

		TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_STRUCT           check_rackchannel_occupation;
		TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_FINISHED_STRUCT  check_rackchannel_occupation_finished;

		TASI_CMD_EMTPY_RACKCHANNEL_STRUCT                      empty_rackchannel;
		TASI_CMD_PREPARE_FOR_WORK_FINISHED_STRUCT              prepare_for_work_finished;

		TASI_CMD_STATUS_STRUCT                                 status;
		TASI_CMD_EXTENDED_STATUS_STRUCT                        extended_status;

		TASI_CMD_RELOAD_CONFIG_STRUCT                          reload_config;
		TASI_CMD_RELOAD_CONFIG_FINISHED_STRUCT                 reload_config_finished;

		//
		// Maintenance commands
		//
		TASI_CMD_MAINTENANCE_STD_FINISHED_STRUCT               maintenance_std_finished;

		TASI_CMD_MAINTENANCE_CALIBRATE_STRUCT			 maintenance_calibrate;
		TASI_CMD_MAINTENANCE_REFERENCE_STRUCT                  maintenance_reference;
		TASI_CMD_MAINTENANCE_DRIVE_STRUCT                      maintenance_drive;
		TASI_CMD_MAINTENANCE_STEER_STRUCT                      maintenance_steer;
		TASI_CMD_MAINTENANCE_QUICKSTEER_STRUCT                 maintenance_quicksteer;
		TASI_CMD_MAINTENANCE_ROTATE_STRUCT                     maintenance_rotate;
		TASI_CMD_MAINTENANCE_MOVE_LAM_STRUCT                   maintenance_move_LAM;
		TASI_CMD_MAINTENANCE_MOVE_SPINDLE_STRUCT               maintenance_move_spindle;
		TASI_CMD_MAINTENANCE_MOVE_FINGER_STRUCT                maintenance_move_finger;
		TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED_STRUCT   maintenance_get_sensor_data_finished;

		TASI_CMD_GET_SW_VERSIONS_FINISHED_STRUCT			 get_sw_versions_finished;
		TASI_CMD_CLEAR_CMD_QUEUE_FINISHED_STRUCT               clear_cmd_queue_finished;
		TASI_CMD_READ_STATISTICS_FINISHED_STRUCT               read_statistics_finished;
  };
} TASI_TELEGRAM_STRUCT;


#pragma pack()


//
// Constant len definitions
//
int const LEN_TASI_MSG_HEADER                         = sizeof( TASI_MSG_HEADER_STRUCT );
int const LEN_TASI_TLV_HEADER                         = sizeof( TASI_TLV_HEADER_STRUCT);
int const LEN_TASI_CMD_STD_FINISHED                   = sizeof( TASI_CMD_STD_FINISHED_STRUCT );

int const LEN_TASI_CMD_SET_POSITION                   = sizeof( TASI_CMD_SET_POSITION_STRUCT );
int const LEN_TASI_CMD_SET_POSITION_FINISHED          = sizeof( TASI_CMD_SET_POSITION_FINISHED_STRUCT );
int const LEN_TASI_CMD_AT_POSITION                    = sizeof( TASI_CMD_AT_POSITION_STRUCT );
int const LEN_TASI_CMD_MOVE_SHUTTLE                   = sizeof( TASI_CMD_MOVE_SHUTTLE_STRUCT );
int const LEN_TASI_CMD_MOVE_SHUTTLE_FINISHED          = sizeof( TASI_CMD_MOVE_SHUTTLE_FINISHED_STRUCT );
int const LEN_TASI_CMD_TRUNCATE_MOVE_SHUTTLE          = sizeof( TASI_CMD_TRUNCATE_MOVE_SHUTTLE_STRUCT );
int const LEN_TASI_CMD_TRUNCATE_MOVE_SHUTTLE_FINISHED = sizeof( TASI_CMD_TRUNCATE_MOVE_SHUTTLE_FINISHED_STRUCT );
int const LEN_TASI_CMD_GET_EXACT_POSITION_FINISHED    = sizeof( TASI_CMD_GET_EXACT_POSITION_FINISHED_STRUCT );
int const LEN_TASI_CMD_LOAD_SHUTTLE                   = sizeof( TASI_CMD_LOAD_SHUTTLE_STRUCT );
int const LEN_TASI_CMD_LOAD_SHUTTLE_FINISHED          = sizeof( TASI_CMD_LOAD_SHUTTLE_FINISHED_STRUCT );
int const LEN_TASI_CMD_UNLOAD_SHUTTLE                 = sizeof( TASI_CMD_UNLOAD_SHUTTLE_STRUCT );
int const LEN_TASI_CMD_UNLOAD_SHUTTLE_FINISHED        = sizeof( TASI_CMD_UNLOAD_SHUTTLE_FINISHED_STRUCT );
int const LEN_TASI_CMD_STATUS                         = sizeof( TASI_CMD_STATUS_STRUCT );
int const LEN_TASI_CMD_EXTENDED_STATUS                = sizeof( TASI_CMD_EXTENDED_STATUS_STRUCT );

int const LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE           = sizeof( TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_STRUCT );
int const LEN_TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_FINISHED  = sizeof( TASI_CMD_PREPARE_TO_LOAD_FROM_ENTRANCE_FINISHED_STRUCT );
int const LEN_TASI_CMD_COMPLETE_LOAD_FROM_ENTRANCE_FINISHED    = sizeof( TASI_CMD_COMPLETE_LOAD_FROM_ENTRANCE_FINISHED_STRUCT );
int const LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT               = sizeof( TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_STRUCT );
int const LEN_TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_FINISHED      = sizeof( TASI_CMD_PREPARE_TO_UNLOAD_TO_EXIT_FINISHED_STRUCT );
int const LEN_TASI_CMD_COMPLETE_UNLOAD_TO_EXIT_FINISHED        = sizeof( TASI_CMD_COMPLETE_UNLOAD_TO_EXIT_FINISHED_STRUCT );
int const LEN_TASI_CMD_PREPARE_TO_LOAD_THROUGH                 = sizeof( TASI_CMD_PREPARE_TO_LOAD_THROUGH_STRUCT );
int const LEN_TASI_CMD_PREPARE_TO_LOAD_THROUGH_FINISHED        = sizeof( TASI_CMD_PREPARE_TO_LOAD_THROUGH_FINISHED_STRUCT );
int const LEN_TASI_CMD_COMPLETE_LOAD_THROUGH_FINISHED          = sizeof( TASI_CMD_COMPLETE_LOAD_THROUGH_FINISHED_STRUCT );

int const LEN_TASI_CMD_GET_SW_VERSIONS_FINISHED                = sizeof( TASI_CMD_GET_SW_VERSIONS_FINISHED_STRUCT );
int const LEN_TASI_CMD_CLEAR_CMD_QUEUE_FINISHED                = sizeof( TASI_CMD_CLEAR_CMD_QUEUE_FINISHED_STRUCT );

int const LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION            = sizeof( TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_STRUCT );
int const LEN_TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_FINISHED   = sizeof( TASI_CMD_STD_FINISHED_STRUCT );       // sizeof( TASI_CMD_CHECK_SHUTTLE_LHD_OCCUPATION_FINISHED_STRUCT );
int const LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION            = sizeof( TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_STRUCT );
int const LEN_TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_FINISHED   = sizeof( TASI_CMD_CHECK_RACKCHANNEL_OCCUPATION_FINISHED_STRUCT );
int const LEN_TASI_CMD_EMTPY_RACKCHANNEL                       = sizeof( TASI_CMD_EMTPY_RACKCHANNEL_STRUCT );
int const LEN_TASI_CMD_PREPARE_FOR_WORK_FINISHED               = sizeof( TASI_CMD_PREPARE_FOR_WORK_FINISHED_STRUCT );
int const LEN_TASI_CMD_READ_STATISTICS_FINISHED                = sizeof( TASI_CMD_READ_STATISTICS_FINISHED_STRUCT );

int const LEN_TASI_CMD_RELOAD_CONFIG                           = sizeof( TASI_CMD_RELOAD_CONFIG_STRUCT );
int const LEN_TASI_CMD_RELOAD_CONFIG_FINISHED                  = sizeof( TASI_CMD_RELOAD_CONFIG_FINISHED_STRUCT );

//
// Maintenance commands
//
int const LEN_TASI_CMD_MAINTENANCE_STD_FINISHED   = sizeof( TASI_CMD_MAINTENANCE_STD_FINISHED_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_CALIBRATE	  = sizeof( TASI_CMD_MAINTENANCE_CALIBRATE_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_REFERENCE      = sizeof( TASI_CMD_MAINTENANCE_REFERENCE_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_DRIVE          = sizeof( TASI_CMD_MAINTENANCE_DRIVE_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_STEER          = sizeof( TASI_CMD_MAINTENANCE_STEER_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_QUICKSTEER     = sizeof( TASI_CMD_MAINTENANCE_QUICKSTEER_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_ROTATE         = sizeof( TASI_CMD_MAINTENANCE_ROTATE_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_MOVE_LAM       = sizeof( TASI_CMD_MAINTENANCE_MOVE_LAM_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_MOVE_SPINDLE   = sizeof( TASI_CMD_MAINTENANCE_MOVE_SPINDLE_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_MOVE_FINGER    = sizeof( TASI_CMD_MAINTENANCE_MOVE_FINGER_STRUCT );
int const LEN_TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED = sizeof( TASI_CMD_MAINTENANCE_GET_SENSOR_DATA_FINISHED_STRUCT );

//
// Helpers
//
int const LEN_TASI_WAYPOINT_STRUCT             = sizeof( TASI_WAYPOINT_STRUCT );
int const LEN_TASI_CARRIER_STRUCT              = sizeof( TASI_CARRIER_STRUCT );
int const LEN_TASI_CARRIER_ID_STRUCT           = sizeof( TASI_CARRIER_ID_STRUCT );
int const LEN_TASI_LHD_SENSOR_STATUS_STRUCT    = sizeof( TASI_LHD_SENSOR_STATUS_STRUCT );
int const LEN_TASI_ENERGY_STATUS_STRUCT        = sizeof( TASI_ENERGY_STATUS_STRUCT );
int const LEN_TASI_SHUTTLE_STATUS_STRUCT       = sizeof( TASI_SHUTTLE_STATUS_STRUCT );
int const LEN_TASI_ERROR_DESCRIPTOR_STRUCT     = sizeof( TASI_ERROR_DESCRIPTOR_STRUCT );
int const LEN_TASI_COMMAND_STATUS_STRUCT       = sizeof( TASI_COMMAND_STATUS_STRUCT );
//int const LEN_TASI_ERROR_PARAMS_STRUCT         = sizeof( TASI_ERROR_PARAMS_STRUCT );


#endif /* TASIPROTOCOL_H_ */
