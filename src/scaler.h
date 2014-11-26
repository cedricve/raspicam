#ifndef _RASPICAM_SCALER_H_
#define _RASPICAM_SCALER_H_
namespace raspicam {

    /**CLass to calculate linear scale
     */
    class   Scaler
    {
        float a,b;
        float _inMin, _inMax, _outMin,_outMax;
        public:
        float aa;
        float bb;
        Scaler() {}
        Scaler ( float inMin, float inMax, float outMin,float outMax ) {
            setParams ( inMin,inMax,outMin,outMax );
        }


        void setParams ( float inMin, float inMax, float outMin,float outMax ) {
            _inMin=inMin;
            _inMax=inMax;
            _outMin=outMin;
            _outMax=outMax;
            double aux = ( _inMax - _inMin );
            if ( aux != 0.0 ) {
                a = ( _outMax - _outMin ) /aux;
                b = _outMax - ( a * _inMax );
            } else
                a = b = 0.0;
            aa = ( _outMax - _outMin ) / ( _inMax - _inMin );
            bb= ( _outMax - ( aa * _inMax ) );
        }
        inline float operator() ( float val ) const
        {
            if ( val<=_inMin ) return _outMin;
            else if ( val>=_inMax ) return  _outMax;
            return val * aa + bb;
        }

        static float scale ( float inMin, float inMax, float outMin,float outMax,float val ) {
            Scaler s ( inMin,inMax,outMin,outMax );
            return s ( val );
        }
    };
}
#endif
