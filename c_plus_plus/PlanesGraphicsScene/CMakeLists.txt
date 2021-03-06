cmake_minimum_required (VERSION 3.10)
project (PlanesGraphicsScene)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0043 NEW)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../common
	${Qt5Widgets_INCLUDE_DIRS})

#link_directories(
#	${CMAKE_CURRENT_BINARY_DIR}/../common/Release/
#)

set(GS_SRCS 	
	animatedtextitem.cpp
	computerboard.cpp
	customgraphicsscene.cpp
	customgraphicsview.cpp
	customhorizlayout.cpp
	gamestatsframe.cpp
	genericboard.cpp
	gridsquare.cpp
	leftpane.cpp
	planesgsview.cpp
	planesgswindow.cpp
	playareagridsquare.cpp
	playerboard.cpp
	rightpane.cpp
	scoreframe.cpp
	optionswindow.cpp
	gamedifficultyoptions.cpp
	main.cpp
)

set(GS_HEADR 	
	animatedtextitem.h
	computerboard.h
	customgraphicsscene.h
	customgraphicsview.h
	customhorizlayout.h
	gamestatsframe.h
	genericboard.h
	gridsquare.h
	leftpane.h
	planesgsview.h
	planesgswindow.h
	playareagridsquare.h
	playerboard.h
	rightpane.h
	scoreframe.h
	optionswindow.h
	gamedifficultyoptions.h
)

add_executable(PlanesGraphicsScene 
	${GS_SRCS}
	${GS_HEADR}
	resources.qrc)
target_link_libraries(PlanesGraphicsScene
	libCommon
	${Qt5Widgets_LIBRARIES})
qt5_use_modules(PlanesGraphicsScene Widgets)

install(TARGETS PlanesGraphicsScene DESTINATION bin)