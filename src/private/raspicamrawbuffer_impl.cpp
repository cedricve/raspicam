//
// Created by Stepan Dyatkovskiy on 12/6/18.
//

#include "mmal/util/mmal_util.h"
#include "mmal/util/mmal_util_params.h"
#include "mmal/util/mmal_default_components.h"

#include "raspicamrawbuffer_impl.h"

namespace raspicam {
    namespace _private {
        RaspiCamRawBufferImpl::RaspiCamRawBufferImpl()
        : _buffer(0),
          _acquired(false) {}

        void RaspiCamRawBufferImpl::construct(const raspicam::_private::RaspiCamRawBufferImpl &src) {
            _buffer = const_cast<MMAL_BUFFER_HEADER_T*>(src._buffer);
            if (_buffer) {
                _acquired = true;
                mmal_buffer_header_acquire(_buffer);
            } else {
                _acquired = false;
            }
        }

        void RaspiCamRawBufferImpl::dispose() {
            if (_acquired) {
                mmal_buffer_header_release(_buffer);
            }
        }

        RaspiCamRawBufferImpl::~RaspiCamRawBufferImpl() {
            dispose();
        }

        RaspiCamRawBufferImpl::RaspiCamRawBufferImpl(const RaspiCamRawBufferImpl &src) {
            construct(src);
        }

        RaspiCamRawBufferImpl& RaspiCamRawBufferImpl::operator=(const RaspiCamRawBufferImpl &src) {
            dispose();
            construct(src);
            return *this;
        }

        void RaspiCamRawBufferImpl::moveFrom(RaspiCamRawBufferImpl &src) {
            _buffer = src._buffer;
            _acquired = src._acquired;

            src._buffer = 0;
            src._acquired = false;
        }

        void RaspiCamRawBufferImpl::setMmalBufferHeader(MMAL_BUFFER_HEADER_T *buffer, bool acquire) {
            _buffer = buffer;

            if (acquire) {
                mmal_buffer_header_acquire(_buffer);
            }

            _acquired = acquire;
        }

        void* RaspiCamRawBufferImpl::getBuffer() {
            return _buffer ? _buffer->data : 0;
        }

        const void* RaspiCamRawBufferImpl::getBuffer() const {
            return _buffer ? _buffer->data : 0;
        }

        void RaspiCamRawBufferImpl::lock() {
            if (_buffer) {
                mmal_buffer_header_mem_lock(_buffer);
            }
        }

        void RaspiCamRawBufferImpl::unlock() {
            if (_buffer) {
                mmal_buffer_header_mem_unlock(_buffer);
            }
        }
    }
}
