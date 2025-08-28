cmake_minimum_required( VERSION 3.28 )
project( oneptr-ciphers C )

set( PROJECT_NAME "onepointer-ciphers" )
set( PROJECT_VERSION "0.0.1-dev1" )
set( PROJECT_DESCRIPTION "Securement digest and encryption ciphers from OnePointerT." )

set( CMAKE_C_STANDARD 17 )
set( CMAKE_CXX_STANDARD 23 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

include_directories( ${PROJECT_SOURCE_DIR} )

add_subdirectory( lib/libm )
include_directories( ${PROJECT_SOURCE_DIR}/lib/libm/src/ce-libm )
include_directories( ${PROJECT_SOURCE_DIR}/lib/libm/lib/endian_h )

file( GLOB ONEPTR_CIPHERS_SRCFILES_C ${PROJECT_SOURCE_DIR}/*.c )
add_library( oneptrc_ciphers STATIC ${ONEPTR_CIPHERS_SRCFILES_C} )
target_link_libraries( oneptrc_ciphers m_ce )


include_directories( ${PROJECT_SOURCE_DIR}/cpy )
file( GLOB ONEPTR_CIPHERS_SRCFILES_CYTHON cpy/*.c )
#add_library( oneptrpy_ciphers STATIC ${ONEPTR_CIPHERS_SRCFILES_CYTHON} )
#target_link_libraries( oneptrpy_ciphers oneptrc_ciphers )

set( ENABLE_DOC_oneptr_ciphers TRUE )
if( ENABLE_DOC_oneptr_ciphers )
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		message("-- Enabled doc_oneptr_ciphers target for make")
        # CMAKE_CURRENT_SOURCE_DIR is where the last call to CMake's project was,
        # CMAKE_CURRENT_BINARY_DIR is where the build of the binaries is performed
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc_oneptr_ciphers ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
	endif()
endif()