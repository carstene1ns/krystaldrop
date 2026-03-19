add_executable(kdrop
	Tools/direct.h
	Tools/ZIPArchiveReader.h
	Tools/direct.cpp
	Tools/ACCArchiveReader.h
	Tools/ACCArchiveReader.cpp
	Tools/ZIPArchiveReader.cpp
	Names.cpp
	main.cpp
	Video/InputBox.cpp
	Video/Event
	Video/Event/AnimTextEvent.cpp
	Video/Event/AnimTextEvent.h
	Video/Gem.cpp
	Video/Background.h
	Video/Background.cpp
	Video/Gem.h
	Video/InputBox.h
	Game/ControlsConfig.h
	Game/Highscores.h
	Game/XMLCharacterParser.h
	Game/Character.cpp
	Game/Hand.cpp
	Game/Table.cpp
	Game/Row.cpp
	Game/Character.h
	Game/XMLCharacterParser.cpp
	Game/Set.cpp
	Game/Hand.h
	Game/Table.h
	Game/AnimRow.cpp
	Game/Memo.h
	Game/ControlsConfig.cpp
	Game/Row.h
	Game/Parameter.h
	Game/AnimRow.h
	Game/Set.h
	Game/Memo.cpp
	Game/Parameter.cpp
	Game/Highscores.cpp
	global.h
	Names.h
	Controller/CharSelectController.h
	Controller/TitleController.h
	Controller/TitleController.cpp
	Controller/MenuController.h
	Controller/CharSelect2Controller.cpp
	Controller/DuelController.h
	Controller/SurvivalController.cpp
	Controller/HighScoresController.h
	Controller/KDApplication.cpp
	Controller/MenuController.cpp
	Controller/BackgroundController.cpp
	Controller/DuelController.cpp
	Controller/ControlsController.h
	Controller/BackgroundController.h
	Controller/CharSelectController.cpp
	Controller/CharSelect2Controller.h
	Controller/ControlsController.cpp
	Controller/HighScoresController.cpp
	Controller/KDApplication.h
	Controller/SurvivalController.h)

target_compile_definitions(kdrop PRIVATE ACC_ZLIB_SUPPORT)

#FIXME: DISPLAY_FPS is broken

target_link_libraries(kdrop PRIVATE
	#SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
	KDpp)

# TODO: port to SDL2
target_link_libraries(kdrop PRIVATE SDL -lSDL_mixer -lSDL_image -lSDL_ttf -lGL)

if(UNIX)
	target_link_libraries(kdrop PRIVATE -lm)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_compile_options(kdrop PRIVATE "-fno-math-errno")
	endif()
endif()
