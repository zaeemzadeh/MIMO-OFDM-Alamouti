# Install script for directory: /home/alireza/Desktop/GNU/gr-mimo/grc

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_alamouti_encode_cc.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_alamouti_decode_cc.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_preamble_detector.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_alamouti_ofdm_tx_cc.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_alamouti_ofdm_tx_cc.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_ofdm_mapper.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_ofdm_mapper.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_alamouti_ofdm_rx.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_ofdm_demapper.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_ofdm_demapper.xml"
    "/home/alireza/Desktop/GNU/gr-mimo/grc/mimo_ofdm_insert_preamble_tagged.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

