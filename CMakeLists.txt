# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)


#Enable the CMP0079 policy (for target_link_libraries with ALIAS targets)
if(POLICY CMP0079)
	cmake_policy(SET CMP0079 NEW)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# add vcpkg to the project
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Masou/Desktop/GPU/vcpkg/scripts/buildsystems/vcpkg.cmake")
# add finaTIRA.cmake to the project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

project ("mproject")

find_package(glm REQUIRED)
find_package(TIRA REQUIRED)

include_directories(
			${TIRA_INCLUDE_DIRS}
)


# Include sub-projects.
add_subdirectory ("mproject")

target_link_libraries(mproject 
							PRIVATE glm::glm
)
