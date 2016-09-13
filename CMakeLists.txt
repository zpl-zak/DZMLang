cmake_minimum_required(VERSION 3.2)
project(DZMLang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result -Wno-unused-parameter -std=gnu++11 -Wall -Wextra -Werror")

set(SOURCE_FILES
    src/lang/evl/evl_apl.hpp
    src/lang/evl/evl_app.hpp
    src/lang/evl/evl_cond.hpp
    src/lang/evl/evl_evl.hpp
    src/lang/evl/evl_lbd.hpp
    src/lang/evl/evl_lit.hpp
    src/lang/evl/evl_mod.hpp
    src/lang/evl/evl_ops.hpp
        src/lang/prc/prc_net.hpp
    src/lang/dzm_evl.hpp
    src/lang/dzm_lex.hpp
    src/lang/dzm_mdl.hpp
    src/lang/dzm_prc.hpp
    src/lang/dzm_prt.hpp
    src/lang/dzm_rep.hpp
    src/dzm.hpp
    src/dzm_log.hpp
    src/dzm_mem.hpp
    src/dzm_utl.hpp
    app.cpp
    dzm_ver.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ctime ctime.c)

add_executable(dzm ${SOURCE_FILES})

target_link_libraries(dzm pthread readline m)

