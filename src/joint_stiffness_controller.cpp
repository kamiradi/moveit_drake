#include "stiffness_control/joint_stiffness_controller.hpp"

namespace joint_stiffness_controller
{

JointStiffnessController::JointStiffnessController()
{
}

controller_interface::CallbackReturn JointStiffnessController::on_init()
{
  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn JointStiffnessController::on_configure(const rclcpp_lifecycle::State&)
{
  auto node = get_node();
  command_sub_ = node->create_subscription<std_msgs::msg::Float64MultiArray>(
      "~/command", 10, std::bind(&JointStiffnessController::command_callback, this, std::placeholders::_1));
  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::InterfaceConfiguration JointStiffnessController::command_interface_configuration() const
{
  return {
    controller_interface::interface_configuration_type::INDIVIDUAL,
    { "joint1/position", "joint2/position" }  // replace with your actual joints
  };
}

controller_interface::InterfaceConfiguration JointStiffnessController::state_interface_configuration() const
{
  return {
    controller_interface::interface_configuration_type::INDIVIDUAL,
    { "joint1/position", "joint2/position" }  // same joints
  };
}

void JointStiffnessController::command_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
  size_t n = msg->data.size() / 2;
  desired_positions_.resize(n);
  stiffness_gains_.resize(n);
  for (size_t i = 0; i < n; ++i)
  {
    desired_positions_[i] = msg->data[i];
    stiffness_gains_[i] = msg->data[i + n];
  }
}

controller_interface::return_type JointStiffnessController::update(const rclcpp::Time&, const rclcpp::Duration&)
{
  for (size_t i = 0; i < position_cmd_handles_.size(); ++i)
  {
    double error = desired_positions_[i] - position_state_handles_[i].get_optional().value_or(0.0);
    double command = stiffness_gains_[i] * error;
    bool success =
        position_cmd_handles_[i].set_value(position_state_handles_[i].get_optional().value_or(0.0) + command);
    if (!success)
    {
      RCLCPP_WARN(get_node()->get_logger(), "Failed to set command for joint %zu", i);
    }
  }
  return controller_interface::return_type::OK;
}

}  // namespace joint_stiffness_controller

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(joint_stiffness_controller::JointStiffnessController, controller_interface::ControllerInterface)
