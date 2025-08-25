#include <iostream>
#include <fstream>
#include <vector>
#include "generated/vehicle_state.pb.h"

using namespace std;

// 生成并返回填充好的 VehicleState 对象
autonomous_driving::VehicleState GenerateVehicleStatePacket() {
    autonomous_driving::VehicleState vehicle_state;
    
    // 设置基础车辆信息
    vehicle_state.set_vehicle_id("ADV_2025_001");
    vehicle_state.set_timestamp(1692930418.0); // 2023-08-25 10:26:58
    vehicle_state.set_speed(15.5);  // 15.5 m/s (~55.8 km/h)
    vehicle_state.set_acceleration(0.8); // 0.8 m/s²
    vehicle_state.set_emergency_stop_activated(false);
    vehicle_state.set_current_driving_mode("AUTO");
    
    // 添加传感器数据 - LiDAR
    auto* lidar_data = vehicle_state.add_sensor_data();
    lidar_data->set_type(autonomous_driving::SensorData::LIDAR);
    lidar_data->set_horizontal_fov(120.0);
    lidar_data->set_vertical_fov(30.0);
    lidar_data->set_max_range(200.0);
    lidar_data->set_confidence(0.98);
    lidar_data->set_is_calibrated(true);
    
    // 添加LiDAR检测到的物体
    auto* vehicle_obj = lidar_data->add_objects();
    vehicle_obj->set_id("obj_001");
    vehicle_obj->set_classification("vehicle");
    vehicle_obj->set_x_position(25.3);
    vehicle_obj->set_y_position(1.5);
    vehicle_obj->set_z_position(0.0);
    vehicle_obj->set_x_velocity(5.0);
    vehicle_obj->set_y_velocity(0.1);
    vehicle_obj->set_width(2.0);
    vehicle_obj->set_length(4.5);
    vehicle_obj->set_height(1.8);
    vehicle_obj->set_confidence(0.95);
    vehicle_obj->set_is_moving(true);
    
    auto* pedestrian_obj = lidar_data->add_objects();
    pedestrian_obj->set_id("obj_002");
    pedestrian_obj->set_classification("pedestrian");
    pedestrian_obj->set_x_position(10.2);
    pedestrian_obj->set_y_position(-2.3);
    pedestrian_obj->set_z_position(0.0);
    pedestrian_obj->set_x_velocity(1.2);
    pedestrian_obj->set_y_velocity(-0.5);
    pedestrian_obj->set_width(0.6);
    pedestrian_obj->set_length(0.4);
    pedestrian_obj->set_height(1.7);
    pedestrian_obj->set_confidence(0.90);
    pedestrian_obj->set_is_moving(true);
    
    // 添加传感器数据 - 摄像头
    auto* camera_data = vehicle_state.add_sensor_data();
    camera_data->set_type(autonomous_driving::SensorData::CAMERA);
    camera_data->set_horizontal_fov(60.0);
    camera_data->set_vertical_fov(40.0);
    camera_data->set_max_range(150.0);
    camera_data->set_confidence(0.96);
    camera_data->set_is_calibrated(true);
    
    // 添加控制指令
    auto* command = vehicle_state.mutable_latest_command();
    command->set_steering_angle(2.5);  // 2.5度
    command->set_throttle(0.35);      // 35%油门
    command->set_brake(0.0);          // 无制动
    command->set_turn_signal_left(false);
    command->set_turn_signal_right(false);
    command->set_target_speed(16.0);  // 目标速度16 m/s
    command->set_command_source("PlanningAlgorithm");
    command->set_execution_time(0.1);  // 预计执行时间0.1秒
    
    return vehicle_state;
}

// 序列化到文件
void SerializeToFile(const autonomous_driving::VehicleState& packet, const string& filename) {
    fstream output(filename, ios::out | ios::trunc | ios::binary);
    if (!packet.SerializeToOstream(&output)) {
        cerr << "Failed to write packet to file." << endl;
    }
    cout << "Packet serialized to " << filename << endl;
    cout << "Packet size: " << packet.ByteSizeLong() << " bytes" << endl;
}

// 序列化到字符串
string SerializeToString(const autonomous_driving::VehicleState& packet) {
    string serialized_data;
    if (!packet.SerializeToString(&serialized_data)) {
        cerr << "Failed to serialize packet to string." << endl;
    }
    return serialized_data;
}

// 序列化到字节数组
vector<uint8_t> SerializeToBytes(const autonomous_driving::VehicleState& packet) {
    vector<uint8_t> byte_data(packet.ByteSizeLong());
    if (!packet.SerializeToArray(byte_data.data(), byte_data.size())) {
        cerr << "Failed to serialize packet to byte array." << endl;
    }
    return byte_data;
}

// 反序列化示例
void DeserializeAndPrint(const string& filename) {
    autonomous_driving::VehicleState packet;
    fstream input(filename, ios::in | ios::binary);
    
    if (!packet.ParseFromIstream(&input)) {
        cerr << "Failed to parse packet from file." << endl;
        return;
    }
    
    cout << "\nDeserialized Packet Content:" << endl;
    cout << "Vehicle ID: " << packet.vehicle_id() << endl;
    cout << "Timestamp: " << packet.timestamp() << endl;
    cout << "Speed: " << packet.speed() << " m/s" << endl;
    cout << "Acceleration: " << packet.acceleration() << " m/s²" << endl;
    cout << "Driving Mode: " << packet.current_driving_mode() << endl;
    
    cout << "\nSensor Data Count: " << packet.sensor_data_size() << endl;
    for (const auto& sensor : packet.sensor_data()) {
        cout << "Sensor Type: " << sensor.type() << endl;
        cout << "Detected Objects: " << sensor.objects_size() << endl;
    }
    
    cout << "\nControl Command:" << endl;
    cout << "Steering Angle: " << packet.latest_command().steering_angle() << " deg" << endl;
    cout << "Throttle: " << packet.latest_command().throttle() << endl;
    cout << "Target Speed: " << packet.latest_command().target_speed() << " m/s" << endl;
}

int main() {
    // 初始化 Protobuf 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    // 生成数据包
    auto packet = GenerateVehicleStatePacket();
    
    // 序列化到文件
    SerializeToFile(packet, "vehicle_state.bin");
    
    // 序列化到字符串
    string serialized_str = SerializeToString(packet);
    cout << "\nSerialized to string, length: " << serialized_str.size() << " bytes" << endl;
    
    // 序列化到字节数组
    auto byte_data = SerializeToBytes(packet);
    cout << "Serialized to byte array, size: " << byte_data.size() << " bytes" << endl;
    
    // 反序列化并打印内容
    DeserializeAndPrint("vehicle_state.bin");
    
    // 清理 Protobuf 库
    google::protobuf::ShutdownProtobufLibrary();
    
    return 0;
}
