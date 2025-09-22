# CMakeList.txt : CMake project for MovieRussianRoulette, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MovieRussianRoulette")

# Add dependencies.
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add source to this project's executable.
add_executable (MovieRussianRoulette 
"MovieRussianRoulette.cpp" 
"MovieRussianRoulette.h" 
"DbClient.hpp" 
"DbClient.cpp"  
"Utils.h" 
"Utils.cpp" 
"Cli.cpp" 
"Movie.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MovieRussianRoulette PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(MovieRussianRoulette PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
