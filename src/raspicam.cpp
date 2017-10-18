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

#include "raspicam.h"
#include "private/private_impl.h"
namespace raspicam {

    RaspiCam::RaspiCam() {
        _impl=new _private::Private_Impl;
    }
    RaspiCam::~RaspiCam() {
        delete _impl;
    }


    bool RaspiCam::open ( bool StartCapture ) {
        return _impl->open ( StartCapture );
    }
    bool RaspiCam::startCapture() {
        return _impl->startCapture();
    }

    void RaspiCam::setUserCallback(void (*userCallback)(void*) , void* data) {
        _impl->setUserCallback(userCallback, data);
    }

    bool RaspiCam::isOpened() const {return _impl->isOpened();}

    bool RaspiCam::grab() {
        return _impl->grab();
    }

    void RaspiCam::retrieve ( unsigned char *data,RASPICAM_FORMAT type ) {
        _impl->retrieve ( data,type );
    }
    unsigned char *RaspiCam::getImageBufferData() const{return _impl->getImageBufferData();}
    size_t RaspiCam::getImageBufferSize() const{return _impl->getImageBufferSize();}

    size_t RaspiCam::getImageTypeSize ( RASPICAM_FORMAT type ) const{return _impl->getImageTypeSize ( type );}

    void RaspiCam::release() {
        _impl->release();
    }

    void RaspiCam::setFormat(RASPICAM_FORMAT fmt){
         _impl->setFormat( fmt);
    }
    void RaspiCam::setWidth ( unsigned int width ) {
        _impl->setWidth ( width );
    }
    void RaspiCam::setHeight ( unsigned int height ) {
        _impl->setHeight ( height );
    }
    void RaspiCam::setCaptureSize ( unsigned int width, unsigned int height ) {
        _impl->setCaptureSize ( width,height );
    }
    void RaspiCam::setBrightness ( unsigned int brightness ) {
        _impl->setBrightness ( brightness );
    }
    void RaspiCam::setRotation ( int rotation ) {
        _impl->setRotation ( rotation );
    }
    void RaspiCam::setISO ( int iso ) {
        _impl->setISO ( iso );
    }
    void RaspiCam::setSharpness ( int sharpness ) {
        _impl->setSharpness ( sharpness );
    }
    void RaspiCam::setContrast ( int contrast ) {
        _impl->setContrast ( contrast );
    }
    void RaspiCam::setSaturation ( int saturation ) {
        _impl->setSaturation ( saturation );
    }
    void RaspiCam::setExposure ( RASPICAM_EXPOSURE exposure ) {
        _impl->setExposure ( exposure );
    }
    void RaspiCam::setShutterSpeed ( unsigned int ss ) {
          _impl->setShutterSpeed ( ss );
    }

    void RaspiCam::setVideoStabilization ( bool v ) {
        _impl->setVideoStabilization ( v );
    }
    void RaspiCam::setExposureCompensation ( int val ) {
        _impl->setExposureCompensation ( val );
    }
    void RaspiCam::setAWB ( RASPICAM_AWB awb ) {
        _impl->setAWB ( awb );
    }
    void RaspiCam::setAWB_RB ( float r,float b ){
        _impl->setAWB_RB(r,b);
    }

    void RaspiCam::setImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect ) {
        _impl->setImageEffect ( imageEffect );
    }
    void RaspiCam::setMetering ( RASPICAM_METERING metering ) {
        _impl->setMetering ( metering );
    }
    void RaspiCam::setHorizontalFlip ( bool hFlip ) {
        _impl->setHorizontalFlip ( hFlip );
    }
    void RaspiCam::setVerticalFlip ( bool vFlip ) {
        _impl->setVerticalFlip ( vFlip );
    }
    void RaspiCam::setFrameRate ( int frames_per_second ) {
        _impl->setFrameRate ( frames_per_second );
    }
    void RaspiCam::setSensorMode ( int mode ) {
        _impl->setSensorMode ( mode );
    }



    RASPICAM_FORMAT RaspiCam::getFormat()const{return _impl->getFormat( ); }
    unsigned int RaspiCam::getWidth() const{return _impl->getWidth() ;}
    unsigned int RaspiCam::getHeight() const{return _impl->getHeight()  ;}
    unsigned int RaspiCam::getBrightness() const{return _impl->getBrightness()  ;}
    unsigned int RaspiCam::getRotation() const{return _impl->getRotation()  ;}
    int RaspiCam::getISO() const{return _impl->getISO() ;}
    unsigned int RaspiCam::getShutterSpeed() const{return _impl->getShutterSpeed();}

    int RaspiCam::getSharpness() const{return _impl->getSharpness() ;}
    int RaspiCam::getContrast() const{return _impl->getContrast() ;}
    int RaspiCam::getSaturation() const{return _impl->getSaturation() ;}
    RASPICAM_EXPOSURE RaspiCam::getExposure() const {return _impl->getExposure() ;}
    RASPICAM_AWB RaspiCam::getAWB() const{return _impl->getAWB() ;}
    float RaspiCam::getAWBG_red()const{return _impl->getAWBG_red();}
    float RaspiCam::getAWBG_blue()const{return _impl->getAWBG_blue();}

    RASPICAM_IMAGE_EFFECT RaspiCam::getImageEffect() const{return _impl->getImageEffect() ;}
    RASPICAM_METERING RaspiCam::getMetering() const{return _impl->getMetering() ;}
    int RaspiCam::getFrameRate() const{return _impl->getFrameRate() ;}
    bool RaspiCam::isHorizontallyFlipped() const {return _impl->isHorizontallyFlipped()  ;}
    bool RaspiCam::isVerticallyFlipped() const {return _impl->isVerticallyFlipped() ;}

    //Returns an id of the camera. We assume the camera id is the one of the raspberry
    //the id is obtained using raspberry serial number obtained in /proc/cpuinfo
    std::string RaspiCam::getId() const{return _impl->getId();}


}
