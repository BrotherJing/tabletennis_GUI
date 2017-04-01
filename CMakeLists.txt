cmake_minimum_required(VERSION 2.8.11)

project(tracking)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)

include_directories(/home/jing/caffe/build_cmake2/include)

# some prerequisites for the LSTM trajectory prediction library..
list(APPEND CMAKE_MODULE_PATH "/home/jing/Documents/RNN_tabletennis/src/cmake/Modules")

find_package(TensorFlow REQUIRED)
find_package(Eigen REQUIRED)

set(PROJECT_INCLUDE_DIRS ${TensorFlow_INCLUDE_DIRS} ${Eigen_INCLUDE_DIRS})

include_directories(${PROJECT_INCLUDE_DIRS} /home/jing/local/include)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Find background subtraction library
find_path(Codebook_INCLUDE_DIR
	Codebook.h
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/BgSubtractor/include)

find_library(Codebook_LIBRARY
	bgsubtract
        HINTS
        /home/jing/Documents/TableTennis/TableTennis/build/BgSubtractor)

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

include_directories(${Codebook_INCLUDE_DIR} ${Tracker_INCLUDE_DIR} ${Trajpred_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(tracking main.cpp mainwindow.cpp tools.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(tracking Qt5::Widgets ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Codebook_LIBRARY} ${Tracker_LIBRARY} ${Trajpred_LIBRARY} ${TensorFlow_LIBRARIES})

#remember to add TensorFlow_LIBRARIES here! otherwise error adding symbols: DSO missing from command line