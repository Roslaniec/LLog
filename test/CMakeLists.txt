# ****************************************************************************
#  Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ****************************************************************************
set( Boost_USE_STATIC_LIBS OFF )
find_package( Boost 1.44 COMPONENTS
  unit_test_framework filesystem system thread REQUIRED)

add_test(Test logtest)

add_executable(logtest test.cpp)
target_link_libraries(logtest
  llog
  ${Boost_LIBRARIES}
  ${MARIADB_LIBRARY})  
target_include_directories(logtest PUBLIC
  ${MARIADB_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${Boost_INCLUDE_DIR})

