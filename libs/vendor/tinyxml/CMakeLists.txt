set(SOURCES
	tinystr.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
	tinyxml
	STATIC
	${SOURCES}
)

set_target_properties(tinyxml PROPERTIES COMPILE_DEFINITIONS TIXML_USE_STL)
