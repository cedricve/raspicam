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

#include <iostream>

#include "raspicamrawbuffer.h"
#include "private/raspicamrawbuffer_impl.h"

namespace raspicam {

    RaspiCamRawBuffer::RaspiCamRawBuffer() {
        _impl = new _private::RaspiCamRawBufferImpl();
    }

    RaspiCamRawBuffer::~RaspiCamRawBuffer() {
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _impl->getBuffer()
            << " use is to be removed [" << _impl->getUseCount() << "] by destructor"
            << std::endl
            << std::flush;
#endif
        delete _impl;
    }

    RaspiCamRawBuffer::RaspiCamRawBuffer(const raspicam::RaspiCamRawBuffer &src) {
        _impl = new _private::RaspiCamRawBufferImpl(*src._impl);
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _impl->getBuffer()
            << " added use [" << _impl->getUseCount() << "] by copy constructor"
            << std::endl
            << std::flush;
#endif
    }

    RaspiCamRawBuffer& RaspiCamRawBuffer::operator=(const raspicam::RaspiCamRawBuffer &src) {
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _impl->getBuffer()
            << " use is to be removed [" << _impl->getUseCount() << "] by assignment"
            << std::endl
            << std::flush;
#endif
        *_impl = *src._impl;
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _impl->getBuffer()
            << " added use [" << _impl->getUseCount() << "] by assignment"
            << std::endl
            << std::flush;
#endif
        return *this;
    }

    void RaspiCamRawBuffer::moveFrom(raspicam::RaspiCamRawBuffer &src) {
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _impl->getBuffer()
            << " use is to be removed [" << _impl->getUseCount() << "] by moveFrom method"
            << std::endl
            << std::flush;
#endif
        _impl->moveFrom(*src._impl);
    }

    void *RaspiCamRawBuffer::getBuffer() {
        return _impl->getBuffer();
    }

    const void *RaspiCamRawBuffer::getBuffer() const {
        return _impl->getBuffer();
    }

    void RaspiCamRawBuffer::lock() {
        _impl->lock();
    }

    void RaspiCamRawBuffer::unlock() {
        _impl->unlock();
    }

}
