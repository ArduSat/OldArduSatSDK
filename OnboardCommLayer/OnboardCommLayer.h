/**
	@brief  Library for Communication over i2c, slave-side.
    
	OnboardCommLayer.h/cpp - Communication over i2c, slave-side.  Takes messages
    and enqueues them for sending until master requests a message.  The message
    buffer is circular and old messages may get overwritten by new ones if
    they are being produced faster than they are consumed.

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
    
	@author Jorge Ortiz and NanoSatisfi, Inc.
    @date May 2013
  
*/


#ifndef ONBOARD_COMM_LAYER_H
#define ONBOARD_COMM_LAYER_H

#include <Arduino.h>
#include <I2C_add.h>
#include <nanosat_message.h>

#define SENDQ_SIZE 3
#define NODE_ADDR I2C_ADD_ARD1
#define IBUF_SIZE sizeof(nanosat_message_t)
#define NUM_SENSORS 18
#define MAX_I2C_REQUEST_SIZE 32

// define a function pointer by name
typedef void (*_default_func_ptr) (size_t, boolean);
typedef void (*_app_func_ptr) (int, byte*, size_t);

// two-wire protocol mode
typedef enum{
    MULTIMASTER,
    MASTER_SLAVE
}setting_t;

// action
typedef enum {
    ENQUEUE,
    DEQUEUE
} _action_t;

class OnboardCommLayer{
    private:
        //variables
        //static OnboardCommLayer* _ocl;
        static boolean _qlock;
        static uint8_t _sendq_head;
        static uint8_t _sendq_tail;
        static nanosat_message_t _send_queue[SENDQ_SIZE];
        static nanosat_message_t* _send_queue_ptrs[SENDQ_SIZE];
        static setting_t _i2c_setting;
        static uint8_t _local_address;

        //statistics
        static int _packets_tx;
        static int _packets_rx;
        static int _packets_dx; //packets dropped

        //internal buffers
        static nanosat_message_t* msg_t;
        static uint8_t _ibuf[IBUF_SIZE];

        //place holders
        static boolean _ocl_inited;
        static boolean _wire_setup;
        static uint8_t _request_addr[1];

        //user functions
        //static void (*_user_onReceive)(int, byte*, size_t len, byte);
        //static void (*_user_onReceiveAppMsg)(int,byte* buf, size_t len);

        //for locking -- Dekker's alg.
        static uint8_t _flag[2];
        static uint8_t _turn;

        //functions
        static void _onMasterRequest();
        static void _onReceive(int bytesReceived);
        //static void _onReceiveAppMsg(void(*)(uint8_t*,size_t),uint8_t);
        static uint8_t _handleIncomingData(uint8_t, boolean);

        //locking
        static boolean _get_lock(_action_t action);
        static void _release_lock(_action_t action);
        static _action_t _get_counter_action(_action_t action);

    public:


        /** Constructor
         * @param i2c_setup MULTIMASTER or MASTER_SLAVE
         * @param node_addr this node address
         */
        OnboardCommLayer(setting_t i2c_setup, uint8_t node_addr);

        //static OnboardCommLayer* getInstance(setting_t i2c_setup);
        /*
         * Send message to the supervisor.
         */
        void send(nanosat_message_t* msg);

        /*
         * Send message to the given address.
         * @param the i2c destination address (as defined in I2C_add.h);
         */
        void send(nanosat_message_t* msg, uint8_t dst_addr);

        /**
         * Send a raw data buffer of size len to the given i2c destination.
         *
         * @param msg pointer to the buffer
         * @param len the size of the buffer
         * @dst_addr the i2c destination address
         */
        void send(uint8_t* msg, size_t len,  uint8_t dst_addr);

        /**
         * Registers a function with two parameters, a pointer to the response
         * buffer, the size of the response buffer, and the i2c destination 
         * address.
         *
         * @param function the first parameter is a function that, when called,
         *          gets passed the amount of data that was written to the
         *          respponse buffer (size_t) and a boolean value (boolean) 
         *          that says when the transfer is finished.
         *          
         * @param byte* a pointer to the response buffer -- the buffer used to
         *          write the response from the destination.
         *
         * @param size_t the maximum size of that buffer.
         * 
         * @param uint8_t the i2c destination address as defined in I2C_add.h
         *      
         */
        void onReceive(void(*) (size_t, boolean), byte*, size_t, uint8_t);

        /**
         * Requests data from a given destination (slave).
         * 
         * @param uint8_t the destination address of the slave that you want to
         *          make the request to.
         *
         * @param int the number of bytes you are requesting from that
         *          destination.
         */
        void requestDataFrom(uint8_t, int);

        /**
         * Requests data from a given destination (slave).
         * 
         * @param uint8_t the destination address of the slave that you want to
         *          make the request to.
         *
         * @param int the number of bytes you are requesting from that
         *          destination.
         * @param byte* the buffer to write the data to
         * @param int the size of the byte buffer
         */
        size_t requestDataFrom(uint8_t, int, byte*, int);

};

#endif
