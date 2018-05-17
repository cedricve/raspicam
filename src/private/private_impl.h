/**********************************************************
 Software developed by AVA ( Ava Group of the University of Cordoba, ava  at uco dot es)
 Main author Rafael Munoz Salinas (rmsalinas at uco dot es)
 This software is released under BSD license as expressed below
-------------------------------------------------------------------
Copyright (c) 2013, AVA ( Ava Group University of Cordoba, ava  at uco dot es)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:

   This product includes software developed by the Ava group of the University of Cordoba.

4. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY AVA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL AVA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************/

#ifndef _Private_RaspiCam_IMPL_H
#define _Private_RaspiCam_IMPL_H
#include "mmal/mmal.h"
//#include "mmal_connection.h"
#include <mutex>
#include <string>
#include "raspicamtypes.h"
#include "private_types.h"
#include "threadcondition.h"
namespace raspicam {
    namespace _private
    {

        /**Base class that do all the hard work
         */
        class Private_Impl
        {
            /** Struct used to pass information in encoder port userdata to callback
            */
            struct PORT_USERDATA
            {
                PORT_USERDATA() {
                    wantToGrab=false;
                    pstate=0;
                }
                void waitForFrame() {
                    //_mutex.lock();
                    std::unique_lock<std::mutex> lck ( _mutex );

                    wantToGrab=true;
//                    _mutex.unlock();
//                    Thcond.Wait();
                       Thcond.Wait(lck); //this will unlock the mutex and wait atomically
                };



                RASPIVID_STATE *pstate;            /// pointer to our state in case required in callback
                Private_Impl *inst;
                std::mutex _mutex;
                ThreadCondition Thcond;
                bool wantToGrab;
                membuf<unsigned char> _buffData;

                /* User define callback interface */
                void (*_userCallback)(void*) = 0;
                void* _userCallbackData;

            };

            public:

            /**Constructor
             */
            Private_Impl();
            /**Destructor
             */
            ~Private_Impl();
            /**Opens the camera and start capturing
            */
            bool open ( bool StartCapture=true );
            /**indicates if camera is open
            */
            bool isOpened() const
            {
                return _isOpened;
            }
            /**Starts camera capture
             */
            bool startCapture();
            /**Indicates if is capturing
             */
            bool isCapturing() const{return _isCapturing;}

            /*
             * the function 'userCallback' will be called every time new data arrived from camera,
             * with 'data' as argument.
             */
            void setUserCallback(void (*userCallback)(void*) , void* data = 0);

            /**Grabs the next frame and keeps it in internal buffer. Blocks until next frame arrives
            */
            bool grab();
            /**Retrieves the buffer previously grabbed.
            * NOTE: Change in version 0.0.5. Format is stablished in setFormat function
            * So type param is ignored. Do not use this parameter.
            * You can use getFormat to know the current format
             */
            void retrieve ( unsigned char *data,RASPICAM_FORMAT type=RASPICAM_FORMAT_IGNORE );
            /**Alternative to retrieve. Returns a pointer to the original image data buffer.
              * Be careful, if you call grab(), this will be rewritten with the new data
             */
            unsigned char *getImageBufferData() const;
            /**
             * Returns the size of the buffer returned in getImagePtr. If is like calling getImageTypeSize(getFormat()). Just for dummies :P
             */
            size_t getImageBufferSize() const;

            /** Stops camera and free resources
            */
            void release();

            //sets capture format. Can not be changed once camera is opened
            void setFormat ( RASPICAM_FORMAT fmt );
            //sets sensor mode. Can not be changed once camera is opened
            void setSensorMode ( int mode );

            //sets width. Can not be changed once camera is opened
            void setWidth ( unsigned int width ) ;
            //sets height. Can not be changed once camera is opened
            void setHeight ( unsigned int height );
            //sets capture size. Can not be changed once camera is opened
            void setCaptureSize ( unsigned int width, unsigned int height );
            void setBrightness ( unsigned int brightness );
            void setRotation ( int rotation );
            void setISO ( int iso );
            void setSharpness ( int sharpness );
            void setContrast ( int contrast );
            void setSaturation ( int saturation );
            void setExposure ( RASPICAM_EXPOSURE exposure );
            void setVideoStabilization ( bool v );
            void setExposureCompensation ( int val ); //-10,10
            void setAWB ( RASPICAM_AWB awb );
            void setAWB_RB ( float red,float blue );//ranges [0,1]

            void setImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect );
            void setMetering ( RASPICAM_METERING metering );
            void setHorizontalFlip ( bool hFlip );
            void setVerticalFlip ( bool vFlip );
            /**
              *Set the shutter speed to the specified value (in microseconds).
              *There is currently an upper limit of approximately 330000us (330ms, 0.33s) past which operation is undefined.
              */
            void setShutterSpeed ( unsigned int shutter ); //currently not  supported
            //sets frame rate. Can not be changed once camera is opened
            void setFrameRate ( int fps );

            RASPICAM_FORMAT  getFormat() const {return State.captureFtm;}
            //Accessors
            unsigned int getSensorMode() const
            {
                return State.sensor_mode;
            }
            unsigned int getWidth() const
            {
                return State.width;
            }
            unsigned int getHeight() const
            {
                return State.height;
            }
            unsigned int getBrightness() const
            {
                return State.brightness;
            }
            unsigned int getRotation() const
            {
                return State.rotation;
            }
            int getISO() const
            {
                return State.ISO;
            }
            int getSharpness() const
            {
                return State.sharpness;
            }
            int getContrast() const
            {
                return State.contrast;
            }
            int getSaturation() const
            {
                return State.saturation;
            }
            unsigned int getShutterSpeed() const
            {
                return State.shutterSpeed;
            }
            RASPICAM_EXPOSURE getExposure() const
            {
                return State.rpc_exposureMode;
            }
            RASPICAM_AWB getAWB() const
            {
                return State.rpc_awbMode;
            }

            float getAWBG_red(){return State.awbg_red;}

            float getAWBG_blue(){return State.awbg_blue;}

            int getFrameRate() const
            {
                return State.framerate;
            }

            RASPICAM_IMAGE_EFFECT getImageEffect() const
            {
                return State.rpc_imageEffect;
            }
            RASPICAM_METERING getMetering() const
            {
                return State.rpc_exposureMeterMode;
            }
            bool isHorizontallyFlipped() const
            {
                return State.hflip;
            }
            bool isVerticallyFlipped() const
            {
                return State.vflip;
            }


            //Returns an id of the camera. We assume the camera id is the one of the raspberry
            //the id is obtained using raspberry serial number obtained in /proc/cpuinfo
            std::string getId() const;

            /**Returns the size of the required buffer for the different image types in retrieve
             */
            size_t getImageTypeSize ( RASPICAM_FORMAT type ) const;

            private:
            static void video_buffer_callback ( MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer );
            static void camera_control_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
            void setDefaultStateParams();
            MMAL_COMPONENT_T *create_camera_component ( RASPIVID_STATE *state );
            void destroy_camera_component ( RASPIVID_STATE *state );


            //Commit
            void commitParameters( );
            void commitBrightness();
            void commitRotation() ;
            void commitISO() ;
            void commitSharpness();
            void commitContrast();
            void commitSaturation();
            void commitExposure();
            void commitAWB();
            void commitImageEffect();
            void commitMetering();
            void commitFlips();
            void commitExposureCompensation();
            void commitVideoStabilization();
            void commitShutterSpeed();
            void commitAWB_RB();

            MMAL_PARAM_EXPOSUREMODE_T convertExposure ( RASPICAM_EXPOSURE exposure ) ;
            MMAL_PARAM_AWBMODE_T  convertAWB ( RASPICAM_AWB awb ) ;
            MMAL_PARAM_IMAGEFX_T convertImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect ) ;
            MMAL_PARAM_EXPOSUREMETERINGMODE_T convertMetering ( RASPICAM_METERING metering ) ;
            int convertFormat ( RASPICAM_FORMAT fmt ) ;


            //Color conversion
	    void convertBGR2RGB(unsigned char *  in_bgr,unsigned char *  out_rgb,int size);
            float VIDEO_FRAME_RATE_NUM;
            RASPIVID_STATE State;
            MMAL_STATUS_T status;
            MMAL_PORT_T *camera_video_port;//,*camera_still_port
            PORT_USERDATA callback_data;
            bool _isOpened;
            bool _isCapturing;

            bool _rgb_bgr_fixed;


        };
    }
}

#endif
