cmake_minimum_required(VERSION 3.10)

project(portfoliooptimiser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(portfolio_simulator
    src/main.cpp
    src/investinstrument/stocks.cpp
    src/pricing/price_simulator.cpp
)

target_include_directories(portfolio_simulator PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
