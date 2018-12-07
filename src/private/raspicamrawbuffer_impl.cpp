//
// Created by Stepan Dyatkovskiy on 12/6/18.
//

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
