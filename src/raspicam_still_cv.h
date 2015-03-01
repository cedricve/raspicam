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
#ifndef _RaspiCam_STILL_H
#define _RaspiCam_STILL_H
#include <opencv2/highgui/highgui.hpp>
#include "raspicamtypes.h"
#include <cstdio>
#include <opencv2/core/core.hpp>
namespace raspicam {

    namespace _private{
        class Private_Impl_Still;
    };


    /**Raspicam API for still camera
     */
    class RaspiCam_Still_Cv{
        //the implementation of the camera
        _private::Private_Impl_Still *_impl;

        public:
        /**Constructor
         */
        RaspiCam_Still_Cv();
        /**Destructor
         */
        ~RaspiCam_Still_Cv();
        /** Open  capturing device for video capturing
         */
        bool open ( void );
        /**
         * Returns true if video capturing has been initialized already.
         */
        bool isOpened() const;
        /**
        *Closes video file or capturing device.
        */
        void release();

        /**
         * Grabs the next frame from video file or capturing device.
         */
        bool grab();

        /**
        *Decodes and returns the grabbed video frame.
         */
        void retrieve ( cv::Mat& image );

        /**Returns the specified VideoCapture property
         */

        double get ( int propId );

        /**Sets a property in the VideoCapture.
        *
         *
         * Implemented properties:
         * CV_CAP_PROP_FRAME_WIDTH,CV_CAP_PROP_FRAME_HEIGHT,
         * CV_CAP_PROP_FORMAT: CV_8UC1 or CV_8UC3
         * CV_CAP_PROP_BRIGHTNESS: [0,100]
         * CV_CAP_PROP_CONTRAST: [0,100]
         * CV_CAP_PROP_SATURATION: [0,100]
         * CV_CAP_PROP_GAIN: (iso): [0,100]
         * CV_CAP_PROP_EXPOSURE: -1 auto. [1,100] shutter speed from 0 to 33ms
         *
               */

        bool set ( int propId, double value );

        /** Returns the camera identifier. We assume the camera id is the one of the raspberry obtained using raspberry serial number obtained in /proc/cpuinfo
         */
        std::string getId() const;

        private:
        uchar *image_buffer;
        bool _isOpened;
	bool _isGrabbed;

    };
}
#endif

