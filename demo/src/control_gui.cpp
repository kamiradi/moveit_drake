#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QLabel>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

class StiffnessGUI : public QWidget
{
  Q_OBJECT

public:
  StiffnessGUI(std::shared_ptr<rclcpp::Node> node) : node_(node)
  {
    publisher_ = node_->create_publisher<std_msgs::msg::Float64MultiArray>("/joint_stiffness_controller/command", 10);

    auto* layout = new QVBoxLayout(this);

    pos_spin1 = new QDoubleSpinBox();
    pos_spin2 = new QDoubleSpinBox();
    stiff_spin1 = new QDoubleSpinBox();
    stiff_spin2 = new QDoubleSpinBox();

    layout->addWidget(new QLabel("Position Joint 1"));
    layout->addWidget(pos_spin1);
    layout->addWidget(new QLabel("Stiffness Joint 1"));
    layout->addWidget(stiff_spin1);
    layout->addWidget(new QLabel("Position Joint 2"));
    layout->addWidget(pos_spin2);
    layout->addWidget(new QLabel("Stiffness Joint 2"));
    layout->addWidget(stiff_spin2);

    auto* send_btn = new QPushButton("Send Command");
    layout->addWidget(send_btn);
    connect(send_btn, &QPushButton::clicked, this, &StiffnessGUI::send_command);
  }

private slots:
  void send_command()
  {
    auto msg = std_msgs::msg::Float64MultiArray();
    msg.data = { pos_spin1->value(), pos_spin2->value(), stiff_spin1->value(), stiff_spin2->value() };
    publisher_->publish(msg);
  }

private:
  std::shared_ptr<rclcpp::Node> node_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher_;

  QDoubleSpinBox* pos_spin1;
  QDoubleSpinBox* pos_spin2;
  QDoubleSpinBox* stiff_spin1;
  QDoubleSpinBox* stiff_spin2;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  QApplication app(argc, argv);

  auto node = rclcpp::Node::make_shared("stiffness_gui");
  StiffnessGUI gui(node);
  gui.show();

  std::thread spin_thread([&]() { rclcpp::spin(node); });

  int ret = app.exec();
  rclcpp::shutdown();
  spin_thread.join();
  return ret;
}
#include "control_gui.moc"
