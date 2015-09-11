# Install script for directory: /home/alireza/Desktop/GNU/gr-mimo/include/mimo

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mimo" TYPE FILE FILES
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/api.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/alamouti_encode_cc.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/alamouti_decode_cc.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/preamble_detector.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/alamouti_ofdm_tx_cc.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/alamouti_ofdm_tx_cc.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/ofdm_mapper.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/ofdm_mapper.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/alamouti_ofdm_rx.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/ofdm_demapper.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/ofdm_demapper.h"
    "/home/alireza/Desktop/GNU/gr-mimo/include/mimo/ofdm_insert_preamble_tagged.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

