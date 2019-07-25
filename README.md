# RaspiCam: C++ API for using Raspberry camera (with OpenCV)

This library allows to use the Raspberry Pi Camera under BSD License. 

The project is started by the AVA research group (rafael Muñoz Salinas; rmsalinas@uco.es) and is now maintained
on this GIT repository by Cédric Verstraeten. Please note that is NOT the OFFICIAL repository of RaspiCam, these can be found [here](http://www.uco.es/investiga/grupos/ava/node/40).

This repository is used in the [Kerberos.io](https://github.com/kerberos-io) project.

## Release notes

Update 2014/03/04: version 0.1.1 released Download at SourceForge
Main feaure in 0.0.7 : Still camera API. You can now use the still mode for high resolution (includes OpenCv interface). See examples for more info.
 
Notes:
Requires to update the firmware to use shutterspeed (sudo rpi-update)
 
Main features
 - Provides  class RaspiCam for easy and full control of the camera
 - Provides class  RaspiCam_Still and RaspiCam_Still_Cv for controlling the camera in still mode
 - Provides class  RaspiCam_Cv for easy control of the camera with OpenCV.
 - Provides class  RaspiCam_Still and RaspiCam_Still_Cv for controlling the camera in still mode
 - Provides class RaspiCam_Still and RaspiCam_Still_Cv for using the still camera mode
 - Easy compilation/installation using cmake.
 - No need to install development file of userland. Implementation is hidden.
 - Many examples 
 
Performance
Following, we show the capture performance of the library (measured capturing 600 frames). 
Gray and YUV420 Mode
   - 1280x960: 29.5fps,  640x480 : 29.5fps,  320x240 : 29.5fps
RGB Mode
   - 1280x960: 28 fps,  640x480 : 29.29fps,  320x240 : 29.24fps
BGR Mode
   - 1280x960: 14 fps,  640x480 : 29.29fps,  320x240 : 29.24fps
 
Note: when using the full resolution video callbacks with the full resolution of the Raspberry Pi Camera v2, you will likely get an error such as `mmal: mmal_vc_port_enable: failed to enable port vc.ril.camera:out:1(BGR3): ENOSPC`. In order to fix this increase your GPU memory to at least 256MB.

Color conversion is the most time consuming part. We still need to improve that part. Go to src/private and check if you can contribute!
 
Note: the library is compiled with the options: -Wall -ffunction-sections  -fomit-frame-pointer -O2 -ffast-math -DNDEBUG -mcpu=arm1176jzf-s  -mfpu=vfp -mfloat-abi=hard -ftree-vectorize
Note 2: the library is currently setting the camera in video mode. So, maximum resolution is 1280x960. I am working on the still port to enable higher resolutions.
 
## Compiling

Clone the repository to your raspberry. Then, uncompress the file and compile
 
	git clone https://github.com/cedricve/raspicam.git
	cd raspicam
	mkdir build
	cd build
	cmake ..
 
At this point you'll see something like 

	-- CREATE OPENCV MODULE=1
	-- CMAKE_INSTALL_PREFIX=/usr/local
	-- REQUIRED_LIBRARIES=/opt/vc/lib/libmmal_core.so;/opt/vc/lib/libmmal_util.so;/opt/vc/lib/libmmal.so
	-- Change a value with: cmake -D<Variable>=<Value>
	-- 
	-- Configuring done
	-- Generating done
	-- Build files have been written to: /home/pi/raspicam/trunk/build
 
If OpenCV development files are installed in your system, then you see following output; otherwise this option will be 0 and the opencv module of the library will not be compiled.

	-- CREATE OPENCV MODULE=1
	
 
Finally compile, install and update the ldconfig:

	make
	sudo make install
	sudo ldconfig
 
After that, you have the programs raspicam_test  and raspicam_cv_test (if opencv was enabled) in build/utils.
Run the first program to check that compilation is ok.
```
sudo ./raspicam_test
sudo ./raspicam_cv_test
```
 
### Using it in your projects
 
You can learn how to use the library by taking a look at the examples in the utils directory and  by analyzing the header files. In addition, we  provide a some simple examples on how to use the library with cmake.
 First, create a directory for our own project. Then, go in and create a file with the name simpletest_raspicam.cpp and add the following code
 
	/**
	*/
	#include <ctime>
	#include <fstream>
	#include <iostream>
	#include <raspicam/raspicam.h>
	using namespace std;
	 
	int main ( int argc,char **argv ) {
		raspicam::RaspiCam Camera; //Camera object
		//Open camera 
		cout<<"Opening Camera..."<<endl;
		if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
		//wait a while until camera stabilizes
		cout<<"Sleeping for 3 secs"<<endl;
		sleep(3);
		//capture
		Camera.grab();
		//allocate memory
		unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
		//extract the image in rgb format
		Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image
		//save
		std::ofstream outFile ( "raspicam_image.ppm",std::ios::binary );
		outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
		outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
		cout<<"Image saved at raspicam_image.ppm"<<endl;
		//free resrources    
		delete data;
		return 0;
	}
 
For cmake users,  create a file named CMakeLists.txt and add:

	cmake_minimum_required (VERSION 2.8) 
	project (raspicam_test)
	find_package(raspicam REQUIRED)
	add_executable (simpletest_raspicam simpletest_raspicam.cpp)  
	target_link_libraries (simpletest_raspicam ${raspicam_LIBS})

 
Finally, create build dir,compile and execute

	mkdir build
	cd build
	cmake ..
	make
	./simpletest_raspicam
 
If you do not like cmake, simply

	g++ simpletest_raspicam.cpp -o simpletest_raspicam -I/usr/local/include -lraspicam -lmmal -lmmal_core -lmmal_util
 
### OpenCV Interface
 
If the OpenCV is found when compiling the library, the libraspicam_cv.so module is created and the RaspiCam_Cv class available. Take a look at the examples in utils to see how to use the class. In addition, we show here how you can use the RaspiCam_Cv in your own project using cmake.
 
First create a file with the name simpletest_raspicam_cv.cpp and add the following code
 
	#include <ctime>
	#include <iostream>
	#include <raspicam/raspicam_cv.h>
	using namespace std; 
	 
	int main ( int argc,char **argv ) {
	   
		time_t timer_begin,timer_end;
		raspicam::RaspiCam_Cv Camera;
		cv::Mat image;
		int nCount=100;
		//set camera params
		Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
		//Open camera
		cout<<"Opening Camera..."<<endl;
		if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
		//Start capture
		cout<<"Capturing "<<nCount<<" frames ...."<<endl;
		time ( &timer_begin );
		for ( int i=0; i<nCount; i++ ) {
			Camera.grab();
			Camera.retrieve ( image);
			if ( i%5==0 )  cout<<"\r captured "<<i<<" images"<<std::flush;
		}
		cout<<"Stop camera..."<<endl;
		Camera.release();
		//show time statistics
		time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
		double secondsElapsed = difftime ( timer_end,timer_begin );
		cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
		//save image 
		cv::imwrite("raspicam_cv_image.jpg",image);
		cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
	}
 
For cmake users, create a file named CMakeLists.txt and add:

	cmake_minimum_required (VERSION 2.8) 
	project (raspicam_test)
	find_package(raspicam REQUIRED)
	find_package(OpenCV)
	IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (simpletest_raspicam_cv simpletest_raspicam_cv.cpp)  
	target_link_libraries (simpletest_raspicam_cv ${raspicam_CV_LIBS})
	ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
	ENDIF()

Finally, create,compile and execute

	mkdir build
	cd build
	cmake ..
	make
	./simpletest_raspicam_cv
	 
 
If you do not like cmake:

	g++ simpletest_raspicam_cv.cpp -o  simpletest_raspicam_cv -I/usr/local/include/ -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui 
	
