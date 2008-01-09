#all include directories
INCLUDE_DIRECTORIES(Core Files Games Games/Asteroids Memory Mix Sound Systems Systems/Collision Utils VS VS/Graphics VS/Math VS/Utils)

INCLUDE(FindSDL_mixer)
INCLUDE(FindSDL)
INCLUDE(FindOpenGL)

SET(CORE_SOURCES
Core/CORE_Game.cpp
Core/CORE_GameMode.cpp
Core/CORE_GameRegistry.cpp
Core/CORE_GameSystem.cpp
Core/CORE_Version.cpp
Core/Core.cpp)

SET(FILE_SOURCES
Files/FS_File.cpp
Files/FS_Record.cpp)

SET(MEMORY_SOURCES
Memory/MEM_Heap.cpp
Memory/MEM_Store.cpp)

SET(UTILS_SOURCES
Utils/UT_Menu.cpp)

SET(VS_SOURCES
VS/Utils/VS_Debug.cpp
VS/Utils/VS_LocalisationTable.cpp
VS/Utils/VS_Log.cpp
VS/Utils/VS_Preferences.cpp
VS/Utils/VS_String.cpp
VS/Utils/VS_System.cpp
VS/Math/VS_Angle.cpp
VS/Math/VS_Math.cpp
VS/Math/VS_Random.cpp
VS/Math/VS_Transform.cpp
VS/Math/VS_Vector.cpp
VS/Graphics/VS_Camera.cpp
VS/Graphics/VS_Color.cpp
VS/Graphics/VS_DisplayList.cpp
VS/Graphics/VS_Entity.cpp
VS/Graphics/VS_Font.cpp
VS/Graphics/VS_Layer.cpp
VS/Graphics/VS_Renderer.cpp
VS/Graphics/VS_RendererPretty.cpp
VS/Graphics/VS_RendererSimple.cpp
VS/Graphics/VS_Screen.cpp
VS/Graphics/VS_Sprite.cpp)

SET(SYSTEMS_SOURCES
Systems/COL_System.cpp
Systems/SYS_Input.cpp
Systems/SYS_Timer.cpp
Systems/Collision/COL_Object.cpp
Systems/Collision/COL_Sprite.cpp
Systems/Collision/PHYS_Emitter.cpp
Systems/Collision/PHYS_Sprite.cpp)

SET(SOUND_SOURCES
Sound/SND_Music.cpp
Sound/SND_Sample.cpp
Sound/SND_System.cpp
Sound/)

SET(ASTEROIDS_SOURCES
Games/GM_MainMenu.cpp
Games/Asteroids/AST_Asteroid.cpp
Games/Asteroids/AST_Camera.cpp
Games/Asteroids/AST_Game.cpp
Games/Asteroids/AST_Hud.cpp
Games/Asteroids/AST_ModeGameOver.cpp
Games/Asteroids/AST_ModeInGame.cpp
Games/Asteroids/AST_ModeTitleScreen.cpp
Games/Asteroids/AST_PlayerShip.cpp
Games/Asteroids/AST_PlayerShot.cpp
Games/Asteroids/AST_Star.cpp)

SET(STARSHOT_SOURCES
Games/StarShot/SST_Fireworks.cpp
Games/StarShot/SST_Game.cpp
Games/StarShot/SST_Hud.cpp
Games/StarShot/SST_Rocket.cpp
Games/StarShot/SST_Sequence.cpp)

SET(PREFERENCES_SOURCES
Games/Preferences/PREF_Game.cpp)

SET(PLATFORM_SOURCES)

if ( APPLE )
	SET( PLATFORM_SOURCES SDLMain.m )	
endif ( APPLE )

#FIND_PACKAGE(SDLMIXER)
#FIND_PACKAGE(SDL REQUIRED)

if ( NOT SDL_FOUND )
	message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

if ( NOT SDLMIXER_FOUND )
	message ( FATAL_ERROR "SDL_Mixer not found!" )
endif ( NOT SDLMIXER_FOUND )

if ( NOT OPENGL_FOUND )
	message ( FATAL_ERROR "OpenGL development libraries not found!" )
endif ( NOT OPENGL_FOUND )

if ( NOT OPENGL_GLU_FOUND )
	message ( FATAL_ERROR "GLU not found!" )
endif ( NOT OPENGL_GLU_FOUND )

INCLUDE_DIRECTORIES(
	${OPENGL_INCLUDE_DIR}
	${SDL_INCLUDE_DIR} 
	${SDLMIXER_INCLUDE_DIR}
)

LINK_LIBRARIES(
	${OPENGL_LIBRARIES} 
	${SDL_LIBRARY} 
	${SDLMIXER_LIBRARY}
)

ADD_DEFINITIONS("-include VS_Prefix.h -D CMAKE_BUILD")

ADD_EXECUTABLE(
	VectorStorm
	WIN32
	MACOSX_BUNDLE  #not a proper bundled app yet;  eventually figure out how to get our resource files where we need them for an OSX bundled app.
	${CORE_SOURCES}
	${FILE_SOURCES}
	${MEMORY_SOURCES}
	${UTILS_SOURCES}
	${VS_SOURCES}
	${SYSTEMS_SOURCES}
	${ASTEROIDS_SOURCES}
	${STARSHOT_SOURCES}
	${PREFERENCES_SOURCES}
	${SOUND_SOURCES}
	${PLATFORM_SOURCES}
)
