/** 
   @brief OnboardCommLayer.h/cpp - Communication over i2c, slave-side.  
    Takes messages
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


/******************************************************************************
 * Includes
 ******************************************************************************/
#include "OnboardCommLayer.h"
#include <Wire.h>

//declare static vars in memory, externalize for all files that include
//the header file -- gets rid of "multiple definition of" errors in linker
//extern OnboardCommLayer* OnboardCommLayer::_ocl=NULL;
extern boolean OnboardCommLayer::_qlock = false;
extern uint8_t OnboardCommLayer::_sendq_head = 0;
extern uint8_t OnboardCommLayer::_sendq_tail = 0; 
extern nanosat_message_t OnboardCommLayer::_send_queue[SENDQ_SIZE];
extern nanosat_message_t* OnboardCommLayer::_send_queue_ptrs[SENDQ_SIZE];
extern uint8_t OnboardCommLayer::_flag[2];
extern uint8_t OnboardCommLayer::_turn =0;
extern setting_t OnboardCommLayer::_i2c_setting = MULTIMASTER;
extern nanosat_message_t* OnboardCommLayer::msg_t = NULL;
extern uint8_t OnboardCommLayer::_ibuf[IBUF_SIZE];
extern uint8_t OnboardCommLayer::_local_address;

extern boolean OnboardCommLayer::_ocl_inited = false;
extern boolean OnboardCommLayer::_wire_setup= false;
extern uint8_t OnboardCommLayer::_request_addr[1];
/*extern void (*OnboardCommLayer::_user_onReceive) (int,size_t len, boolean isDone);
extern void (*OnboardCommLayer::_user_onReceiveAppMsg) (int, byte* buf, size_t len);*/

//statistics
extern int OnboardCommLayer::_packets_tx = 0;
extern int OnboardCommLayer::_packets_rx = 0;
extern int OnboardCommLayer::_packets_dx = 0;  //dropped packets


// array of sensor addresses
uint8_t _sensor_addresses[NUM_SENSORS];

//init vars
boolean _wire_setup2 = false;
boolean _ocl_inited2 = false;

//array of function pointers
//callback functions and application buffers
_default_func_ptr df_ptrs[NUM_SENSORS];
byte* _buffer_ptrs[NUM_SENSORS];
uint8_t _buffer_sizes[NUM_SENSORS];

//application function pointers
_app_func_ptr appf_ptrs[NUM_SENSORS];

//byte _setMaster = 0;
//uint8_t _request_addr = 0;

/******************************************************************************
 * Constructors -- Private (Singleton)
 ******************************************************************************/
OnboardCommLayer::OnboardCommLayer(setting_t i2c_setup, uint8_t node_addr){
    _local_address = node_addr;
    _request_addr[0] = 0;
    //Serial.println(_wire_setup2);
    //if(!_wire_setup2){
        Serial.println("setting up wire");
        Wire.begin();
        Wire.begin(_local_address);
    //    _wire_setup2 = true;
    /*} else {
        Serial.println("wire inited already");
    }*/

    //if(_ocl_inited2==false){
        Serial.println("ocl setup");
        memset(&_send_queue[0], 0, SENDQ_SIZE*sizeof(nanosat_message_t));
        _flag[0] = 0;
        _flag[1] = 0;
        //_user_onReceive = NULL;
        //_user_onReceiveAppMsg = NULL;
        for(int i=0; i<SENDQ_SIZE; i++)
            _send_queue_ptrs[i]=NULL;

        //init the sensor addresses
        _sensor_addresses[0]=(uint8_t)I2C_ADD_MAG;
        _sensor_addresses[1]=(uint8_t)I2C_ADD_LUX1;
        _sensor_addresses[2]=(uint8_t)I2C_ADD_LUX2; 
        _sensor_addresses[3]=(uint8_t)I2C_ADD_TMP1;
        _sensor_addresses[4]=(uint8_t)I2C_ADD_TMP2;
        _sensor_addresses[5]=(uint8_t)I2C_ADD_TMP3;
        _sensor_addresses[6]=(uint8_t)I2C_ADD_TMP4;
        _sensor_addresses[7]=(uint8_t)I2C_ADD_ACC;
        _sensor_addresses[8]=(uint8_t)I2C_ADD_GYR;
        _sensor_addresses[9]=(uint8_t)I2C_ADD_MLX;
        _sensor_addresses[10]=(uint8_t)I2C_ADD_SUPERVISOR;
        _sensor_addresses[11]=(uint8_t)I2C_ADD_SPECT;
        _sensor_addresses[12]=(uint8_t)I2C_ADD_GEIGER;
        _sensor_addresses[13]=(uint8_t)I2C_ADD_GPS;

        //"sensors" on supervisor
        _sensor_addresses[14]=(uint8_t)ADD_SUPERVISOR_SUN;
        _sensor_addresses[15]=(uint8_t)ADD_SUPERVISOR_CAM;
        _sensor_addresses[16]=(uint8_t)ADD_SUPERVISOR_CLOCK;
        _sensor_addresses[17]=(uint8_t)ADD_SUPERVISOR_TELEMETRY;

        //setup the rest of the code
        _i2c_setting = i2c_setup;
        switch(i2c_setup){
            case MASTER_SLAVE:
                Wire.onRequest(_onMasterRequest);
                break;
            case MULTIMASTER:
                Wire.onReceive(OnboardCommLayer::_onReceive);
                break;
        }
        _ocl_inited2=true;
    //} 

}

/******************************************************************************
 * User API
 ******************************************************************************/

/*
   Enqueues a message for sending upon request from the master.
*/
void OnboardCommLayer::send(nanosat_message_t* msg){
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    if(_i2c_setting == MASTER_SLAVE){
        _qlock = _get_lock(ENQUEUE);
        if(msg!=NULL){
            memcpy(&_send_queue[_sendq_tail], msg, sizeof(nanosat_message_t));
            _send_queue_ptrs[_sendq_tail] = &_send_queue[_sendq_tail];
            _sendq_tail= (_sendq_tail+1)%SENDQ_SIZE;
        }
        _release_lock(ENQUEUE);
        _qlock = false;
    } else { //MULTIMASTER
        //Serial.print("Sending::"); Serial.print(msg->prefix);
        msg->checksum = fletcher_encode((byte*)&msg->data, (long) sizeof(nanosat_data_t));
        _request_addr[0] = I2C_ADD_SUPERVISOR;
        //Serial.print("request_addr::set=");Serial.println(_request_addr[0]);
        Wire.beginTransmission(I2C_ADD_SUPERVISOR);
        Wire.write((const uint8_t*)msg, (size_t)sizeof(nanosat_message_t));
        Wire.endTransmission();
        //Serial.println("done");
    }
    _packets_tx+=1;
}

void OnboardCommLayer::send(nanosat_message_t* msg, uint8_t dst_addr){
    msg->prefix = NODE_COMM_MESSAGE_PREFIX;
    if(_i2c_setting == MASTER_SLAVE){
        _qlock = _get_lock(ENQUEUE);
        if(msg!=NULL){
            memcpy(&_send_queue[_sendq_tail], msg, sizeof(nanosat_message_t));
            _send_queue_ptrs[_sendq_tail] = &_send_queue[_sendq_tail];
            _sendq_tail= (_sendq_tail+1)%SENDQ_SIZE;
        }
        _release_lock(ENQUEUE);
        _qlock = false;
    } else { //MULTIMASTER
        msg->checksum = fletcher_encode((byte*)&msg->data, (long) sizeof(nanosat_data_t));

        _request_addr[0] = dst_addr;
        switch(dst_addr){
            case ADD_SUPERVISOR_SUN:
                dst_addr = I2C_ADD_SUPERVISOR;
                break;
            case ADD_SUPERVISOR_CAM:
                dst_addr = I2C_ADD_SUPERVISOR;
                break;
            case ADD_SUPERVISOR_CLOCK:
                dst_addr = I2C_ADD_SUPERVISOR;
                break;
        }
        Wire.beginTransmission(dst_addr);
        Wire.write((const uint8_t*)msg, 
                (size_t)sizeof(nanosat_message_t));
        Wire.endTransmission();
    }
    _packets_tx+=1;
}

void OnboardCommLayer::send(uint8_t* msg, size_t len,  uint8_t dst_addr){
    if(_i2c_setting == MULTIMASTER){
        _request_addr[0] = dst_addr;
        Wire.beginTransmission(dst_addr);
        Wire.write(msg, len);
        Wire.endTransmission();

        _packets_tx+=1;
    }
}

size_t OnboardCommLayer::requestDataFrom(uint8_t _dst_addr, int sz, byte _app_buf[], int buflen){
    int bytesRxd = 0;
    if(sz>buflen)
        sz = buflen;
    Wire.requestFrom((int)_dst_addr, (int)sz);
    while(Wire.available()){
        _app_buf[bytesRxd]=Wire.read();
        bytesRxd+=1;
    }
    return (size_t)bytesRxd;
}

void OnboardCommLayer::requestDataFrom(uint8_t _dst_addr, int sz){
    //Serial.println("OnboardCommLayer::requestDataFrom() called");
    int _remaining = sz;
    _request_addr[0]=_dst_addr;

    //stats
    if(_remaining>0)
        _packets_rx+=1;

    //make requests for data from _dst_addr slave
    boolean last_chunk =false;
    uint8_t offset = 0;
    while(_remaining>0){
        
        if(_remaining-MAX_I2C_REQUEST_SIZE<=0)
            last_chunk=true;
        if(_remaining<MAX_I2C_REQUEST_SIZE)
            Wire.requestFrom((int)_dst_addr, _remaining);
        else
            Wire.requestFrom((int)_dst_addr, MAX_I2C_REQUEST_SIZE);
        offset=_handleIncomingData(offset, last_chunk);
        _remaining-=MAX_I2C_REQUEST_SIZE;
    }
}

void OnboardCommLayer::onReceive(void (*function) (size_t, boolean), 
        byte* app_byte_buf, size_t buf_len, uint8_t sensor_addr){
    int i=0;
    int found = -1;
    while(i<NUM_SENSORS){
        if(sensor_addr == _sensor_addresses[i]){
            found = i;
            break;
        }
        i++;
    }
    if(found>=0){

        /*if(function!=NULL){
            Serial.print("REG2::");Serial.println(found);}
        else{
            Serial.print("REG2::");Serial.println("function_null");
        }*/
        df_ptrs[found] = function;
        if(app_byte_buf!=NULL){
            _buffer_ptrs[found] = app_byte_buf;
            _buffer_sizes[found]=buf_len;
        }
    } else {
        Serial.print("error");
    }
}

/*void OnboardCommLayer::onReceiveAppMsg(void (*function) (int, byte*, size_t, byte), uint8_t sensor_addr){
    //_user_onReceiveAppMsg = function;
    int i=0;
    while(i<NUM_SENSORS){
        if(sensor_addr == _sensor_addresses[i])
            break;
    }
    //_user_onReceive = function;
    if(i<NUM_SENSORS){
        df_ptrs[i] = function;
    }
}*/



/******************************************************************************
 * Private functions 
 ******************************************************************************/
/*
   Uses Dekker's algorithms to acquire a lock for the specified action.
*/
boolean OnboardCommLayer::_get_lock(_action_t action){
    _action_t counter_action = _get_counter_action(action);
    _flag[action] = 1;
    while(_flag[counter_action] == 1){
        if(_turn != action){
            _flag[action] = 0;
            while(_turn != action){
                //busy wait w/delay
                delay(20);
            }
            _flag[action]=1;
        }
    }
    return true;
}

/*
   Release the lock for the specified action.
*/
void OnboardCommLayer::_release_lock(_action_t action){
    _action_t counter_action = _get_counter_action(action);
    _turn = counter_action;
    _flag[action] = 0;
}

/*
   Returns the counter action for the specified action.
*/
_action_t OnboardCommLayer::_get_counter_action(_action_t action){
    _action_t counter_action;
    if(action == ENQUEUE)
        counter_action = DEQUEUE;
    else
        counter_action = ENQUEUE;
    return counter_action;
}

/*
   Dequeues a message and send it to the master.
*/
void OnboardCommLayer::_onMasterRequest(){
    _qlock = _get_lock(DEQUEUE);
    nanosat_message_t* msg_ = _send_queue_ptrs[_sendq_head];
    if(msg_!=NULL){
        msg_->checksum = fletcher_encode((byte*)&msg_->data, (long) sizeof(nanosat_data_t));
        Wire.write((const uint8_t*)msg_, (size_t)sizeof(nanosat_message_t));
        memset(&_send_queue[_sendq_head], 0, sizeof(nanosat_message_t));
        _send_queue_ptrs[_sendq_head]=NULL;
        _sendq_head = (_sendq_head+1)%SENDQ_SIZE;
    }
    _release_lock(DEQUEUE);
}

uint8_t OnboardCommLayer::_handleIncomingData(uint8_t offset, 
        boolean last_chunk){
    int i=0;
    int found = -1;
    //Serial.print("h_request_addr=");Serial.println(_request_addr[0]);
    while(i<NUM_SENSORS){
        if(_request_addr[0] == _sensor_addresses[i]){
            found = i;
            break;
        }
        i++;
    }
    //_user_onReceive = function;
    byte last_byte=0;
    uint8_t next_idx=0;
    if(found>0){
        //Serial.print("nodump::");Serial.println(found);
        _default_func_ptr f = df_ptrs[found];
        byte* _appBuffer = NULL;
        uint8_t _max_size = 0;
        if(f!=NULL){
            _appBuffer = _buffer_ptrs[found];
            _max_size = _buffer_sizes[found];

            int bytesRead = 0;
            while(Wire.available()){
                byte b = Wire.read();
                bytesRead +=1;
                //Serial.println(b);
                if(_appBuffer!=NULL){
                    next_idx = offset + bytesRead - 1;
                    if(next_idx<_max_size){
                        _appBuffer[next_idx]=b;
                    } else {
                        //call the registered chunk handler
                        f(bytesRead, false);

                        //reset the bytesRead counter
                        bytesRead=1;
                        offset=0;
                        next_idx = offset + bytesRead - 1; //0

                        //clear the buffer
                        memset(_appBuffer, 0, _max_size);

                        //write it to the buffer
                        _appBuffer[next_idx]=b;
                    }
                }
            }

            //finally, call the function, signaling done
            //and clear the buffer
            if(last_chunk)
                f(bytesRead,true);
        } else {
            _packets_dx+=1;
            Serial.println("null_function");
            //drop the packet
            while(Wire.available())
                Wire.read();
        }
        return next_idx;
    } else {
        _packets_dx+=1;
        Serial.println("dump");
        //drop the packet
        while(Wire.available())
            Wire.read();
    }
}


/*
 Dispatches raw message reception
*/
void OnboardCommLayer::_onReceive(int bytesReceived){
    _packets_rx +=1;
    //Serial.println("b");
    /*while(Wire.available())
        Wire.read();*/
    _handleIncomingData(0, true);

}

/*
 Dispatches Application data reception.
*/
/*void OnboardCommLayer::_onReceiveAppMsg(int bytesReceived){
    //receive a message from another node, sensor, or supervisor
    int idx=0;
    if(_user_onReceiveAppMsg!=NULL){
        //_user_onReceiveMsg(msg.data);
        while(Wire.available()>0){
            _ibuf[idx] = (uint8_t)Wire.read();
            idx+=1;
            if(idx == sizeof(nanosat_message_t))
                break;
        }
        nanosat_message_t* rmsg = (nanosat_message_t*)&_ibuf[0];
        nano_datamsg_t* appmsg = (nano_datamsg_t*)&rmsg->data;
        _user_onReceiveAppMsg(appmsg->buf, appmsg->len);
    }
}*/

