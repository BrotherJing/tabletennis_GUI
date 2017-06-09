cmake_minimum_required(VERSION 2.8.11)

project(tracking)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)

include_directories(/home/jing/caffe/build_cmake2/include)

# some prerequisites for the LSTM trajectory prediction library..
list(APPEND CMAKE_MODULE_PATH "/home/jing/Documents/RNN_tabletennis/src/cmake/Modules")

find_package(TensorFlow REQUIRED)
find_package(Eigen REQUIRED)

set(PROJECT_INCLUDE_DIRS ${TensorFlow_INCLUDE_DIRS} ${Eigen_INCLUDE_DIRS})

include_directories(${PROJECT_INCLUDE_DIRS} /home/jing/local/include)

include_directories(${PROJECT_SOURCE_DIR}/../)

# Find background subtraction library
find_path(Codebook_INCLUDE_DIR
	Codebook.h
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/BgSubtractor/include)

find_library(Codebook_LIBRARY
	bgsubtract
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/build/BgSubtractor)

# Find reconstruct 3D library
find_path(Rec3D_INCLUDE_DIR
        Reconstruct.h
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/Reconstruct3D/include)

find_library(Rec3D_LIBRARY
        rec3d
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/build/Reconstruct3D)

# Find CNN tracker library
find_path(Tracker_INCLUDE_DIR
	NNTracker.h
        HINTS
        /home/jing/Documents/CNN_tabletennis/src/include)

find_library(Tracker_LIBRARY
	tracker
        HINTS
        /home/jing/Documents/CNN_tabletennis/build/src)

# find LSTM trajectory prediction library
find_path(Trajpred_INCLUDE_DIR
        TrajPredict.h
        HINTS
        /home/jing/Documents/RNN_tabletennis/src/include)
        
find_library(Trajpred_LIBRARY
        trajpred
        HINTS
        /home/jing/Documents/RNN_tabletennis/src/build)

include_directories(${Codebook_INCLUDE_DIR} ${Rec3D_INCLUDE_DIR} ${Tracker_INCLUDE_DIR} ${Trajpred_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(test main.cpp ../ballprops.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(test ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Codebook_LIBRARY} ${Rec3D_LIBRARY} ${Tracker_LIBRARY} ${Trajpred_LIBRARY} ${TensorFlow_LIBRARIES})