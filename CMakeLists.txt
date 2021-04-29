cmake_minimum_required(VERSION 3.17)
project(ex6)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(ex6 MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h CLI.h CLI.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp minCircle.h minCircle.cpp Server.cpp SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h Server.h)