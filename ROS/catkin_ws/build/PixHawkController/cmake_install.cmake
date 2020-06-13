# Install script for directory: /home/deepak/catkin_ws/src/PixHawkController

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/deepak/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixhwk_ctrl_node/msg" TYPE FILE FILES
    "/home/deepak/catkin_ws/src/PixHawkController/msg/OmplStatus.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/PixHawk_Goal.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/PixHawk_SteeringControl.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/PixHawk_ThrottleControl.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/Lanes.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/traffic_light.msg"
    "/home/deepak/catkin_ws/src/PixHawkController/msg/desired_velocity.msg"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixhwk_ctrl_node/cmake" TYPE FILE FILES "/home/deepak/catkin_ws/build/PixHawkController/catkin_generated/installspace/pixhwk_ctrl_node-msg-paths.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/deepak/catkin_ws/devel/include/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/deepak/catkin_ws/devel/share/roseus/ros/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/deepak/catkin_ws/devel/share/common-lisp/ros/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/deepak/catkin_ws/devel/share/gennodejs/ros/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND "/usr/bin/python" -m compileall "/home/deepak/catkin_ws/devel/lib/python2.7/dist-packages/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/deepak/catkin_ws/devel/lib/python2.7/dist-packages/pixhwk_ctrl_node")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/deepak/catkin_ws/build/PixHawkController/catkin_generated/installspace/pixhwk_ctrl_node.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixhwk_ctrl_node/cmake" TYPE FILE FILES "/home/deepak/catkin_ws/build/PixHawkController/catkin_generated/installspace/pixhwk_ctrl_node-msg-extras.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixhwk_ctrl_node/cmake" TYPE FILE FILES
    "/home/deepak/catkin_ws/build/PixHawkController/catkin_generated/installspace/pixhwk_ctrl_nodeConfig.cmake"
    "/home/deepak/catkin_ws/build/PixHawkController/catkin_generated/installspace/pixhwk_ctrl_nodeConfig-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixhwk_ctrl_node" TYPE FILE FILES "/home/deepak/catkin_ws/src/PixHawkController/package.xml")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node" TYPE EXECUTABLE FILES "/home/deepak/catkin_ws/devel/lib/pixhwk_ctrl_node/pixhawkctrlNode")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode"
         OLD_RPATH "/opt/ros/kinetic/lib:/usr/lib/x86_64-linux-gnu/hdf5/serial/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/pixhwk_ctrl_node/pixhawkctrlNode")
    endif()
  endif()
endif()

