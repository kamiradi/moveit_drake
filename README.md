# Experimental MoveIt 2 - Drake Integration

> [!NOTE]
> Experimental and will continue to have breaking changes until first release.

`moveit_drake` brings together the vertical ROS integration of the [MoveIt 2](https://moveit.ai/) motion planning framework, with the Mathematical Programming interface within [Drake](https://drake.mit.edu/).
This allows the user to setup motion planning as an optimization problem within ROS, with the rich specification of constraints and costs provided by `drake`.


## Features

- Exposes [`KinematicTrajectoryOptimization`](https://drake.mit.edu/doxygen_cxx/classdrake_1_1planning_1_1trajectory__optimization_1_1_kinematic_trajectory_optimization.html) implementation in `drake` as a motion planner.
- Exposes [`TOPPRA`](https://drake.mit.edu/doxygen_cxx/classdrake_1_1multibody_1_1_toppra.html) implementation in `drake` as a trajectory post-processing adapter.

## Docker Workflow (Preferred and tested)

### Requirements
Docker and Docker Compose - Follow the instructions [here](https://docs.docker.com/engine/install/ubuntu/).
Also, run the [Linux post-installation steps](https://docs.docker.com/engine/install/linux-postinstall/).

### Steps
The following steps clone and build the base image that you will require to test/build/run/develop with the repo.

```bash
git clone https://github.com/moveit/moveit_drake.git
cd moveit_drake
docker compose build
```

This should give you an image with `drake` and `moveit2`.
Next, create a container with the following and create shell access.

```bash
docker compose up
docker compose exec -it moveit_drake bash
```

Follow [instructions](#build-moveit_drake) below to build `moveit_drake`


## Local Installation

### Install Drake

[Follow these instructions](https://drake.mit.edu/installation.html)

### Build `moveit_drake`

Follow the [MoveIt Source Build](https://moveit.ros.org/install-moveit2/source/) instructions to set up a ROS 2 workspace with MoveIt from source.

Open a command line and navigate to your workspace:

```bash
cd ${WORKSPACE}/src
```

Clone this repo to your workspace, including upstream dependencies:

```bash
git clone https://github.com/moveit/moveit_drake.git
vcs import < moveit_drake/moveit_drake.repos
rosdep install -r --from-paths . --ignore-src --rosdistro ${ROS_DISTRO} -y
```

Configure and build the workspace (this will take some time, as it builds MoveIt):

```bash
cd ${WORKSPACE}
colcon build --event-handlers desktop_notification- status- --cmake-args -DCMAKE_BUILD_TYPE=Release --parallel-workers 1
```


## Examples

The planning pipeline testbench compares `moveit_drake` planners with existing MoveIt planners such as OMPL and Pilz.

```bash
ros2 launch moveit_drake pipeline_testbench.launch.py
```

This interactive example shows constrained planning using the Drake KTOpt planner.

```bash
ros2 launch moveit_drake constrained_planning_demo.launch.py
```


## Development

### Formatting

We use [pre-commit](https://moveit.ros.org/documentation/contributing/code/#pre-commit-formatting-checks) to format the code in this repo.

Within the container, you can run the following command to format the code:

```bash
cd src/moveit_drake
pre-commit run -a
```

### Some helper commands
To rebuild only the `moveit_drake` package:

```bash
rm -rf build/moveit_drake install/moveit_drake
colcon build --packages-select moveit_drake
```
