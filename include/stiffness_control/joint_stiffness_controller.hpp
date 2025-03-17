#pragma once

#include <rclcpp/rclcpp.hpp>
#include <controller_interface/controller_interface.hpp>
#include <hardware_interface/loaned_command_interface.hpp>
#include <hardware_interface/loaned_state_interface.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

namespace joint_stiffness_controller
{

class JointStiffnessController : public controller_interface::ControllerInterface
{
public:
  JointStiffnessController();

  controller_interface::InterfaceConfiguration command_interface_configuration() const override;
  controller_interface::InterfaceConfiguration state_interface_configuration() const override;
  controller_interface::return_type update(const rclcpp::Time&, const rclcpp::Duration&) override;
  controller_interface::CallbackReturn on_init() override;
  controller_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State& state) override;

private:
  std::vector<hardware_interface::LoanedCommandInterface> position_cmd_handles_;
  std::vector<hardware_interface::LoanedStateInterface> position_state_handles_;

  rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr command_sub_;
  std::vector<double> desired_positions_;
  std::vector<double> stiffness_gains_;

  void command_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
};

}  // namespace joint_stiffness_controller
