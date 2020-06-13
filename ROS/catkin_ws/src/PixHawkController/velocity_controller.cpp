#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <pixhwk_ctrl_node/desired_velocity.h>
#include <pixhwk_ctrl_node/traffic_light.h>
#include <can/e2o_status.h>
#include <algorithm>                // std::sort and std::find

ros::Publisher pub_des_vel;

int curr_priority = -100;
int set_velocity = 0;
int curr_velocity = 0;
int prev_node_id = -1;

struct Velocity_msg
{
    double velocity;
    int priority;
    int node_id;

    // bool operator==(const int id)const
    // {
    //     return node_id == id;
    // }

    friend bool operator==(const Velocity_msg& lhs,const Velocity_msg& rhs)
    {
        return lhs.node_id == rhs.node_id;
    }

} velocity_msg_temp;

// struct find_Node_id : std::unary_function<Velocity_msg, bool> 
// {
//     int node_id;
//     find_Node_id(int node_id):Node_id(Node_id) 
//     { }
//     bool operator()(Velocity_msg const& m) const 
//     {
//         return m.Node_id == Node_id;
//     }
// };

std::vector<Velocity_msg> velocity_msg_queue;


bool priority_compare (Velocity_msg vel_a, Velocity_msg vel_b) 
{           
    return vel_a.priority < vel_b.priority; 
}

void desired_velocity_callback(pixhwk_ctrl_node::desired_velocity msg) 
{
    std::vector<Velocity_msg>::iterator it;
    // ROS_INFO("%f    %d    %d", msg.velocity, msg.priority, set_velocity);
    velocity_msg_temp.velocity = msg.velocity;
    velocity_msg_temp.priority = msg.priority;
    velocity_msg_temp.node_id = msg.node_id;
    // std::cout<<"\nincoming message\n"<<"vel = "<<velocity_msg_temp.velocity<<"    priority = "<<velocity_msg_temp.priority<<"      id = "<<velocity_msg_temp.node_id<<std::endl;
    int id = msg.node_id;

    if(velocity_msg_queue.empty())              //if queue is empty simply perform push_back()
    {
        velocity_msg_queue.push_back(velocity_msg_temp);
    }

    else
    {
        // it = std::find_if(velocity_msg_queue.begin(), velocity_msg_queue.end(), velocity_msg_temp);      //searching for an element with priority = msg.priority
        // if(it != velocity_msg_queue.end())              //element found
        //     {
                //replacing the element with new values
                {
                velocity_msg_queue.erase(std::remove(velocity_msg_queue.begin(),velocity_msg_queue.end(),velocity_msg_temp),velocity_msg_queue.end());
                std::cout<<" removed element\n";
                }
                // velocity_msg_queue.erase(velocity_msg_queue.begin()+it);
                velocity_msg_queue.push_back(velocity_msg_temp);
        //     }

        // else                                            //element not found
        //     {
        //         //simply adding the element at the end of the queue
        //         velocity_msg_queue.push_back(velocity_msg_temp);
        //     }
    }
    std::cout<<"*******************************inside print loop***************************************\n"<<std::endl;
    for(it = velocity_msg_queue.begin(); it != velocity_msg_queue.end(); ++it)
    {
        std::cout<<"\tvel = "<<(*it).velocity<<" priority = "<<(*it).priority<<" id = "<<(*it).node_id<<std::endl;
    }
    std::cout<<"\n***************************************************************************************\n"<<std::endl;

            
}

void e2o_callback(const can::e2o_status::ConstPtr& e2o_info)
{
    // currvel = velocity->data;
    curr_velocity = e2o_info->velocity;
    // if the set velocity is reached then reset priority.
    if (curr_velocity == set_velocity) {
        curr_priority = 0;
    }
    // curr_time = ros::Time::now().toSec();
    // set_distance += (curr_time - last_time)*curr_vel*5/18;
    // last_time=curr_time;
    // cout<<""set_distance<<'\n';
    // cout<<currvel<<'\n';
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "velocity_controller");
    ros::NodeHandle nh;
    ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
    // std::vector<Velocity_msg>::iterator n;

    ros::Subscriber desired_velocity_sub = nh.subscribe<pixhwk_ctrl_node::desired_velocity>("/set_desired_velocity", 1, desired_velocity_callback);
    // get the speed of the car
    ros::Subscriber vel_sub = nh.subscribe<can::e2o_status>("/can_node/e2o_info", 1, e2o_callback, noDelay);

    
    pub_des_vel = nh.advertise<std_msgs::Float64>("/desired_velocity", 1);
    std_msgs::Float64 des_vel_msg;
    while(ros::ok())
    {
        int p;
        std::sort(velocity_msg_queue.begin(), velocity_msg_queue.end(),priority_compare);
        

        // if new task more important
        if(!velocity_msg_queue.empty())
        {
            //if id's are same > pub directly | else if id's are diff > compare priorities;
            p = velocity_msg_queue.back().priority;
            std::cout<<"\t\tprev_node_id = "<<prev_node_id<<std::endl;
            std::cout<<"\t\tcurr_node_id = "<<velocity_msg_queue.back().node_id<<std::endl;
            
            if(prev_node_id != velocity_msg_queue.back().node_id)
            {
                if (p >= curr_priority) 
                {
                    // std::cout<<"yo 6"<<std::endl;
                    // change set velocity
                    set_velocity = velocity_msg_queue.back().velocity;
                    // change curr_priority to not accept any lower priority task
                    curr_priority = velocity_msg_queue.back().priority;
                }
                
                else 
                {
                    ROS_INFO("Priority %d is lower than current: %d", velocity_msg_queue.back().priority, curr_priority);
                }
                
                des_vel_msg.data = set_velocity;
                pub_des_vel.publish(des_vel_msg);
                prev_node_id = velocity_msg_queue.back().node_id;
            }

            else if(prev_node_id == velocity_msg_queue.back().node_id)
            {
                set_velocity = velocity_msg_queue.back().velocity;
                curr_priority = velocity_msg_queue.back().priority;
                des_vel_msg.data = set_velocity;
                pub_des_vel.publish(des_vel_msg);
            }
        }
        else
            {
                std::cout<<" Empty queue"<<std::endl;
            }
        
        ros::spinOnce();
        
    }

    return 0;
}