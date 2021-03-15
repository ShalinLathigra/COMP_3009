# Copyright (c)
# 2021 Sharjeel Ali <sharjeelali@cmail.carleton.ca>
# 2018 David Mould <mould@scs.carleton.ca>

cmake_minimum_required(VERSION 2.6)

#Set the target plaform of the project to be win32
if(WIN32)
	set(CMAKE_GENERATOR_PLATFORM win32)
endif(WIN32)

# Name of project
project(COMP3009_T1_HelloWorld)

# Specify project source files
set(SRCS
    ${PROJECT_SOURCE_DIR}/main.cpp
)

# Add executable based on the source files
add_executable(COMP3009_T1_HelloWorld ${SRCS})
