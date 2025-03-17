cmake_minimum_required(VERSION 3.22)
project(moveit_drake LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH /opt/drake/lib)
find_package(drake CONFIG REQUIRED PATHS /opt/drake)

find_package(ament_cmake REQUIRED)

# Common cmake code applied to all moveit packages
find_package(moveit_common REQUIRED)
moveit_package()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(class_loader REQUIRED)
find_package(generate_parameter_library REQUIRED)
find_package(moveit_core REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(moveit_ros_planning REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_visual_tools REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rviz_visual_tools REQUIRED)
find_package(warehouse_ros REQUIRED)
find_package(shape_msgs REQUIRED)
find_package(ros2_control REQUIRED)
find_package(controller_interface REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

generate_parameter_library(ktopt_moveit_parameters
                           parameters/ktopt_moveit_parameters.yaml)

set(THIS_PACKAGE_INCLUDE_DEPENDS
    ament_cmake
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    moveit_visual_tools
    rclcpp
    rviz_visual_tools
    shape_msgs
    warehouse_ros)

include_directories(include)

# ktopt planning plugin
add_library(
  moveit_drake SHARED
  # Kinematic Trajectory Optimization (KTOpt)
  src/ktopt_planner_manager.cpp
  src/ktopt_planning_context.cpp
  # TOPPRA
  src/add_toppra_time_parameterization.cpp
  src/joint_stiffness_controller.cpp
  # Conversions
  src/conversions.cpp)

ament_target_dependencies(
  moveit_drake
  EIGEN3
  moveit_core
  moveit_msgs
  pluginlib
  rclcpp
  shape_msgs
  rclcpp
  controller_interface
  hardware_interface
  std_msgs
  ros2_control)
target_link_libraries(moveit_drake drake::drake ktopt_moveit_parameters)

# Ensure that the plugin finds libdrake.so at runtime
set_target_properties(moveit_drake PROPERTIES INSTALL_RPATH "/opt/drake/lib"
                                              BUILD_RPATH "/opt/drake/lib")

pluginlib_export_plugin_description_file(moveit_core plugin_descriptions.xml)

install(
  TARGETS moveit_drake ktopt_moveit_parameters
  EXPORT moveit_drakeTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include/drake)

ament_export_targets(moveit_drakeTargets HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS} drake
                          generate_parameter_library)

add_subdirectory(demo)

ament_package()
