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

#include "raspicam_cv.h"
#include "private/private_impl.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "scaler.h"
namespace raspicam {
    RaspiCam_Cv::RaspiCam_Cv() {
        _impl=new _private::Private_Impl();
	set(cv::CAP_PROP_FORMAT,CV_8UC3);
 
    }
    RaspiCam_Cv::~RaspiCam_Cv() {
        delete _impl;
    }

    /** Open  capturing device for video capturing
     */
    bool RaspiCam_Cv::open ( void ) {
        return _impl->open();
    }
    /**
     * Returns true if video capturing has been initialized already.
     */
    bool RaspiCam_Cv::isOpened() const {return _impl->isOpened();}
    /**
    *Closes video file or capturing device.
    */
    void RaspiCam_Cv::release() {
        _impl->release();
    }

    /**
     * Grabs the next frame from video file or capturing device.
     */
    bool RaspiCam_Cv::grab() {
        return _impl->grab();
    }

    void RaspiCam_Cv::setUserCallback(void (*userCallback)(void*) , void* data) {
        _impl->setUserCallback(userCallback, data);
    }

    /**
    *Decodes and returns the grabbed video frame.
     */
    void RaspiCam_Cv::retrieve ( cv::Mat& image ) {
        //here we go!
        image.create ( _impl->getHeight(),_impl->getWidth(),imgFormat );
        _impl->retrieve ( image.ptr<uchar> ( 0 ));
    }

    /**Returns the specified VideoCapture property
     */

    double RaspiCam_Cv::get ( int propId ) {

        switch ( propId ) {
        case cv::CAP_PROP_MODE:
            return _impl->getSensorMode();
        case cv::CAP_PROP_FRAME_WIDTH :
            return _impl->getWidth();
        case cv::CAP_PROP_FRAME_HEIGHT :
            return _impl->getHeight();
        case cv::CAP_PROP_FPS:
            return _impl->getFrameRate();
        case cv::CAP_PROP_FORMAT :
            return imgFormat;
        case cv::CAP_PROP_BRIGHTNESS :
            return _impl->getBrightness();
        case cv::CAP_PROP_CONTRAST :
            return Scaler::scale ( -100,100,0,100,  _impl->getContrast() ); 
        case cv::CAP_PROP_SATURATION :
            return  Scaler::scale ( -100,100,0,100, _impl->getSaturation() );;
//     case cv::CAP_PROP_HUE : return _cam_impl->getSharpness();
        case cv::CAP_PROP_GAIN :
            return  Scaler::scale ( 0,800,0,100, _impl->getISO() );
        case cv::CAP_PROP_EXPOSURE :
            if ( _impl->getShutterSpeed() ==0 )
                return -1;//auto
            else return Scaler::scale (0,330000, 0,100, _impl->getShutterSpeed() )  ;
	   break;
        default :
            return -1;
        };
    }

    /**Sets a property in the VideoCapture.
     */

    bool RaspiCam_Cv::set ( int propId, double value ) {

        switch ( propId ) {
        case cv::CAP_PROP_MODE:
            _impl->setSensorMode(value);
            break;
            
        case cv::CAP_PROP_FRAME_WIDTH :
            _impl->setWidth ( value );
            break;
        case cv::CAP_PROP_FRAME_HEIGHT :
            _impl->setHeight ( value );
            break;
        case cv::CAP_PROP_FORMAT :{
            bool res=true;
            if ( value==CV_8UC1  ){
                _impl->setFormat(RASPICAM_FORMAT_GRAY);
                imgFormat=value;
            }
            else if (value==CV_8UC3){
                _impl->setFormat(RASPICAM_FORMAT_RGB);
                imgFormat=value;
            }
            else res=false;//error int format
            return res;
        }break;
        case cv::CAP_PROP_BRIGHTNESS :
            _impl->setBrightness ( value );
            break;
        case cv::CAP_PROP_CONTRAST : 
            _impl->setContrast ( Scaler::scale ( 0,100,-100,100, value ) );
            break;
        case cv::CAP_PROP_SATURATION :
            _impl->setSaturation ( Scaler::scale ( 0,100,-100,100, value ) );
            break;
//     case cv::CAP_PROP_HUE : return _cam_impl->getSharpness();
        case cv::CAP_PROP_GAIN :
            _impl->setISO ( Scaler::scale ( 0,100,0,800, value ) );
            break;
        case cv::CAP_PROP_EXPOSURE :
            if ( value>0 && value<=100 ) { 
                _impl->setShutterSpeed ( Scaler::scale ( 0,100,0,330000, value ) );
            } else {
                _impl->setExposure ( RASPICAM_EXPOSURE_AUTO );
                _impl->setShutterSpeed ( 0 );
            }
            break;
        case cv::CAP_PROP_CONVERT_RGB :
            imgFormat=CV_8UC3;
            break;
        case cv::CAP_PROP_FPS:
            _impl->setFrameRate ( value );
        default :
            return false;
        };
        return true;
    }
	
	void RaspiCam_Cv::setRotation ( int nRotation ) {
		// input is [0;3], map to [0; 360]
		int clamped = std::max(0, std::min(nRotation, 3));
		int degrees = clamped * 90;
		_impl->setRotation ( degrees );
	}
	
	void RaspiCam_Cv::setImageEffect ( int nEffect ) {
		// Clamp to enum
		int clamped = std::max((int)RASPICAM_IMAGE_EFFECT::RASPICAM_IMAGE_EFFECT_NONE, std::min(nEffect, (int)RASPICAM_IMAGE_EFFECT::RASPICAM_IMAGE_EFFECT_CARTOON));
		_impl->setImageEffect ( (RASPICAM_IMAGE_EFFECT)clamped );
	}
	
	void RaspiCam_Cv::setVideoStabilization ( bool enable ) {
		_impl->setVideoStabilization ( enable );
	}
	
	void RaspiCam_Cv::setHorizontalFlip ( bool enable ) {
		_impl->setHorizontalFlip ( enable );
	}
	
	void RaspiCam_Cv::setVerticalFlip ( bool enable ) {
		_impl->setVerticalFlip ( enable );
	}
	
	void RaspiCam_Cv::setExposureCompensation ( int value ) {
		// clamp to [-10; 10]
		int clamped = std::max(-10, std::min(value, 10));
		_impl->setExposureCompensation ( clamped );
	}
	
	void RaspiCam_Cv::setAWB ( int nEnumValue ) {
		// Clamp to enum
		int clamped = std::max((int)RASPICAM_AWB::RASPICAM_AWB_OFF, std::min(nEnumValue, (int)RASPICAM_AWB::RASPICAM_AWB_HORIZON));
		_impl->setAWB ( (RASPICAM_AWB)clamped );
	}
	
	void RaspiCam_Cv::setMetering ( int nEnumValue ) {
		// Clamp to enum
		int clamped = std::max((int)RASPICAM_METERING::RASPICAM_METERING_AVERAGE, std::min(nEnumValue, (int)RASPICAM_METERING::RASPICAM_METERING_MATRIX));
		_impl->setMetering ( (RASPICAM_METERING)clamped );
	}
	
    std::string RaspiCam_Cv::getId() const{
        return _impl->getId();
    }

}


