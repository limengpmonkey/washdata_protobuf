#include <iostream>
#include <fstream>
#include <iomanip>
#include "generated/vehicle_state.pb.h"

using namespace std;

// 打印传感器数据
void PrintSensorData(const autonomous_driving::SensorData& sensor) {
    cout << "  Sensor Type: ";
    switch (sensor.type()) {
        case autonomous_driving::SensorData::LIDAR:
            cout << "LIDAR"; break;
        case autonomous_driving::SensorData::RADAR:
            cout << "RADAR"; break;
        case autonomous_driving::SensorData::CAMERA:
            cout << "CAMERA"; break;
        case autonomous_driving::SensorData::ULTRASONIC:
            cout << "ULTRASONIC"; break;
        default:
            cout << "UNKNOWN"; break;
    }
    cout << "\n";
    
    cout << "  Horizontal FOV: " << sensor.horizontal_fov() << " deg\n";
    cout << "  Vertical FOV: " << sensor.vertical_fov() << " deg\n";
    cout << "  Max Range: " << sensor.max_range() << " m\n";
    cout << "  Confidence: " << fixed << setprecision(2) << sensor.confidence() * 100 << "%\n";
    cout << "  Calibrated: " << (sensor.is_calibrated() ? "Yes" : "No") << "\n";
    
    // 打印检测到的物体
    if (sensor.objects_size() > 0) {
        cout << "  Detected Objects (" << sensor.objects_size() << "):\n";
        for (const auto& obj : sensor.objects()) {
            cout << "    ID: " << obj.id() << "\n";
            cout << "    Classification: " << obj.classification() << "\n";
            cout << "    Position: (" << obj.x_position() << ", " 
                 << obj.y_position() << ", " << obj.z_position() << ") m\n";
            cout << "    Velocity: (" << obj.x_velocity() << ", " 
                 << obj.y_velocity() << ") m/s\n";
            cout << "    Dimensions: " << obj.width() << " x " 
                 << obj.length() << " x " << obj.height() << " m\n";
            cout << "    Confidence: " << fixed << setprecision(2) << obj.confidence() * 100 << "%\n";
            cout << "    Moving: " << (obj.is_moving() ? "Yes" : "No") << "\n";
            cout << "    -------------------------\n";
        }
    }
}

// 打印控制指令
void PrintControlCommand(const autonomous_driving::ControlCommand& cmd) {
    cout << "  Steering Angle: " << cmd.steering_angle() << " deg\n";
    cout << "  Throttle: " << fixed << setprecision(2) << cmd.throttle() * 100 << "%\n";
    cout << "  Brake: " << fixed << setprecision(2) << cmd.brake() * 100 << "%\n";
    cout << "  Turn Signals: Left=" << (cmd.turn_signal_left() ? "On" : "Off") 
         << ", Right=" << (cmd.turn_signal_right() ? "On" : "Off") << "\n";
    cout << "  Target Speed: " << cmd.target_speed() << " m/s\n";
    cout << "  Command Source: " << cmd.command_source() << "\n";
    cout << "  Execution Time: " << cmd.execution_time() << " s\n";
}

// 读取并打印数据包
void ReadAndPrintPacket(const string& filename) {
    autonomous_driving::VehicleState vehicle_state;
    
    // 从文件读取数据
    fstream input(filename, ios::in | ios::binary);
    if (!input) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    
    if (!vehicle_state.ParseFromIstream(&input)) {
        cerr << "Error: Failed to parse packet from file " << filename << endl;
        return;
    }
    
    // 打印车辆基本信息
    cout << "==================== VEHICLE STATE ====================\n";
    cout << "Vehicle ID: " << vehicle_state.vehicle_id() << "\n";
    cout << "Timestamp: " << fixed << setprecision(3) << vehicle_state.timestamp() << " s\n";
    cout << "Speed: " << vehicle_state.speed() << " m/s\n";
    cout << "Acceleration: " << vehicle_state.acceleration() << " m/s²\n";
    cout << "Driving Mode: " << vehicle_state.current_driving_mode() << "\n";
    cout << "Emergency Stop: " << (vehicle_state.emergency_stop_activated() ? "Activated" : "Inactive") << "\n";
    
    // 打印传感器数据
    if (vehicle_state.sensor_data_size() > 0) {
        cout << "\n==================== SENSOR DATA ====================\n";
        for (const auto& sensor : vehicle_state.sensor_data()) {
            PrintSensorData(sensor);
            cout << "------------------------------------------------\n";
        }
    }
    
    // 打印控制指令
    if (vehicle_state.has_latest_command()) {
        cout << "\n==================== CONTROL COMMAND ====================\n";
        PrintControlCommand(vehicle_state.latest_command());
    }
    
    cout << "\nPacket size: " << vehicle_state.ByteSizeLong() << " bytes\n";
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <packet_file>\n";
        cerr << "Example: " << argv[0] << " vehicle_state.bin\n";
        return -1;
    }
    
    ReadAndPrintPacket(argv[1]);
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
