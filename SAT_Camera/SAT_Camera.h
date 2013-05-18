/** @brief  Library to take photos using the ArduSat Camera.

    This is the library to use for taking photos from the ArduSat Camera. The camera is 1.3MP and take photos at a maximum resolution of 1280x1024.
	
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

#ifndef CAMERA_H
#define CAMERA_H

#include <inttypes.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

#define MAX_PATHLEN 20
#define SIZE 32

class SAT_Camera
{
    private:
        static uint8_t _local_address;         //local address of this node
        static OnboardCommLayer* _commLayer;
        static void _registerCam();
        static byte _send_buffer[];
        static char _picpath[];
        static void (*_user_function)(size_t);

        static void _onPictureDone(size_t, boolean);

    public:
		/**Constructor that has a single ID parameter. */
        SAT_Camera();
		
		/**Constructor that has a single ID parameter.
			@param compressionRatio - The compression ratio (JPEG) for the image.
			@param pictureSize - The number of vertical pixels. Horizontal is automatically computed. 
        */
        void takePicture(byte compressionRatio, unsigned int pictureSize, 
                float radians, float latitude,float longitude);
		
		/**Takes a photo with the default settings. */
        void takePictureSimple();
		
		/**Callback for when the photo has been taken. */
        void onPictureTaken(void(*)(size_t));
        void on(uint8_t);
};

extern SAT_Camera Camera;

#endif


