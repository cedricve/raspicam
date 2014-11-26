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
****************************************************************/#include <iostream>
#include <fstream>
#include <cstdlib>
#include "raspicam_still.h"
using namespace std;

raspicam::RaspiCam_Still Camera;
//Returns the value of a param. If not present, returns the defvalue
float getParamVal ( string id,int argc,char **argv,float defvalue ) {
    for ( int i=0; i<argc; i++ )
        if ( id== argv[i] )
            return atof ( argv[i+1] );
    return defvalue;
}


//prints program command line usage
void usage() {
    cout<<"-w val : sets image width (2592 default)"<<endl;
    cout<<"-h val : sets image height (1944 default)"<<endl;
     cout<<"-iso val: set iso [100,800] (400 default)"<<endl;
}


int main ( int argc, char *argv[] ) {
    usage();

    int width = getParamVal ( "-w",argc,argv,2592 );
    int height =getParamVal ( "-h",argc,argv,1944 );
    int iso=getParamVal ( "-iso",argc,argv,400);


    cout << "Initializing ..."<<width<<"x"<<height<<endl;
    Camera.setWidth ( width );
    Camera.setHeight ( height );
    Camera.setISO(iso);
    Camera.setEncoding ( raspicam::RASPICAM_ENCODING_BMP );
    Camera.open();
    cout<<"capture"<<endl;
    unsigned int length = Camera.getImageBufferSize(); // Header + Image Data + Padding
    unsigned char * data = new unsigned char[length];
      if ( !Camera.grab_retrieve(data, length) ) {
        cerr<<"Error in grab"<<endl;
        return -1;
    }

    cout<<"saving picture.bmp"<<endl;
    ofstream file ( "picture.bmp",ios::binary );
    file.write ( ( char* ) data,   length );
    return 0;
}

