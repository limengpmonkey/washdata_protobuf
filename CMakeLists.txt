cmake_minimum_required(VERSION 3.15)
project(ProtoConverter CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Protobuf库（仅需链接，不生成代码）
find_package(Protobuf REQUIRED)

# 指定预生成的旧版文件路径
set(OLD_PROTO_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/vehicle_state.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/sensor_data.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/perception_object.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/control_command.pb.cc"
)

# 指定预生成的新版文件路径
set(NEW_PROTO_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/generated_new/vehicle_state_v2.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated_new/sensor_data_v2.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated_new/perception_object_v2.pb.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated_new/control_command_v2.pb.cc"
)

# 添加可执行文件
add_executable(usejson_convert_old2new
    usejson_convert_old2new.cc
    ${OLD_PROTO_SRCS}
    ${NEW_PROTO_SRCS}
)

# 包含头文件目录
target_include_directories(usejson_convert_old2new PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/generated"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated_new"
)

# 链接Protobuf库和线程库
target_link_libraries(usejson_convert_old2new
    PRIVATE
    protobuf::libprotobuf
    pthread
)

# 标记生成文件为已存在（避免CMake检查生成规则）
set_source_files_properties(
    ${OLD_PROTO_SRCS} 
    #${NEW_PROTO_SRCS}
    PROPERTIES GENERATED FALSE
)