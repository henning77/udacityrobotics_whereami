#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget req;
    req.request.linear_x = lin_x;
    req.request.angular_z = ang_z;

    if (!client.call(req)) {
        ROS_ERROR("Failed to call service DriveToTarget");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    // Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

    bool white_pixel_found = false;

    // Image segmented in 5 segments
    // -2=far left, -1=left, 0=mid, 1=right, 2=far right
    int white_pixel_pos = 0;
    int segment_width = img.width / 5;

    for (int row = 0; row < img.height; row++) {
        for (int col = 0; col < img.step; col+=3) {
            // Check that r/g/b are all 255
            if (img.data[row * img.step + col] == 255
                && img.data[row * img.step + col + 1] == 255
                && img.data[row * img.step + col + 2] == 255) {
                white_pixel_found = true;
                white_pixel_pos = ( (col/3) / segment_width) - 2;

                ROS_INFO_STREAM("Found white: row=" + std::to_string(row) + " col=" + std::to_string(col) + " segment=" + std::to_string(white_pixel_pos));
                break;
            }
        }
    }

    if (white_pixel_found) {        
        drive_robot(0.5, -white_pixel_pos);
    } else {
        drive_robot(0, 0);
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}