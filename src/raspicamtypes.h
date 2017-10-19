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

#ifndef _RASPICAM_TYPES_H
#define _RASPICAM_TYPES_H

namespace raspicam {

    /**Image formats
     */
    enum RASPICAM_FORMAT{
        RASPICAM_FORMAT_YUV420,
        RASPICAM_FORMAT_GRAY,
        RASPICAM_FORMAT_BGR,
        RASPICAM_FORMAT_RGB,
        RASPICAM_FORMAT_IGNORE //do not use
    };

    /**Exposure types
     */
    enum RASPICAM_EXPOSURE {
        RASPICAM_EXPOSURE_OFF,
        RASPICAM_EXPOSURE_AUTO,
        RASPICAM_EXPOSURE_NIGHT,
        RASPICAM_EXPOSURE_NIGHTPREVIEW,
        RASPICAM_EXPOSURE_BACKLIGHT,
        RASPICAM_EXPOSURE_SPOTLIGHT,
        RASPICAM_EXPOSURE_SPORTS,
        RASPICAM_EXPOSURE_SNOW,
        RASPICAM_EXPOSURE_BEACH,
        RASPICAM_EXPOSURE_VERYLONG,
        RASPICAM_EXPOSURE_FIXEDFPS,
        RASPICAM_EXPOSURE_ANTISHAKE,
        RASPICAM_EXPOSURE_FIREWORKS
    }  ;

    /**Auto white balance types
     */
    enum RASPICAM_AWB {
        RASPICAM_AWB_OFF,
        RASPICAM_AWB_AUTO,
        RASPICAM_AWB_SUNLIGHT,
        RASPICAM_AWB_CLOUDY,
        RASPICAM_AWB_SHADE,
        RASPICAM_AWB_TUNGSTEN,
        RASPICAM_AWB_FLUORESCENT,
        RASPICAM_AWB_INCANDESCENT,
        RASPICAM_AWB_FLASH,
        RASPICAM_AWB_HORIZON
    }  ;

    /**Image effects
     */
    enum RASPICAM_IMAGE_EFFECT {
        RASPICAM_IMAGE_EFFECT_NONE,
        RASPICAM_IMAGE_EFFECT_NEGATIVE,
        RASPICAM_IMAGE_EFFECT_SOLARIZE,
        RASPICAM_IMAGE_EFFECT_SKETCH,
        RASPICAM_IMAGE_EFFECT_DENOISE,
        RASPICAM_IMAGE_EFFECT_EMBOSS,
        RASPICAM_IMAGE_EFFECT_OILPAINT,
        RASPICAM_IMAGE_EFFECT_HATCH,
        RASPICAM_IMAGE_EFFECT_GPEN,
        RASPICAM_IMAGE_EFFECT_PASTEL,
        RASPICAM_IMAGE_EFFECT_WATERCOLOR,
        RASPICAM_IMAGE_EFFECT_FILM,
        RASPICAM_IMAGE_EFFECT_BLUR,
        RASPICAM_IMAGE_EFFECT_SATURATION,
        RASPICAM_IMAGE_EFFECT_COLORSWAP,
        RASPICAM_IMAGE_EFFECT_WASHEDOUT,
        RASPICAM_IMAGE_EFFECT_POSTERISE,
        RASPICAM_IMAGE_EFFECT_COLORPOINT,
        RASPICAM_IMAGE_EFFECT_COLORBALANCE,
        RASPICAM_IMAGE_EFFECT_CARTOON
    }  ;

    /**Metering types
     */
    enum RASPICAM_METERING {
        RASPICAM_METERING_AVERAGE,
        RASPICAM_METERING_SPOT,
        RASPICAM_METERING_BACKLIT,
        RASPICAM_METERING_MATRIX
    }  ;
    /*Econdig modes (for still mode)
     */

    typedef enum RASPICAM_ENCODING {
        RASPICAM_ENCODING_JPEG,
        RASPICAM_ENCODING_BMP,
        RASPICAM_ENCODING_GIF,
        RASPICAM_ENCODING_PNG,
        RASPICAM_ENCODING_RGB
    } RASPICAM_ENCODING;

}
#endif

