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

#ifndef _RaspiCam_H_
#define _RaspiCam_H_
#include <cstdio>
#include <string>
#include "raspicamtypes.h"
namespace raspicam {

    namespace _private{
        class Private_Impl;
    }
    /**Base class that do all the hard work
     */
    class RaspiCam
    {
        public:
        /**Constructor
         */
        RaspiCam();
        /**Destructor
         */
        ~RaspiCam();
        /**Opens the camera
        * @param StartCapture determines if camera must start capture or not.
              */
        bool open ( bool StartCapture=true );
        /**Makes camera start capturing
         */
        bool startCapture();

        /*
         * the function 'userCallback' will be called every time new data arrived from camera,
         * with 'data' as argument.
         */
        void setUserCallback(void (*userCallback)(void*) , void* data=0);

        /**indicates if camera is open
        */
        bool isOpened() const ;
        /**Grabs the next frame and keeps it in internal buffer. Blocks until next frame arrives
        */
        bool grab();
        /**Retrieves the buffer previously grabbed.
         * You can decide how image is given by setting type. The input buffer provided must have the appropriate
         * size accordingly. You can use getImageTypeSize() to determine the size
            * NOTE: Change in version 0.0.5. Format is stablished in setFormat function
            * So type param is ignored. Do not use this parameter.
            * You can use getFormat() to know the current format
          */
        void retrieve ( unsigned char *data,RASPICAM_FORMAT type=RASPICAM_FORMAT_IGNORE );
        /**Alternative to retrieve. Returns a pointer to the original image data buffer (which is in getFormat() format).
          *
          * Be careful, if you call grab(), this will be rewritten with the new data
          */
        unsigned char *getImageBufferData() const;
        /**
         * Returns the size of the images captured.
         */
        size_t getImageBufferSize() const;


        /** Stops camera and free resources
        */
        void release();

        /**Sets capture format
         */
        void setFormat ( RASPICAM_FORMAT fmt );
        /**Sets camera width. Use a multiple of 320 (640, 1280)
         */
        void setWidth ( unsigned int width ) ;
        /**Sets camera Height. Use a multiple of 240 (480, 960)
         */
        void setHeight ( unsigned int height );
        void setCaptureSize ( unsigned int width, unsigned int height );
        /** Set image brightness [0,100]
         */
        void setBrightness ( unsigned int brightness );
        /**
         * Set image sharpness (-100 to 100)
         */
        void setSharpness ( int sharpness );
        /**
         *  Set image contrast (-100 to 100)
         */
        void setContrast ( int contrast );
        /**
         * Set capture ISO (100 to 800)
         */
        void setISO ( int iso );
        /**
         * Set image saturation (-100 to 100)
         */
        void setSaturation ( int saturation );
        /**Sets on/off video stabilisation
         */
        void setVideoStabilization ( bool v );
        /**
         *  Set EV compensation (-10,10)
         */
        void setExposureCompensation ( int val ); //-10,10
        void setRotation ( int rotation );
        void setExposure ( RASPICAM_EXPOSURE exposure );
        void setShutterSpeed ( unsigned int ss );
        void setAWB ( RASPICAM_AWB awb );
        // et specific values for whitebalance. Requires to set seAWB in OFF mode first
        void setAWB_RB ( float r,float b );//range is 0-1.
        void setImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect );
        void setMetering ( RASPICAM_METERING metering );
        void setHorizontalFlip ( bool hFlip );
        void setVerticalFlip ( bool vFlip );
        void setFrameRate ( int frames_per_second );
        void setSensorMode ( int mode );

        //Accessors
        RASPICAM_FORMAT getFormat() const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        unsigned int getBrightness() const;
        unsigned int getRotation() const;
        int getISO() const;
        int getSharpness() const;
        int getContrast() const;
        int getSaturation() const;
        unsigned int getShutterSpeed() const;
        RASPICAM_EXPOSURE getExposure() const ;
        RASPICAM_AWB getAWB() const;
        float getAWBG_red()const;
        float getAWBG_blue()const;
        RASPICAM_IMAGE_EFFECT getImageEffect() const ;
        RASPICAM_METERING getMetering() const;
        int getFrameRate() const;
        bool isHorizontallyFlipped() const ;
        bool isVerticallyFlipped() const ;


        /** Returns an id of the camera. We assume the camera id is the one of the raspberry
        *the id is obtained using raspberry serial number obtained in /proc/cpuinfo
        */
        std::string getId() const;



        /**Returns the size of the required buffer for the different image types in retrieve
         */
        size_t getImageTypeSize ( RASPICAM_FORMAT type ) const;
        private:
        _private::Private_Impl *_impl;
    };
}
#endif
