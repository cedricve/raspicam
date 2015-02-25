INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_LIBRARIES(${PROJECT_NAME} )

 
ADD_EXECUTABLE(raspicam_test raspicam_test.cpp )
INSTALL(TARGETS raspicam_test   RUNTIME DESTINATION bin)
 
ADD_EXECUTABLE(raspicam_still_test raspicam_still_test.cpp )
INSTALL(TARGETS raspicam_still_test   RUNTIME DESTINATION bin)

IF (OpenCV_FOUND)
ADD_EXECUTABLE(raspicam_cv_test raspicam_cv_test.cpp )
target_link_libraries(raspicam_cv_test  ${PROJECT_NAME}_cv)
ADD_EXECUTABLE(raspicam_cv_still_test raspicam_cv_still_test.cpp )
target_link_libraries(raspicam_cv_still_test ${PROJECT_NAME}_cv)
INSTALL(TARGETS raspicam_cv_test  raspicam_cv_still_test RUNTIME DESTINATION bin)

ENDIF()
