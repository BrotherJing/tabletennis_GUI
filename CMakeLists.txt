cmake_minimum_required(VERSION 2.8.11)

project(tracking)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)

include_directories(/home/jing/caffe/build_cmake/include)

# Find the QtWidgets library
find_package(Qt5Widgets)

find_path(Codebook_INCLUDE_DIR
	Codebook.h
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/BgSubtractor/include)

find_library(Codebook_LIBRARY
	bgsubtract
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/build/BgSubtractor)

include_directories(${Codebook_INCLUDE_DIR})

find_path(Tracker_INCLUDE_DIR
	NNTracker.h
        HINTS
        /home/jing/Documents/CNN_tabletennis/src/include)

find_library(Tracker_LIBRARY
	tracker
        HINTS
        /home/jing/Documents/CNN_tabletennis/build/src)

include_directories(${Tracker_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(tracking main.cpp mainwindow.cpp tools.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(tracking Qt5::Widgets ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Codebook_LIBRARY} ${Tracker_LIBRARY})
