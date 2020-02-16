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

#include "mmal/util/mmal_util.h"
#include "mmal/util/mmal_util_params.h"
#include "mmal/util/mmal_default_components.h"

#include "raspicamrawbuffer_impl.h"

namespace raspicam {
    namespace _private {

        void RaspiCamRawBufferImpl::moveFrom(RaspiCamRawBufferImpl &src) {
            _buffer.reset<MMAL_BUFFER_HEADER_T>(0);
            _buffer.swap(src._buffer);
        }

        void RaspiCamRawBufferImpl::setMmalBufferHeader(MMAL_BUFFER_HEADER_T *buffer) {
            _buffer = std::shared_ptr<MMAL_BUFFER_HEADER_T>(buffer, Deleter());

#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << buffer << "->" << (void*)buffer->data
            << " has been wrapped by RaspiCamRawBuffer."
            << std::endl
            << std::flush;
#endif
        }

        void* RaspiCamRawBufferImpl::getBuffer() {
            return _buffer ? _buffer.get()->data : 0;
        }

        const void* RaspiCamRawBufferImpl::getBuffer() const {
            return _buffer ? _buffer.get()->data : 0;
        }

        void RaspiCamRawBufferImpl::lock() {
            if (_buffer)
                mmal_buffer_header_mem_lock(_buffer.get());
        }

        void RaspiCamRawBufferImpl::unlock() {
            if (_buffer)
                mmal_buffer_header_mem_unlock(_buffer.get());
        }

        void RaspiCamRawBufferImpl::Deleter::operator()(MMAL_BUFFER_HEADER_T *_buffer) const {
#ifdef RASPICAM_RAW_BUFFER_ENABLE_TRACE
            std::cout << std::hex
            << "RaspiCam: buffer " << _buffer << "->" << (void*)_buffer->data
            << " has been released." << std::endl
            << std::flush;
#endif
            mmal_buffer_header_release(_buffer);
        }

        long RaspiCamRawBufferImpl::getUseCount() const {
            return _buffer.use_count();
        }
    }
}
