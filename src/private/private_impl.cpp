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

#include "private_impl.h"
#include <iostream>
#include <cstdio>
#include "mmal/util/mmal_util.h"
#include "mmal/util/mmal_util_params.h"
#include "mmal/util/mmal_default_components.h"
using namespace std;
namespace raspicam {
    namespace _private{
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2
#define VIDEO_FRAME_RATE_DEN 1
#define VIDEO_OUTPUT_BUFFERS_NUM 3


        Private_Impl::Private_Impl() {
            camera_video_port = NULL;
//             camera_still_port = NULL;
            _isOpened=false;
            _isCapturing=false;
            //set default state params
            setDefaultStateParams();
        }

        Private_Impl::~Private_Impl() {

            release();
        }

        void Private_Impl::setDefaultStateParams() {

            // Default everything to zero
            memset ( &State, 0, sizeof ( RASPIVID_STATE ) );
            State.framerate 		= 10;
            State.width 			= 1280;      // use a multiple of 320 (640, 1280)
            State.height 			= 960;		// use a multiple of 240 (480, 960)
            State.sharpness = 0;
            State.contrast = 0;
            State.brightness = 50;
            State.saturation = 0;
            State.ISO = 400;
            State.videoStabilisation = false;
            State.exposureCompensation = 0;
            State.captureFtm=RASPICAM_FORMAT_RGB;
            State.rpc_exposureMode = RASPICAM_EXPOSURE_AUTO;
            State.rpc_exposureMeterMode = RASPICAM_METERING_AVERAGE;
            State.rpc_awbMode = RASPICAM_AWB_AUTO;
            State.rpc_imageEffect = RASPICAM_IMAGE_EFFECT_NONE;
            State.colourEffects.enable = 0;
            State.colourEffects.u = 128;
            State.colourEffects.v = 128;
            State.rotation = 0;
            State.hflip = State.vflip = 0;
            State.roi.x = State.roi.y = 0.0;
            State.roi.w = State.roi.h = 1.0;
            State.shutterSpeed=0;//auto
            State.awbg_red=1.0;
            State.awbg_blue=1.0;
            State.sensor_mode = 0; //do not set mode by default

        }
        bool  Private_Impl::open ( bool StartCapture ) {
            if ( _isOpened ) return false; //already opened
// create camera
            if ( ! create_camera_component ( &State ) ) {
                cerr<<__func__<<" Failed to create camera component"<<__FILE__<<" "<<__LINE__<<endl;
                return false;
            }
            commitParameters();
            camera_video_port   = State.camera_component->output[MMAL_CAMERA_VIDEO_PORT];
            callback_data.pstate = &State;
            callback_data.inst = this;
            // assign data to use for callback
            camera_video_port->userdata = ( struct MMAL_PORT_USERDATA_T * ) &callback_data;
            State.camera_component->control->userdata = ( struct MMAL_PORT_USERDATA_T * ) &callback_data;

            _isOpened=true;
            if ( StartCapture ) return startCapture();
            else return true;
        }
        /**
         */
        bool Private_Impl::startCapture() {
            if ( !_isOpened ) {
                cerr<<__FILE__<<":"<<__LINE__<<":"<<__func__<<" not opened."<<endl;
                return false; //already opened
            }

            // start capture
            if ( mmal_port_parameter_set_boolean ( camera_video_port, MMAL_PARAMETER_CAPTURE, 1 ) != MMAL_SUCCESS ) {
                release();
                return false;
            }
            // Send all the buffers to the video port

            int num = mmal_queue_length ( State.video_pool->queue );
            int q;
            for ( q=0; q<num; q++ ) {
                MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get ( State.video_pool->queue );

                if ( !buffer )
                    cerr<<"Unable to get a required buffer"<<q<<" from pool queue"<<endl;

                if ( mmal_port_send_buffer ( camera_video_port, buffer ) != MMAL_SUCCESS )
                    cerr<<"Unable to send a buffer to encoder output port "<< q<<endl;
            }
            _isCapturing=true;
            return true;
        }

        void Private_Impl::setUserCallback(void (*userCallback)(void*) , void* data){
            callback_data._userCallbackData = data;
            callback_data._userCallback = userCallback;
        }

        void Private_Impl::release() {
            if ( !_isOpened ) return;

            // Disable camera_video_port
            if ( camera_video_port && camera_video_port->is_enabled ) {
                mmal_port_disable ( camera_video_port );
                camera_video_port = NULL;
            }
            ////
            // Disable all our ports that are not handled by connections
            if ( State.camera_component )
                mmal_component_disable ( State.camera_component );


            destroy_camera_component ( &State );

            _isOpened=false;
            _isCapturing=false;

        }
        /**
        *
         */
        bool Private_Impl::grab() {
            if ( !isCapturing() ) return false;
            callback_data.waitForFrame();
            return true;
        }
        /**
        *
         */
        void Private_Impl::retrieve ( unsigned char *data,RASPICAM_FORMAT type ) {
            if ( callback_data._buffData.size==0 ) return;
            if ( type!=RASPICAM_FORMAT_IGNORE ) {
                cerr<<__FILE__<<":"<<__LINE__<<" :Private_Impl::retrieve type is not RASPICAM_FORMAT_IGNORE as it should be"<<endl;
            }
            memcpy ( data,callback_data._buffData.data,getImageTypeSize ( State.captureFtm ) );
        }


        unsigned char *Private_Impl::getImageBufferData() const{
            return callback_data._buffData.data;
        }

        size_t Private_Impl::getImageBufferSize() const{
            return getImageTypeSize ( getFormat() );
        }


        /**
        *
         *
         */

        size_t Private_Impl::getImageTypeSize ( RASPICAM_FORMAT type ) const{
            switch ( type ) {
            case RASPICAM_FORMAT_YUV420:
                return getWidth() *getHeight() + 2* ( ( getWidth() /2 *getHeight() /2 ) );
                break;
            case RASPICAM_FORMAT_GRAY:
                return getWidth() *getHeight();
                break;
            case RASPICAM_FORMAT_BGR:
            case RASPICAM_FORMAT_RGB:
                return 3*getWidth() *getHeight();
                break;
            default:
                return 0;
            };
        }



        /**
         * Destroy the camera component
         *
         * @param state Pointer to state control struct
         *
         */
        void Private_Impl::destroy_camera_component ( RASPIVID_STATE *state ) {
            if ( state->video_pool )
                mmal_port_pool_destroy ( state->camera_component->output[MMAL_CAMERA_VIDEO_PORT], state->video_pool );
            if ( state->camera_component ) {
                mmal_component_destroy ( state->camera_component );
                state->camera_component = NULL;
            }
        }
        MMAL_COMPONENT_T *Private_Impl::create_camera_component ( RASPIVID_STATE *state ) {
            MMAL_COMPONENT_T *camera = 0;
            MMAL_ES_FORMAT_T *format;
            MMAL_PORT_T  *video_port = NULL;

            MMAL_STATUS_T status;
            /* Create the component */
            status = mmal_component_create ( MMAL_COMPONENT_DEFAULT_CAMERA, &camera );

            if ( status != MMAL_SUCCESS ) {
                cerr<< ( "Failed to create camera component" );
                return 0;
            }

            if ( !camera->output_num ) {
                cerr<< ( "Camera doesn't have output ports" );
                mmal_component_destroy ( camera );
                return 0;
            }

            video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];

            //set sensor mode
            if ( state->sensor_mode != 0 && mmal_port_parameter_set_uint32 ( camera->control,
                                                    MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG,
                                                    state->sensor_mode)  != MMAL_SUCCESS)
            {
                cerr << __func__ << ": Failed to set sensmode.";
            }

            _rgb_bgr_fixed = !(mmal_util_rgb_order_fixed(video_port));

            //  set up the camera configuration

            MMAL_PARAMETER_CAMERA_CONFIG_T cam_config;
            cam_config.hdr.id=MMAL_PARAMETER_CAMERA_CONFIG;
            cam_config.hdr.size=sizeof ( cam_config );
            cam_config.max_stills_w = state->width;
            cam_config.max_stills_h = state->height;
            cam_config.stills_yuv422 = 0;
            cam_config.one_shot_stills = 0;
            cam_config.max_preview_video_w = state->width;
            cam_config.max_preview_video_h = state->height;
            cam_config.num_preview_video_frames = 3;
            cam_config.stills_capture_circular_buffer_height = 0;
            cam_config.fast_preview_resume = 0;
            cam_config.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;
            mmal_port_parameter_set ( camera->control, &cam_config.hdr );

            MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T change_event_request =
                    {{MMAL_PARAMETER_CHANGE_EVENT_REQUEST, sizeof(MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T)},
                     MMAL_PARAMETER_CAMERA_SETTINGS, 1};

            status = mmal_port_parameter_set(camera->control, &change_event_request.hdr);
            if ( status != MMAL_SUCCESS )
            {
                cerr<<("No camera settings events");
                mmal_component_destroy ( camera );
                return 0;
            }

            // Enable the camera, and tell it its control callback function
            status = mmal_port_enable(camera->control, camera_control_callback);

            if (status != MMAL_SUCCESS)
            {
                cerr << "Unable to enable control port : error " << (int) status;
                mmal_component_destroy ( camera );
                return 0;
            }

            // Set the encode format on the video  port

            format = video_port->format;
            format->encoding_variant =   convertFormat ( State.captureFtm );
            format->encoding = convertFormat ( State.captureFtm );
            format->es->video.width = VCOS_ALIGN_UP(state->width, 32);
            format->es->video.height = VCOS_ALIGN_UP(state->height, 16);
            format->es->video.crop.x = 0;
            format->es->video.crop.y = 0;
            format->es->video.crop.width = state->width;
            format->es->video.crop.height = state->height;
            format->es->video.frame_rate.num = state->framerate;
            format->es->video.frame_rate.den = VIDEO_FRAME_RATE_DEN;

            status = mmal_port_format_commit ( video_port );
            if ( status ) {
                cerr<< ( "camera video format couldn't be set" );
                mmal_component_destroy ( camera );
                return 0;
            }

            // PR : plug the callback to the video port
            status = mmal_port_enable ( video_port,video_buffer_callback );
            if ( status ) {
                cerr<< ( "camera video callback2 error" );
                mmal_component_destroy ( camera );
                return 0;
            }

            // Ensure there are enough buffers to avoid dropping frames
            if ( video_port->buffer_num < VIDEO_OUTPUT_BUFFERS_NUM )
                video_port->buffer_num = VIDEO_OUTPUT_BUFFERS_NUM;



            //PR : create pool of message on video port
            MMAL_POOL_T *pool;
            video_port->buffer_size = video_port->buffer_size_recommended;
            video_port->buffer_num = video_port->buffer_num_recommended;
            pool = mmal_port_pool_create ( video_port, video_port->buffer_num, video_port->buffer_size );
            if ( !pool ) {
                cerr<< ( "Failed to create buffer header pool for video output port" );
            }
            state->video_pool = pool;


            /* Enable component */
            status = mmal_component_enable ( camera );

            if ( status ) {
                cerr<< ( "camera component couldn't be enabled" );
                mmal_component_destroy ( camera );
                return 0;
            }

            state->camera_component = camera;//this needs to be before set_all_parameters

            return camera;
        }


        void Private_Impl::commitBrightness() {
            mmal_port_parameter_set_rational ( State.camera_component->control, MMAL_PARAMETER_BRIGHTNESS, ( MMAL_RATIONAL_T ) {
                State.brightness, 100
            } );
        }


        void Private_Impl::commitRotation() {
            int rotation = int ( State.rotation / 90 ) * 90;
            mmal_port_parameter_set_int32 ( State.camera_component->output[0], MMAL_PARAMETER_ROTATION,rotation );
            mmal_port_parameter_set_int32 ( State.camera_component->output[1], MMAL_PARAMETER_ROTATION,rotation );
            mmal_port_parameter_set_int32 ( State.camera_component->output[2], MMAL_PARAMETER_ROTATION, rotation );
        }

        void Private_Impl::commitISO() {
            if ( mmal_port_parameter_set_uint32 ( State.camera_component->control, MMAL_PARAMETER_ISO, State.ISO ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set ISO parameter.\n";
        }

        void Private_Impl::commitSharpness() {
            if ( mmal_port_parameter_set_rational ( State.camera_component->control, MMAL_PARAMETER_SHARPNESS, ( MMAL_RATIONAL_T ) {
            State.sharpness, 100
        } ) != MMAL_SUCCESS )
            cout << __func__ << ": Failed to set sharpness parameter.\n";
        }

        void Private_Impl::commitShutterSpeed() {
            if ( mmal_port_parameter_set_uint32 ( State.camera_component->control, MMAL_PARAMETER_SHUTTER_SPEED, State.shutterSpeed ) !=  MMAL_SUCCESS )
                cout << __func__ << ": Failed to set shutter parameter.\n";
        }



        void Private_Impl::commitContrast() {
            if ( mmal_port_parameter_set_rational ( State.camera_component->control, MMAL_PARAMETER_CONTRAST, ( MMAL_RATIONAL_T ) {
            State.contrast, 100
        } ) != MMAL_SUCCESS )
            cout << __func__ << ": Failed to set contrast parameter.\n";
        }

        void Private_Impl::commitSaturation() {
            if ( mmal_port_parameter_set_rational ( State.camera_component->control, MMAL_PARAMETER_SATURATION, ( MMAL_RATIONAL_T ) {
            State.saturation, 100
        } ) != MMAL_SUCCESS )
            cout << __func__ << ": Failed to set saturation parameter.\n";
        }

        void Private_Impl::commitExposure() {
            MMAL_PARAMETER_EXPOSUREMODE_T exp_mode = {{MMAL_PARAMETER_EXPOSURE_MODE,sizeof ( exp_mode ) }, convertExposure ( State.rpc_exposureMode ) };
            if ( mmal_port_parameter_set ( State.camera_component->control, &exp_mode.hdr ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set exposure parameter.\n";
        }
        /**
         * Adjust the exposure compensation for images (EV)
         * @param camera Pointer to camera component
         * @param exp_comp Value to adjust, -10 to +10
         * @return 0 if successful, non-zero if any parameters out of range
         */
        void Private_Impl::commitExposureCompensation() {
            if ( mmal_port_parameter_set_int32 ( State.camera_component->control, MMAL_PARAMETER_EXPOSURE_COMP , State.exposureCompensation ) !=MMAL_SUCCESS )
                cout << __func__ << ": Failed to set Exposure Compensation parameter.\n";

        }

        void Private_Impl::commitAWB() {
            MMAL_PARAMETER_AWBMODE_T param = {{MMAL_PARAMETER_AWB_MODE,sizeof ( param ) }, convertAWB ( State.rpc_awbMode ) };
            if ( mmal_port_parameter_set ( State.camera_component->control, &param.hdr ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set AWB parameter.\n";
        }

        void Private_Impl::commitImageEffect() {
            MMAL_PARAMETER_IMAGEFX_T imgFX = {{MMAL_PARAMETER_IMAGE_EFFECT,sizeof ( imgFX ) }, convertImageEffect ( State.rpc_imageEffect ) };
            if ( mmal_port_parameter_set ( State.camera_component->control, &imgFX.hdr ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set image effect parameter.\n";
        }

        void Private_Impl::commitMetering() {
            MMAL_PARAMETER_EXPOSUREMETERINGMODE_T meter_mode = {{MMAL_PARAMETER_EXP_METERING_MODE, sizeof ( meter_mode ) }, convertMetering ( State.rpc_exposureMeterMode ) };
            if ( mmal_port_parameter_set ( State.camera_component->control, &meter_mode.hdr ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set metering parameter.\n";
        }

        void Private_Impl::commitFlips() {
            MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof ( MMAL_PARAMETER_MIRROR_T ) }, MMAL_PARAM_MIRROR_NONE};
            if ( State.hflip && State.vflip )
                mirror.value = MMAL_PARAM_MIRROR_BOTH;
            else if ( State.hflip )
                mirror.value = MMAL_PARAM_MIRROR_HORIZONTAL;
            else if ( State.vflip )
                mirror.value = MMAL_PARAM_MIRROR_VERTICAL;
            if ( mmal_port_parameter_set ( State.camera_component->output[0], &mirror.hdr ) != MMAL_SUCCESS ||
                    mmal_port_parameter_set ( State.camera_component->output[1], &mirror.hdr ) != MMAL_SUCCESS ||
                    mmal_port_parameter_set ( State.camera_component->output[2], &mirror.hdr ) )
                cout << __func__ << ": Failed to set horizontal/vertical flip parameter.\n";
        }


        /**
         * Set the specified camera to all the specified settings
         * @param camera Pointer to camera component
         * @param params Pointer to parameter block containing parameters
         * @return 0 if successful, none-zero if unsuccessful.
         */
        void Private_Impl::commitParameters ( ) {
            assert ( State.camera_component!=0 );
            commitSaturation();
            commitSharpness();
            commitContrast();
            commitBrightness();
            commitISO();
            if ( State.shutterSpeed!=0 ) {
                commitShutterSpeed();
                State.rpc_exposureMode=RASPICAM_EXPOSURE_FIXEDFPS;
                commitExposure();
            } else           commitExposure();
            commitExposureCompensation();
            commitMetering();
            commitImageEffect();
            commitRotation();
            commitFlips();
            commitVideoStabilization();
            commitAWB();
            commitAWB_RB();

        }
        void Private_Impl::commitVideoStabilization() {
            // Set Video Stabilization
            if ( mmal_port_parameter_set_boolean ( State.camera_component->control, MMAL_PARAMETER_VIDEO_STABILISATION, State.videoStabilisation ) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set video stabilization parameter.\n";
        }

        void Private_Impl::camera_control_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
        {
            PORT_USERDATA *pData = ( PORT_USERDATA * ) port->userdata;
            std::unique_lock<std::mutex> lck ( pData->_mutex );

            if (buffer->cmd == MMAL_EVENT_PARAMETER_CHANGED)
            {
                MMAL_EVENT_PARAMETER_CHANGED_T *param = (MMAL_EVENT_PARAMETER_CHANGED_T *)buffer->data;
                if (param->hdr.id == MMAL_PARAMETER_CAMERA_SETTINGS) {
                    MMAL_PARAMETER_CAMERA_SETTINGS_T *settings = (MMAL_PARAMETER_CAMERA_SETTINGS_T*)param;

                    pData->pstate->shutterSpeed = settings->exposure;
                    pData->pstate->awbg_red = float(settings->awb_red_gain.num)/settings->awb_red_gain.den;
                    pData->pstate->awbg_blue = float(settings->awb_blue_gain.num)/settings->awb_blue_gain.den;
                }
            }
            else if (buffer->cmd == MMAL_EVENT_ERROR)
            {
                printf("No data received from sensor. Check all connections, including the Sunny one on the camera board");
            }
            else
                printf("Received unexpected camera control callback event, 0x%08x", buffer->cmd);

            mmal_buffer_header_release(buffer);
        }

        void Private_Impl::video_buffer_callback ( MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer ) {
            MMAL_BUFFER_HEADER_T *new_buffer;
            PORT_USERDATA *pData = ( PORT_USERDATA * ) port->userdata;

            bool hasGrabbed=false;
//            pData->_mutex.lock();
             std::unique_lock<std::mutex> lck ( pData->_mutex );
            if ( pData ) {
                if( buffer->length &&
                        ( pData->_userCallback || pData->wantToGrab )){
                    mmal_buffer_header_mem_lock ( buffer );

                    Private_Impl *self = pData->inst;

                    int width = pData->pstate->width;
                    int height = pData->pstate->height;
                    RASPICAM_FORMAT fmt = pData->pstate->captureFtm;
                    bool encoded = false; // So far only unencoded formats can be configured

                    // For unencoded formats, the buffer is padding to blocks
                    // TODO: According to picamera ('Under certain circumstances (non-resized, non-YUV, video-port captures), the resolution is rounded to 16x16 blocks instead of 32x16. Adjust your resolution rounding accordingly')
                    int bufferWidth = VCOS_ALIGN_UP(width, 32);
                    int bufferHeight = VCOS_ALIGN_UP(height, 16);

                    if ( bufferWidth == width || encoded ) {
                        pData->_buffData.resize ( buffer->length );
                        memcpy ( pData->_buffData.data,buffer->data,buffer->length );
                    }
                    else {

                        pData->_buffData.resize ( self->getImageTypeSize( fmt ) );

                        int bpp = 1;
                        if(fmt == RASPICAM_FORMAT_RGB || fmt == RASPICAM_FORMAT_BGR) {
                            bpp = 3;
                        }

                        for(int i = 0; i < height; i++) {
                            memcpy ( pData->_buffData.data + i*width*bpp, buffer->data + i*bufferWidth*bpp, width*bpp);
                        }

                        if ( fmt == RASPICAM_FORMAT_YUV420 ) {
                            // Starting points in both buffers
                            uint8_t *outUV = pData->_buffData.data + width*height;
                            uint8_t *bufferUV = buffer->data + bufferHeight*bufferWidth;

                            width /= 2;
                            height /= 2;
                            bufferWidth /= 2;
                            bufferHeight /= 2;

                            for(int plane = 0; plane < 2; plane++) {
                                for(int i = 0; i < height; i++) {
                                    memcpy ( outUV + i*width, bufferUV + i*bufferWidth, width );
                                }
                                outUV += width*height;
                                bufferUV += bufferWidth*bufferHeight;
                            }
                        }

                    }

                    pData->wantToGrab =false;
                    hasGrabbed=true;
                    mmal_buffer_header_mem_unlock ( buffer );
                }
            }
            //pData->_mutex.unlock();
           // if ( hasGrabbed ) pData->Thcond.BroadCast(); //wake up waiting client
            // release buffer back to the pool
            mmal_buffer_header_release ( buffer );
            // and send one back to the port (if still open)
            if ( port->is_enabled ) {
                MMAL_STATUS_T status;

                new_buffer = mmal_queue_get ( pData->pstate->video_pool->queue );

                if ( new_buffer )
                    status = mmal_port_send_buffer ( port, new_buffer );

                if ( !new_buffer || status != MMAL_SUCCESS )
                    printf ( "Unable to return a buffer to the encoder port" );
            }

            if ( pData->pstate->shutterSpeed!=0 && pData->pstate->rpc_exposureMode == RASPICAM_EXPOSURE_FIXEDFPS)
                mmal_port_parameter_set_uint32 ( pData->pstate->camera_component->control, MMAL_PARAMETER_SHUTTER_SPEED, pData->pstate->shutterSpeed ) ;

            if ( hasGrabbed ) {
                if ( pData->_userCallback ) {
                    // TODO - add locking mechanism for the callback
                    pData->_userCallback(pData->_userCallbackData);
                }
                pData->Thcond.BroadCast(); //wake up waiting client
            }

        }



        void Private_Impl::setWidth ( unsigned int width ) {
            State.width = width;
        }

        void Private_Impl::setHeight ( unsigned int height ) {
            State.height = height;
        }
        void Private_Impl::setFormat ( RASPICAM_FORMAT fmt ) {
            if ( isOpened() ) {
                cerr<<__FILE__<<":"<<__LINE__<<":"<<__func__<<": can not change format with camera already opened"<<endl;
                return;
            }
            State.captureFtm = fmt;
        }

        void Private_Impl::setSensorMode ( int mode ) {
            if ( isOpened() ) {
                cerr<<__FILE__<<":"<<__LINE__<<":"<<__func__<<": can not change sensor mode with camera already opened"<<endl;
                return;
            }
            State.sensor_mode = mode;
        }

        void Private_Impl::setCaptureSize ( unsigned int width, unsigned int height ) {
            setWidth ( width );
            setHeight ( height );
        }

        void Private_Impl::setVideoStabilization ( bool v ) {
            State.videoStabilisation=v;
            if ( isOpened() ) commitVideoStabilization();
        }

        void Private_Impl::setBrightness ( unsigned int brightness ) {
            if ( brightness > 100 )                brightness = 100 ;
            State.brightness = brightness;
            if ( isOpened() ) commitBrightness();
        }
        void Private_Impl::setShutterSpeed ( unsigned  int shutter ) {
            if ( shutter > 330000 )
                shutter = 330000;
            State.shutterSpeed= shutter;
            if ( isOpened() ) commitShutterSpeed();
        }




        void Private_Impl::setRotation ( int rotation ) {
            while ( rotation < 0 )
                rotation += 360;
            if ( rotation >= 360 )
                rotation = rotation % 360;
            State.rotation = rotation;
            if ( isOpened() ) commitRotation();
        }

        void Private_Impl::setISO ( int iso ) {
            State.ISO = iso;
            if ( isOpened() ) commitISO();
        }

        void Private_Impl::setSharpness ( int sharpness ) {
            if ( sharpness < -100 ) sharpness = -100;
            if ( sharpness > 100 ) sharpness = 100;
            State.sharpness = sharpness;
            if ( isOpened() ) commitSharpness();
        }

        void Private_Impl::setContrast ( int contrast ) {
            if ( contrast < -100 ) contrast = -100;
            if ( contrast > 100 ) contrast = 100;
            State.contrast = contrast;
            if ( isOpened() ) commitContrast();
        }

        void Private_Impl::setSaturation ( int saturation ) {
            if ( saturation < -100 ) saturation = -100;
            if ( saturation > 100 ) saturation = 100;
            State.saturation = saturation;
            if ( isOpened() ) commitSaturation();
        }


        void Private_Impl::setAWB_RB ( float red_g, float blue_g ) {
            State.awbg_blue = blue_g;
            State.awbg_red = red_g;
            if ( isOpened() ) commitAWB_RB();
        }
        void Private_Impl::setExposure ( RASPICAM_EXPOSURE exposure ) {
            State.rpc_exposureMode = exposure;
            if ( isOpened() ) commitExposure();
        }

        void Private_Impl::setAWB ( RASPICAM_AWB awb ) {
            State.rpc_awbMode = awb;
            if ( isOpened() ) commitAWB();
        }

        void Private_Impl::setImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect ) {
            State.rpc_imageEffect = imageEffect;
            if ( isOpened() ) commitImageEffect();
        }

        void Private_Impl::setMetering ( RASPICAM_METERING metering ) {
            State.rpc_exposureMeterMode = metering;
            if ( isOpened() ) commitMetering();
        }
        void Private_Impl::setExposureCompensation ( int val ) {
            if ( val < -10 ) val= -10;
            if ( val > 10 ) val = 10;
            State.exposureCompensation=val;
            if ( isOpened() ) commitExposureCompensation();
        }

        void Private_Impl::setHorizontalFlip ( bool hFlip ) {
            State.hflip = hFlip;
            if ( isOpened() ) commitFlips();
        }

        void Private_Impl::setVerticalFlip ( bool vFlip ) {
            State.vflip = vFlip;
            if ( isOpened() ) commitFlips();
        }

        void Private_Impl::setFrameRate ( int frames_per_second ) {
            State.framerate = frames_per_second;
        }

        MMAL_PARAM_EXPOSUREMETERINGMODE_T Private_Impl::convertMetering ( RASPICAM_METERING metering ) {
            switch ( metering ) {
            case RASPICAM_METERING_AVERAGE:
                return MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE;
            case RASPICAM_METERING_SPOT:
                return  MMAL_PARAM_EXPOSUREMETERINGMODE_SPOT;
            case RASPICAM_METERING_BACKLIT:
                return MMAL_PARAM_EXPOSUREMETERINGMODE_BACKLIT;
            case RASPICAM_METERING_MATRIX:
                return MMAL_PARAM_EXPOSUREMETERINGMODE_MATRIX;
            default:
                return MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE;
            }
        }
        MMAL_PARAM_EXPOSUREMODE_T Private_Impl::convertExposure ( RASPICAM_EXPOSURE exposure ) {

            switch ( exposure ) {
            case RASPICAM_EXPOSURE_OFF:
                return MMAL_PARAM_EXPOSUREMODE_OFF;
            case RASPICAM_EXPOSURE_AUTO:
                return MMAL_PARAM_EXPOSUREMODE_AUTO;
            case RASPICAM_EXPOSURE_NIGHT:
                return MMAL_PARAM_EXPOSUREMODE_NIGHT;
            case RASPICAM_EXPOSURE_NIGHTPREVIEW:
                return MMAL_PARAM_EXPOSUREMODE_NIGHTPREVIEW;
            case RASPICAM_EXPOSURE_BACKLIGHT:
                return MMAL_PARAM_EXPOSUREMODE_BACKLIGHT;
            case RASPICAM_EXPOSURE_SPOTLIGHT:
                return MMAL_PARAM_EXPOSUREMODE_SPOTLIGHT;
            case RASPICAM_EXPOSURE_SPORTS:
                return MMAL_PARAM_EXPOSUREMODE_SPORTS;
            case RASPICAM_EXPOSURE_SNOW:
                return MMAL_PARAM_EXPOSUREMODE_SNOW;
            case RASPICAM_EXPOSURE_BEACH:
                return MMAL_PARAM_EXPOSUREMODE_BEACH;
            case RASPICAM_EXPOSURE_VERYLONG:
                return MMAL_PARAM_EXPOSUREMODE_VERYLONG;
            case RASPICAM_EXPOSURE_FIXEDFPS:
                return MMAL_PARAM_EXPOSUREMODE_FIXEDFPS;
            case RASPICAM_EXPOSURE_ANTISHAKE:
                return MMAL_PARAM_EXPOSUREMODE_ANTISHAKE;
            case RASPICAM_EXPOSURE_FIREWORKS:
                return MMAL_PARAM_EXPOSUREMODE_FIREWORKS;
            default:
                return MMAL_PARAM_EXPOSUREMODE_AUTO;
            }
        }

        MMAL_PARAM_AWBMODE_T Private_Impl::convertAWB ( RASPICAM_AWB awb ) {
            switch ( awb ) {
            case RASPICAM_AWB_OFF:
                return MMAL_PARAM_AWBMODE_OFF;
            case RASPICAM_AWB_AUTO:
                return MMAL_PARAM_AWBMODE_AUTO;
            case RASPICAM_AWB_SUNLIGHT:
                return MMAL_PARAM_AWBMODE_SUNLIGHT;
            case RASPICAM_AWB_CLOUDY:
                return MMAL_PARAM_AWBMODE_CLOUDY;
            case RASPICAM_AWB_SHADE:
                return MMAL_PARAM_AWBMODE_SHADE;
            case RASPICAM_AWB_TUNGSTEN:
                return MMAL_PARAM_AWBMODE_TUNGSTEN;
            case RASPICAM_AWB_FLUORESCENT:
                return MMAL_PARAM_AWBMODE_FLUORESCENT;
            case RASPICAM_AWB_INCANDESCENT:
                return MMAL_PARAM_AWBMODE_INCANDESCENT;
            case RASPICAM_AWB_FLASH:
                return MMAL_PARAM_AWBMODE_FLASH;
            case RASPICAM_AWB_HORIZON:
                return MMAL_PARAM_AWBMODE_HORIZON;
            default:
                return MMAL_PARAM_AWBMODE_AUTO;
            }
        }

        MMAL_PARAM_IMAGEFX_T Private_Impl::convertImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect ) {
            switch ( imageEffect ) {
            case RASPICAM_IMAGE_EFFECT_NONE:
                return MMAL_PARAM_IMAGEFX_NONE;
            case RASPICAM_IMAGE_EFFECT_NEGATIVE:
                return MMAL_PARAM_IMAGEFX_NEGATIVE;
            case RASPICAM_IMAGE_EFFECT_SOLARIZE:
                return MMAL_PARAM_IMAGEFX_SOLARIZE;
            case RASPICAM_IMAGE_EFFECT_SKETCH:
                return MMAL_PARAM_IMAGEFX_SKETCH;
            case RASPICAM_IMAGE_EFFECT_DENOISE:
                return MMAL_PARAM_IMAGEFX_DENOISE;
            case RASPICAM_IMAGE_EFFECT_EMBOSS:
                return MMAL_PARAM_IMAGEFX_EMBOSS;
            case RASPICAM_IMAGE_EFFECT_OILPAINT:
                return MMAL_PARAM_IMAGEFX_OILPAINT;
            case RASPICAM_IMAGE_EFFECT_HATCH:
                return MMAL_PARAM_IMAGEFX_HATCH;
            case RASPICAM_IMAGE_EFFECT_GPEN:
                return MMAL_PARAM_IMAGEFX_GPEN;
            case RASPICAM_IMAGE_EFFECT_PASTEL:
                return MMAL_PARAM_IMAGEFX_PASTEL;
            case RASPICAM_IMAGE_EFFECT_WATERCOLOR:
                return MMAL_PARAM_IMAGEFX_WATERCOLOUR;
            case RASPICAM_IMAGE_EFFECT_FILM:
                return MMAL_PARAM_IMAGEFX_FILM;
            case RASPICAM_IMAGE_EFFECT_BLUR:
                return MMAL_PARAM_IMAGEFX_BLUR;
            case RASPICAM_IMAGE_EFFECT_SATURATION:
                return MMAL_PARAM_IMAGEFX_SATURATION;
            case RASPICAM_IMAGE_EFFECT_COLORSWAP:
                return MMAL_PARAM_IMAGEFX_COLOURSWAP;
            case RASPICAM_IMAGE_EFFECT_WASHEDOUT:
                return MMAL_PARAM_IMAGEFX_WASHEDOUT;
            case RASPICAM_IMAGE_EFFECT_POSTERISE:
                return MMAL_PARAM_IMAGEFX_POSTERISE;
            case RASPICAM_IMAGE_EFFECT_COLORPOINT:
                return MMAL_PARAM_IMAGEFX_COLOURPOINT;
            case RASPICAM_IMAGE_EFFECT_COLORBALANCE:
                return MMAL_PARAM_IMAGEFX_COLOURBALANCE;
            case RASPICAM_IMAGE_EFFECT_CARTOON:
                return MMAL_PARAM_IMAGEFX_CARTOON;
            default:
                return MMAL_PARAM_IMAGEFX_NONE;
            }
        }

        int Private_Impl::convertFormat ( RASPICAM_FORMAT fmt ) {
            switch ( fmt ) {
            case RASPICAM_FORMAT_RGB:
                return _rgb_bgr_fixed ? MMAL_ENCODING_RGB24 : MMAL_ENCODING_BGR24;
            case RASPICAM_FORMAT_BGR:
                return _rgb_bgr_fixed ? MMAL_ENCODING_BGR24 : MMAL_ENCODING_RGB24;
            case RASPICAM_FORMAT_GRAY:
                return MMAL_ENCODING_I420;
            case RASPICAM_FORMAT_YUV420:
                return MMAL_ENCODING_I420;
            default:
                return MMAL_ENCODING_I420;
            }
        }


        //Returns an id of the camera. We assume the camera id is the one of the raspberry
        //the id is obtained using raspberry serial number obtained in /proc/cpuinfo
        string Private_Impl::getId() const{
            char serial[1024];
            serial[0]='\0';
            ifstream file ( "/proc/cpuinfo" );
            if ( !file ) {
                cerr<<__FILE__<<" "<<__LINE__<<":"<<__func__<<"Could not read /proc/cpuinfo"<<endl;
                return serial;
            }
            //read lines until find serial
            bool found=false;
            while ( !file.eof() && !found ) {
                char line[1024];
                file.getline ( line,1024 );
                string str ( line );
                char aux[100];

                if ( str.find ( "Serial" ) !=string::npos ) {
                    if ( sscanf ( line,"%s : %s",aux,serial ) !=2 ) {
                        cerr<<__FILE__<<" "<<__LINE__<<":"<<__func__<<"Error parsing /proc/cpuinfo"<<endl;
                    } else found=true;
                }
            };
            return serial;
        }

        void Private_Impl::convertBGR2RGB ( unsigned char *  in_bgr,unsigned char *  out_rgb,int size ) {
            unsigned char *end=in_bgr+size;
            unsigned char *in_ptr=in_bgr;
            while ( in_ptr<end ) {
                swap ( in_ptr[2],in_ptr[0] );
                in_ptr+=3;
            }
            mempcpy ( out_rgb,in_bgr,size );


        }
        void Private_Impl::commitAWB_RB() {
           MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};
           param.r_gain.num = (unsigned int)(State.awbg_red * 65536);
           param.b_gain.num = (unsigned int)(State.awbg_blue * 65536);
           param.r_gain.den = param.b_gain.den = 65536;
           if ( mmal_port_parameter_set(State.camera_component->control, &param.hdr) != MMAL_SUCCESS )
                cout << __func__ << ": Failed to set AWBG gains parameter.\n";
        }
    }
}
