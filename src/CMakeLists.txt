# cat function to combine headers
function(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

# make combined header # file(GLOB HEADER_FILES "*/*.hpp")  # doesn't work, needs to be in specific order :/
file(REMOVE ss2lib.hpp)
set(HEADER_FILES  "./helpers/helpers.hpp"
                  "./Histogram/Histogram.hpp"
                  "./SurfaceSource/SurfaceSource.hpp"
                  "./InputFile/InputFile.hpp"
                  )
foreach(HEADER_FILE ${HEADER_FILES})
  cat(${HEADER_FILE} ss2lib.hpp)
endforeach()

# executable targets:
add_executable(   ss2dist      ss2dist.cpp      )
add_executable(   ss2more      ss2more.cpp      )
add_executable(   turtle2rssa  turtle2rssa.cpp  )
add_executable(   writetest    writetest.cpp    )

# get source files from all folders
file(GLOB LIB_SOURCES "*/*.cpp")

# shared lib
add_library(ss2lib SHARED ${LIB_SOURCES})

# includes
include_directories(  "."                 )
include_directories(  "./helpers/"        )
include_directories(  "./Histogram/"      )
include_directories(  "./SurfaceSource/"  )
include_directories(  "./InputFile/"      )

# links
target_link_libraries(  ss2dist       ss2lib  )
target_link_libraries(  turtle2rssa   ss2lib  )
target_link_libraries(  writetest     ss2lib  )
target_link_libraries(  ss2more       ss2lib  )
target_link_libraries(  ss2more       flann   )

# add the install targets
install (TARGETS ss2dist DESTINATION bin)
install (TARGETS ss2lib DESTINATION  lib)
install (FILES "${PROJECT_SOURCE_DIR}/src/ss2lib.hpp" DESTINATION include)

# install support scripts
install (FILES "${PROJECT_SOURCE_DIR}/scripts/dist2plot.py" DESTINATION bin)
install (FILES "${PROJECT_SOURCE_DIR}/scripts/spec2plot.py" DESTINATION bin)
