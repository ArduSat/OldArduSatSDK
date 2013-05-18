////////////////////////////////////
//copy these in Rx/Tx test sketches
////////////////////////////////////
enum test_message_types{
    SET_REQUEST_SIZE,      //sets the request size
    GET_REQUEST_SIZE,      //gets the request size previously set (or default);
    SET_SLAVE_ADDR,        //sets the slave addr of this receiver
    GET_SLAVE_ADDR         //get the slave addr of this receiver
};


typedef struct{
    uint8_t type;        
    uint8_t plen;        //the size of the payload
    uint8_t* payload;    //payload
} test_message_t;

////////////////////////////////////
