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

#ifndef _RaspiCam_private_types_H
#define _RaspiCam_private_types_H

namespace raspicam {
    namespace _private{


        /// struct contain camera settings
        struct MMAL_PARAM_COLOURFX_T
        {
            int enable,u,v;       /// Turn colourFX on or off, U and V to use
        } ;
        struct PARAM_FLOAT_RECT_T
        {
            double x,y,w,h;
        } ;


        /** Structure containing all state information for the current run
         */
        struct RASPIVID_STATE
        {
            int sensor_mode;                          /// Requested width of image
            int width;                          /// Requested width of image
            int height;                         /// requested height of image
            int framerate;                      /// Requested frame rate (fps)
            /// the camera output or the encoder output (with compression artifacts)
            MMAL_COMPONENT_T *camera_component;    /// Pointer to the camera component
            MMAL_POOL_T *video_pool; /// Pointer to the pool of buffers used by encoder output port
            //camera params
            int sharpness;             /// -100 to 100
            int contrast;              /// -100 to 100
            int brightness;            ///  0 to 100
            int saturation;            ///  -100 to 100
            int ISO;                   ///  TODO : what range?
            bool videoStabilisation;    /// 0 or 1 (false or true)
            int exposureCompensation;  /// -10 to +10 ?
            int shutterSpeed;
	    RASPICAM_FORMAT captureFtm;
            RASPICAM_EXPOSURE rpc_exposureMode;
            RASPICAM_METERING rpc_exposureMeterMode;
            RASPICAM_AWB rpc_awbMode;
            RASPICAM_IMAGE_EFFECT rpc_imageEffect;
            MMAL_PARAMETER_IMAGEFX_PARAMETERS_T imageEffectsParameters;
            MMAL_PARAM_COLOURFX_T colourEffects;
            int rotation;              /// 0-359
            int hflip;                 /// 0 or 1
            int vflip;                 /// 0 or 1
            PARAM_FLOAT_RECT_T  roi;   /// region of interest to use on the sensor. Normalised [0,1] values in the rect
            float awbg_red;//white balance red and blue
            float awbg_blue;
        } ;

        //clean buffer
        template<typename T>
        class membuf{
            public:
            membuf() {
                data=0;
                size=0;
            }
            ~membuf() {
                if ( data!=0 ) delete []data;
            }
            void resize ( size_t s ) {
                if ( s!=size ) {
                    delete data;
                    size=s;
                    data=new  T[size];
                }
            }
            T *data;
            size_t size;
        };

    }
}

#endif

