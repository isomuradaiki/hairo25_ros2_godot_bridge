#include "ros2_subscriber.h"

ROS2Subscriber::ROS2Subscriber(std::shared_ptr<rclcpp::Node> node,
                const std::string &topic_name,
                ROS_MSG_TYPE msg_type,
                std::function<void(const std::string&)> cb_str,
                std::function<void(int)> cb_int,
                Callable cb_image,
                std::function<void(PackedInt32Array)> cb_int16_array,
                std::function<void(PackedFloat32Array)> cb_float32_array,
                Callable cb_compressed_image
            )
    : topic(topic_name), type(msg_type),
        callback_string(cb_str), callback_int(cb_int),callback_image(cb_image),
        callback_int16_array(cb_int16_array),callback_float32_array(cb_float32_array),
        callback_compressed_image(cb_compressed_image) {

    if (type == ROS2_STRING && callback_string) {
        sub_string = node->create_subscription<std_msgs::msg::String>(
            topic, 10,
            [this](std_msgs::msg::String::UniquePtr msg){
                callback_string(msg->data);
            });
    } else if (type == ROS2_INT32 && callback_int) {
        sub_int = node->create_subscription<std_msgs::msg::Int32>(
            topic, 10,
            [this](std_msgs::msg::Int32::UniquePtr msg){
                callback_int(msg->data);
            });
    } else if (type == ROS2_IMAGE && callback_image.is_valid()){
        this->setup_image_subscription(node);
    } else if (type == ROS2_COMPRESSED_IMAGE && callback_compressed_image.is_valid()) {
        this->setup_compressed_image_subscription(node);
    } else if (type == ROS2_INT16_ARRAY && callback_int16_array){
        sub_int16_array = node->create_subscription<std_msgs::msg::Int16MultiArray>(
            topic,10,
            [this](std_msgs::msg::Int16MultiArray::UniquePtr msg){
                PackedInt32Array arr;
                arr.resize(msg->data.size());
                for(size_t i=0;i<msg->data.size();i++){
                    arr[i] = msg->data[i];
                }
                callback_int16_array(arr);
            }
        );
    } else if (type == ROS2_FLOAT32_ARRAY && callback_float32_array){
        sub_float32_array = node->create_subscription<std_msgs::msg::Float32MultiArray>(
            topic,10,
            [this](std_msgs::msg::Float32MultiArray::UniquePtr msg){
                PackedFloat32Array arr;
                arr.resize(msg->data.size());
                for(size_t i=0;i<msg->data.size();i++){
                    arr[i] = msg->data[i];
                }
                callback_float32_array(arr);
            }
        );
    }
}

void ROS2Subscriber::setup_image_subscription(std::shared_ptr<rclcpp::Node> node) {
    if (!callback_image.is_valid()) return;
    sub_image = node->create_subscription<sensor_msgs::msg::Image>(
        topic, 10,
        [this](sensor_msgs::msg::Image::UniquePtr msg){
            Ref<Image> image;
            image.instantiate();

            Image::Format format = Image::FORMAT_RGB8;

            if (msg->encoding == "bgr8") {
                std::vector<uint8_t> rgb_data(msg->data.size());
                for (size_t i = 0; i < msg->data.size(); i += 3) {
                    rgb_data[i]     = msg->data[i + 2];
                    rgb_data[i + 1] = msg->data[i + 1];
                    rgb_data[i + 2] = msg->data[i];
                }
                PackedByteArray arr;
                arr.resize(rgb_data.size());
                memcpy(arr.ptrw(), rgb_data.data(), rgb_data.size());
                image->create_from_data(msg->width, msg->height, false, format, arr);
            } else if (msg->encoding == "rgb8") {
                size_t size = (size_t)msg->width*msg->height*3;
                if (msg->data.size()!=size){
                    UtilityFunctions::print("ERROR:wrong image_size");
                }
                format = Image::FORMAT_RGB8;
                PackedByteArray arr;
                arr.resize(msg->data.size());
                memcpy(arr.ptrw(), msg->data.data(), msg->data.size());
                image=Image::create_from_data(msg->width, msg->height, false, format, arr);
                
            } else if (msg->encoding == "mono8") {
                format = Image::FORMAT_L8;
                PackedByteArray arr;
                arr.resize(msg->data.size());
                memcpy(arr.ptrw(), msg->data.data(), msg->data.size());
                image->create_from_data(msg->width, msg->height, false, format, arr);
            } else {
                UtilityFunctions::printerr("Unsupported image encoding: ", msg->encoding.c_str());
                return;
            }

            callback_image.callv(Array::make(image));
        });
}

void ROS2Subscriber::setup_compressed_image_subscription(std::shared_ptr<rclcpp::Node> node) {
    if (!callback_compressed_image.is_valid()) return;

    sub_compressed_image = node->create_subscription<sensor_msgs::msg::CompressedImage>(
        topic, 10,
        [this](sensor_msgs::msg::CompressedImage::UniquePtr msg){
            
            Ref<Image> image;
            image.instantiate();

            PackedByteArray data;
            data.resize(msg->data.size());
            memcpy(data.ptrw(), msg->data.data(), msg->data.size());

            // JPEG / PNG 判定
            if (msg->format.find("jpeg") != std::string::npos ||
                msg->format.find("jpg")  != std::string::npos)
            {
                const Error err = image->load_jpg_from_buffer(data);
                if (err != OK) {
                    UtilityFunctions::printerr("Failed to decode JPEG compressed image");
                    return;
                }
            }
            else if (msg->format.find("png") != std::string::npos)
            {
                const Error err = image->load_png_from_buffer(data);
                if (err != OK) {
                    UtilityFunctions::printerr("Failed to decode PNG compressed image");
                    return;
                }
            }
            else {
                UtilityFunctions::printerr("Unsupported compressed image format: ", msg->format.c_str());
                return;
            }

            callback_compressed_image.callv(Array::make(image));
        }
    );
}
