set(SOURCES
	mtrand.cpp
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
	mtrand
	STATIC
	${SOURCES}
)
