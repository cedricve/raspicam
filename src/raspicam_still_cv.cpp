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

#include "private_still/private_still_impl.h"
#include "raspicam_still_cv.h"
#include "scaler.h"
using namespace cv;
namespace raspicam {
    RaspiCam_Still_Cv::RaspiCam_Still_Cv() {
        _impl= new _private::Private_Impl_Still() ;
        _isOpened=false;
        image_buffer=0;
        _impl->setEncoding ( raspicam::RASPICAM_ENCODING_RGB );
        _isGrabbed=false;
	_impl->setVerticalFlip(true);
    }
    RaspiCam_Still_Cv::~RaspiCam_Still_Cv() {
        release();
        delete _impl;
    }


    bool RaspiCam_Still_Cv::open ( ) {
        _isOpened= _impl->initialize() ==0;
        return _isOpened;
    }

    bool RaspiCam_Still_Cv::isOpened ( ) const{
        return _isOpened;
    }
    bool RaspiCam_Still_Cv::grab () {
        if ( image_buffer==0 ) image_buffer=new uchar [ _impl-> getImageBufferSize()];
        _isGrabbed= _impl->takePicture ( image_buffer,  _impl-> getImageBufferSize() );
        return _isGrabbed;
    }
    void RaspiCam_Still_Cv::retrieve ( cv::Mat &image ) {
        if ( image_buffer!=0 && _isGrabbed ) {
            image.create ( _impl->getHeight(),_impl->getWidth(),CV_8UC3 );
            memcpy ( image.ptr<uchar> ( 0 ),image_buffer,image.cols*image.rows*3 );
        }
    }

    void RaspiCam_Still_Cv::release() {}




    /**Returns the specified VideoCapture property
     */

    double RaspiCam_Still_Cv::get ( int propId ) {

        switch ( propId ) {

        case CV_CAP_PROP_FRAME_WIDTH :
            return _impl->getWidth();
        case CV_CAP_PROP_FRAME_HEIGHT :
            return _impl->getHeight();
        case CV_CAP_PROP_FPS:
            return 30;
        case CV_CAP_PROP_FORMAT :
            return CV_8UC3;
        case CV_CAP_PROP_MODE :
            return 0;
        case CV_CAP_PROP_BRIGHTNESS :
            return _impl->getBrightness();
        case CV_CAP_PROP_CONTRAST :
            return Scaler::scale ( -100,100,0,100,  _impl->getContrast() );
        case CV_CAP_PROP_SATURATION :
            return  Scaler::scale ( -100,100,0,100, _impl->getSaturation() );;
//     case CV_CAP_PROP_HUE : return _cam_impl->getSharpness();
        case CV_CAP_PROP_GAIN :
            return  Scaler::scale ( 0,800,0,100, _impl->getISO() );
        case CV_CAP_PROP_EXPOSURE :
//             if ( _impl->getShutterSpeed() ==0 )
            return -1;//not yet
//             else return Scaler::scale (0,330000, 0,100, _impl->getShutterSpeed() )  ;
            break;
        case CV_CAP_PROP_CONVERT_RGB :
            return ( true );
//     case CV_CAP_PROP_WHITE_BALANCE :return _cam_impl->getAWB();
        default :
            return -1;
        };
    }

    /**Sets a property in the VideoCapture.
     */

    bool RaspiCam_Still_Cv::set ( int propId, double value ) {

        switch ( propId ) {

        case CV_CAP_PROP_FRAME_WIDTH :
            if ( value!=_impl->getWidth() ) {
                delete image_buffer;
                image_buffer=0;
                _impl->setWidth ( value );
            }
            break;
        case CV_CAP_PROP_FRAME_HEIGHT :
            if ( value!=_impl->getHeight() ) {
                delete image_buffer;
                image_buffer=0;
                _impl->setHeight ( value );
            }
            break;
        case CV_CAP_PROP_FORMAT : {
            return false;		//ONLY 8UC3 allowed at this moment
        }
        break;
        case CV_CAP_PROP_MODE ://nothing to  do yet
            return false;
            break;
        case CV_CAP_PROP_BRIGHTNESS :
            _impl->setBrightness ( value );
            break;
        case CV_CAP_PROP_CONTRAST :
            _impl->setContrast ( Scaler::scale ( 0,100,-100,100, value ) );
            break;
        case CV_CAP_PROP_SATURATION :
            _impl->setSaturation ( Scaler::scale ( 0,100,-100,100, value ) );
            break;
//     case CV_CAP_PROP_HUE : return _cam_impl->getSharpness();
        case CV_CAP_PROP_GAIN :
            _impl->setISO ( Scaler::scale ( 0,100,0,800, value ) );
            break;
        case CV_CAP_PROP_EXPOSURE :
//             if ( value>0 && value<=100 ) {
//                 _impl->setShutterSpeed ( Scaler::scale ( 0,100,0,330000, value ) );
//             } else {
//                 _impl->setExposure ( RASPICAM_EXPOSURE_AUTO );
//                 _impl->setShutterSpeed ( 0 );
//             }
            break;
        case CV_CAP_PROP_CONVERT_RGB :
//              CV_8UC3;
            break;
//     case CV_CAP_PROP_WHITE_BALANCE :return _cam_impl->getAWB();
        default :
            return false;
        };
        return true;

    }
    std::string RaspiCam_Still_Cv::getId() const{
        return _impl->getId();
    }
}


