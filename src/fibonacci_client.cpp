#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <learning_actionlib/FibonacciAction.h>
#include <iostream>

void activeCB(){
  ROS_INFO("Goal just went active");
}

void feedbackCB(const learning_actionlib::FibonacciFeedbackConstPtr &feedback){
  for(int i=0, n=feedback->sequence.size(); i<n; ++i){
    std::cout<< feedback->sequence[i] << ", ";
  }
  std::cout<< std::endl;
}

void doneCB(const actionlib::SimpleClientGoalState &state, const learning_actionlib::FibonacciResultConstPtr &result){
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  ROS_INFO("Answer: %i", result->sequence.back());
}

int main (int argc, char **argv)
{
  ros::init(argc, argv, "test_fibonacci");

  // create the action client
  // true causes the client to spin it's own thread
  actionlib::SimpleActionClient<learning_actionlib::FibonacciAction> ac("fibonacci", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  learning_actionlib::FibonacciGoal goal;
  goal.order = 20;
  ac.sendGoal(goal, &doneCB, &activeCB, &feedbackCB);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}
