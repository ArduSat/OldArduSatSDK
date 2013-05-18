/** 
   @brief SAT_ExperimentData.h/cpp - wraps the experimental data in an object.

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
#include "SAT_ExperimentData.h"


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_ExperimentData::SAT_ExperimentData(uint32_t exp_id, uint32_t row_id, uint32_t col_id,
                                boolean fragment, uint32_t frag_idx, uint32_t frag_max,
                                payload_type_t ptype, uint8_t* value, size_t len)
{
    data_msg.exp_id = exp_id;
    data_msg.row_id = row_id;
    data_msg.col_id = col_id;
    data_msg.ptype = ptype;
    size_t wcnt = 0;

    data_msg.frag = fragment;
    data_msg.frag_idx = frag_idx;
    data_msg.frag_max = frag_max;;
    
    if(DATA_SIZE<len){
        wcnt = DATA_SIZE;
        /*data_msg.frag = true;
        data_msg.frag_idx = 0;
        data_msg.frag_max = (uint32_t)ceil((double)len/(double)DATA_SIZE);*/
    } else{
        wcnt = len;
    }
}

SAT_ExperimentData::SAT_ExperimentData(){}

/******************************************************************************
 * Experiment data API interface
 ******************************************************************************/
exp_data_t* SAT_ExperimentData::get_exp_data(){
    return &data_msg;
}

void SAT_ExperimentData::set_exp_id(uint32_t exp_id){
    data_msg.exp_id = exp_id;
}

uint32_t SAT_ExperimentData::get_exp_id(){
    return data_msg.exp_id;
}

void SAT_ExperimentData::set_row_id(uint32_t row_id){
    data_msg.row_id = row_id;
}

uint32_t SAT_ExperimentData::get_row_id(){
    return data_msg.row_id;
}

void SAT_ExperimentData::set_col_id(uint32_t col_id){
    data_msg.col_id = col_id;
}

uint32_t SAT_ExperimentData::get_col_id(){
    return data_msg.col_id;
}

