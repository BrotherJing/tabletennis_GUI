cmake_minimum_required(VERSION 2.8.11)

project(tracking)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)

#CHANGE TO YOUR REPOS' DIRECTORIES!!
set(PROJECT_DIR_TABLETENNIS /home/jing/Documents/TableTennis)
set(PROJECT_DIR_CNN_TABLETENNIS /home/jing/Documents/CNN_tabletennis)
set(PROJECT_DIR_RNN_TABLETENNIS /home/jing/Documents/RNN_tabletennis)

#CHANGE TO YOUR Caffe INCLUDE DIRECTORY!!
include_directories(/home/jing/caffe/build_cmake2/include)

# some prerequisites for the LSTM trajectory prediction library..
list(APPEND CMAKE_MODULE_PATH ${PROJECT_DIR_RNN_TABLETENNIS}/src/cmake/Modules)

find_package(TensorFlow REQUIRED)
find_package(Eigen REQUIRED)

set(PROJECT_INCLUDE_DIRS ${TensorFlow_INCLUDE_DIRS} ${Eigen_INCLUDE_DIRS})

#CHANGE THIS TO YOUR Eigen3 DIRECTORY!!
include_directories(${PROJECT_INCLUDE_DIRS} /home/jing/local/include)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Find background subtraction library
find_path(Codebook_INCLUDE_DIR
	Codebook.h
        HINTS
        ${PROJECT_DIR_TABLETENNIS}/TableTennis/BgSubtractor/include)

find_library(Codebook_LIBRARY
	bgsubtract
        HINTS
        ${PROJECT_DIR_TABLETENNIS}/TableTennis/build/BgSubtractor)

# Find reconstruct 3D library
find_path(Rec3D_INCLUDE_DIR
	Reconstruct.h
        HINTS
        ${PROJECT_DIR_TABLETENNIS}/TableTennis/Reconstruct3D/include)

find_library(Rec3D_LIBRARY
	rec3d
        HINTS
        ${PROJECT_DIR_TABLETENNIS}/TableTennis/build/Reconstruct3D)

# Find CNN tracker library
find_path(Tracker_INCLUDE_DIR
	NNTracker.h
        HINTS
        ${PROJECT_DIR_CNN_TABLETENNIS}/src/include)

find_library(Tracker_LIBRARY
	tracker
        HINTS
        ${PROJECT_DIR_CNN_TABLETENNIS}/build/src)

# find LSTM trajectory prediction library
find_path(Trajpred_INCLUDE_DIR
        TrajPredict.h
        HINTS
        ${PROJECT_DIR_RNN_TABLETENNIS}/src/include)
        
find_library(Trajpred_LIBRARY
        trajpred
        HINTS
        ${PROJECT_DIR_RNN_TABLETENNIS}/src/build)

include_directories(${Codebook_INCLUDE_DIR} ${Rec3D_INCLUDE_DIR} ${Tracker_INCLUDE_DIR} ${Trajpred_INCLUDE_DIR})

# Tell CMake to create the helloworld executable
add_executable(tracking main.cpp mainwindow.cpp tools.cpp pingpongtablearea.cpp ballprops.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(tracking Qt5::Widgets ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Codebook_LIBRARY}  ${Rec3D_LIBRARY} ${Tracker_LIBRARY} ${Trajpred_LIBRARY} ${TensorFlow_LIBRARIES})

#remember to add TensorFlow_LIBRARIES here! otherwise error adding symbols: DSO missing from command line
