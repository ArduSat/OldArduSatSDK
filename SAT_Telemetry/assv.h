/**
	@brief  ASSV

   Copyright 2013 NanoSatisfi, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
    
	@author Jeroen Cappaert and NanoSatisfi, Inc.
    @date May 2013
  
*/


#ifndef ASSV_H
#define ASSV_H


/** CSP Address & elemental parameters **/
#define ASSV_I2C_SPEED 	 		100
#define ASSV_SYS_CLK 	 		F_CPU
#define ASSV_I2C_ADDR     		0x20

#define NODE_ASSV        		4
#define NODE_ASSV_X			20

/** port numbers **/
#define ASSV_PORT_FTP			7
#define ASSV_PORT_LED			8
#define ASSV_PORT_LOAD_IMG		9
#define ASSV_PORT_RAM_TO_ROM	10
#define ASSV_PORT_JUMP			11
#define ASSV_PORT_HK			12
#define ASSV_PORT_TEST			13
#define ASSV_PORT_UPLOAD		14
#define ASSV_PORT_LOG_SERIAL	15
#define ASSV_PORT_FLASH_ARDUINO		16
#define ASSV_PORT_POWER_ARDUINO		17
#define ASSV_PORT_POWER_SENSOR		18

/** Node CSP addresse for ArduSat-X */
#define NODE_OBC_X      17
#define NODE_EPS_X      18
#define NODE_HUB_X      19
#define NODE_COM_X      21

/** Sensor defs */
#define N_ARD		16
#define N_SENS		4
#define ASSV_SPECTRUINO 0
#define ASSV_GPS 	1
#define ASSV_GEIGER	2
#define ASSV_CAMERA	3

/** added for non-avr systems  */
#ifndef _BV
	#define _BV(bit) (1 << bit)
#endif

typedef struct __attribute__((packed)){
	uint32_t counter_boot;
	int8_t sd_status;		//! status of SD card
	uint8_t bootcause;
} assv_boot_t; // 6 bytes packed


typedef struct __attribute__((packed)){
	uint8_t node;		//! node to be flashed
	uint8_t* rpath;		//! remote path of file to be flashed on arduino
} assv_arduino_flash_t;

typedef struct __attribute__((packed)){
	uint8_t node;
	uint8_t state;
} assv_power_t;


typedef struct __attribute__((packed)){
	uint8_t last_status;
	uint32_t latitude;
	uint32_t longitude;
	uint32_t altitude;
	uint32_t velocity;
	uint32_t gps_time_week;
	uint32_t gps_time_sec;
} assv_gps_t; // 25 bytes packed


typedef struct __attribute__((packed)){
         int8_t temp[2];              //! Temperature sensors [TEMP1, TEMP2]
         uint32_t current[2];		   //! current sensing system [5V, 3V3]
	 uint16_t arduino_states; 	//! state of on/off arduinos
	 uint8_t sensor_states;		//! states of on/off sensors
	 assv_boot_t assv_boot;		//! boot info
	 assv_gps_t gps_info;		//! GPS location info
} assv_hk_t; // 6 + 25 + 13 = 43 bytes packed


void assv_set_node(uint8_t node);
int assv_hk_get(assv_hk_t* assv_hk);
int assv_hk_unpack(assv_hk_t* assv_hk);
int assv_hk_print(assv_hk_t* assv_hk);
int assv_hk_print_power(uint16_t arduino_states, uint8_t sensor_states);
int assv_hk_print_gps(assv_gps_t* gps_hk);
int assv_test(void);
int assv_toggle_led(void);
int assv_power_arduino(uint8_t node, uint8_t state);
int assv_power_sensor(uint8_t node, uint8_t state);
int assv_flash_arduino(uint8_t node, uint8_t* rpath);
int assv_log_serial(uint8_t node);


#endif
