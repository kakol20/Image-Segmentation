# CMakeList.txt : CMake project for Image Segmentation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Image Segmentation")

# set(ImageSegmentation "Image Segmentation")

# Add source to this project's executable.
add_executable (ImageSegmentation
  "ext/json/json.hpp"
  "ext/stb/stb_image.h"
  "ext/stb/stb_image_write.h"
  "src/colourSpace/ColorSpace.hpp"
  "src/colourSpace/OkLab.cpp"
  "src/colourSpace/OkLCh.cpp"
  "src/colourSpace/sRGB.hpp"
  "src/image/Image.cpp"
  "src/kmeans/Colour.cpp"
  "src/kmeans/KMeans.cpp"
  "src/maths/Maths.hpp"
  "src/other/Log.cpp"
  "src/other/Random.cpp"
  "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ImageSegmentation PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
