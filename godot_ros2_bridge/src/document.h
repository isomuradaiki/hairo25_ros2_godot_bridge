#pragma once
#ifndef _DEF_DOCUMENT_H
#define _DEF_DOCUMENT_H

#define DOC_CREATE_NODE u8"make ROS2 node"
#define DOC_SPIN_SOME u8"ROSノードを動作させる。(_process関数内に記述)"
#define DOC_SHUTDOWN u8"ROSノードを破棄"

#define DOC_ADD_PUBLISHER u8"ノードにpublisherを追加"
#define DOC_PUBLISH_STRING u8"`std_msgs::msg::String`をpublishする"
#define DOC_PUBLISH_INT32 u8"`std_msgs::msg::Int32`をpublishする"
#define DOC_PUBLISH_INT16_ARRAY u8"`std_msgs::Int16MultiArray`をpublishする"
#define DOC_PUBLISH_FLOAT32_ARRAY u8"`std_msgs::Float32MultiArray`をpublishする"

#define DOC_SUBSCRIBER_STRING u8"`std_msgs::msg::String`をsubscribeする"
#define DOC_SUBSCRIBER_INT32 u8"`std_msgs::msg::Int32`をsubscribeする"
#define DOC_SUBSCRIBER_INT16_ARRAY u8"`std_msgs::Int16MultiArray`をsubscribeする"
#define DOC_SUBSCRIBER_FLOAT32_ARRAY u8"`std_msgs::Float32MultiArray`をsubscribeする"
#define DOC_SUBSCRIBER_IMAGE u8"`sensor_msgs::msg::Image`をsubscribeする"


#endif