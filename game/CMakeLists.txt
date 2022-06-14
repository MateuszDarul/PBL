file(GLOB_RECURSE SOURCE_FILES
	src/System/MouseInputManager.cpp
	src/System/KeyboardInputManager.cpp
	src/System/InputManager.cpp
	src/System/GameApplication.cpp
	src/System/CollidersManager.cpp

	src/DataStructures/Particle.cpp
	src/DataStructures/Plane.cpp
	src/DataStructures/Frustum.cpp

	src/GameObject/GameObject.cpp
	src/GameObject/Components/Component.cpp
	src/GameObject/Components/ColliderComponent.cpp
	src/GameObject/Components/SphereCollider.cpp
	src/GameObject/Components/BoxCollider.cpp
	src/GameObject/Components/NameComponent.cpp
	src/GameObject/Components/ListComponent.cpp
	src/GameObject/Components/ModelComponent.cpp
	src/GameObject/Components/ModelInstancesComponent.cpp
	src/GameObject/Components/ModelGuiComponent.cpp
	src/GameObject/Components/ShaderComponent.cpp
	src/GameObject/Components/TransformComponent.cpp
	src/GameObject/Components/ScriptComponent.cpp
	src/GameObject/Components/CameraComponent.cpp
	src/GameObject/Components/TextComponent.cpp
	src/GameObject/Components/LineComponent.cpp
	src/GameObject/Components/LightComponent.cpp
	src/GameObject/Components/PointLightComponent.cpp
	src/GameObject/Components/SpotLightComponent.cpp
	src/GameObject/Components/FrustumCullingComponent.cpp
	src/GameObject/Components/ParticleComponent.cpp
	src/GameObject/Components/RefractiveGlassComponent.cpp
	src/GameObject/Components/ShadowComponent.cpp
	src/GameObject/Components/SlopeCollider.cpp

	src/GameObject/Scripts/LanternRange.cpp

	src/Resource/Font.cpp
	src/Resource/Mesh.cpp
	src/Resource/Material.cpp
	src/Resource/ResourceManager.cpp
	src/Resource/Shadow.cpp
	src/Resource/ShadowsManager.cpp

	src/Scene/Scene.cpp
	src/Scene/SceneNode.cpp
	src/Scene/Skybox.cpp
	src/Scene/MapLoader.cpp
	src/Scene/Menu/Menu.cpp

	src/AI/State.cpp
	src/AI/FollowPathState.cpp

	src/AI/IdleState.cpp
	src/AI/AttackingTowerState.cpp
	src/AI/AttackingPlayerState.cpp
	src/AI/RetreatFromFlashlightState.cpp
	src/AI/TakeDamageState.cpp

	src/AI/StateMachine.cpp
	src/AI/EnemyScript.cpp
	src/AI/EnemySpawnerScript.cpp

	src/AI/SteeringBehaviours.cpp

	src/Sounds/SoundDevice.cpp
	src/Sounds/SoundBuffer.cpp
	src/Sounds/SoundSource.cpp
	src/Sounds/MusicBuffer.cpp
	src/Sounds/SoundPlayer.cpp

	main.cpp
)
	
file(GLOB_RECURSE HEADER_FILES 
	include/System/MouseInputManager.h
	include/System/MouseButtons.h
	include/System/KeyboardInputManager.h
	include/System/KeyboardKeys.h
	include/System/InputManager.h
	include/System/GameApplication.h
	include/System/CollidersManager.h

	include/DataStructures/Particle.h
	include/DataStructures/Plane.h
	include/DataStructures/Frustum.h
	include/DataStructures/CollisionStructs.h

	include/GameObject/GameObject.h
	include/GameObject/Components/Components.h
	include/GameObject/Components/Component.h
	include/GameObject/Components/ColliderComponent.h
	include/GameObject/Components/SphereCollider.h
	include/GameObject/Components/BoxCollider.h
	include/GameObject/Components/NameComponent.h
	include/GameObject/Components/ListComponent.h
	include/GameObject/Components/ModelComponent.h
	include/GameObject/Components/ModelInstancesComponent.h
	include/GameObject/Components/ModelGuiComponent.h
	include/GameObject/Components/ShaderComponent.h
	include/GameObject/Components/TransformComponent.h
	include/GameObject/Components/ScriptComponent.h
	include/GameObject/Components/CameraComponent.h
	include/GameObject/Components/TextComponent.h
	include/GameObject/Components/LineComponent.h
	include/GameObject/Components/LightComponent.h
	include/GameObject/Components/PointLightComponent.h
	include/GameObject/Components/SpotLightComponent.h
	include/GameObject/Components/FrustumCullingComponent.h
	include/GameObject/Components/ParticleComponent.cpp
	include/GameObject/Components/RefractiveGlassComponent.cpp
	include/GameObject/Components/ShadowComponent.h
	include/GameObject/Components/SlopeCollider.h

	include/Resource/Font.h
	include/Resource/Mesh.h
	include/Resource/Material.h
	include/Resource/ResourceManager.h
	include/Resource/Shadow.h
	include/Resource/ShadowsManager.h

	include/Scene/Scene.h
	include/Scene/SceneNode.h
	include/Scene/Skybox.h
	include/Scene/MapLoader.h
	include/Scene/Menu/Menu.h

	include/AI/State.h
	include/AI/FollowPathState.h

	include/AI/IdleState.h
	include/AI/AttackingTowerState.h
	include/AI/AttackingPlayerState.h
	include/AI/RetreatFromFlashlightState.h
	include/AI/TakeDamageState.h

	include/AI/StateMachine.h
	include/AI/EnemyScript.h
	include/AI/EnemySpawnerScript.h

	include/AI/SteeringBehaviours.h

	include/AI/Path.h

	include/Sounds/SoundDevice.h
	include/Sounds/SoundBuffer.h
	include/Sounds/SoundSource.h
	include/Sounds/MusicBuffer.h
	include/Sounds/SoundPlayer.h

	include/GameObject/Scripts/*.h
)

include_directories(include)
include_directories(include/System)
include_directories(include/DataStructures)
include_directories(include/GameObject)
include_directories(include/GameObject/Components)
include_directories(include/Resource)
include_directories(include/Scene)
include_directories(include/Scene/Menu)
include_directories(include/AI)
include_directories(include/Sounds)

add_compile_options(-Wall)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "${ASSIMP_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${FREETYPE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${RAPIDJSON_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${OPENAL_SOFT_INCLUDE_DIR}")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_include_directories(${PROJECT_NAME} PUBLIC "${FLAC_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${OPUS_INCLUDE_DIR}")
endif()
target_include_directories(${PROJECT_NAME} PUBLIC "${LIBSNDFILE_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${ASSIMP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARY}"      "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${STB_IMAGE_LIBRARY}" "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${FREETYPE_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${OPENAL_SOFT_LIBRARY}")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_link_libraries(${PROJECT_NAME} "${OPUS_LIBRARY}")
endif()
target_link_libraries(${PROJECT_NAME} "${LIBSNDFILE_LIBRARY}")

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
							   ${CMAKE_SOURCE_DIR}/Resources
							   ${CMAKE_CURRENT_BINARY_DIR}/Resources)
							   
   	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

		add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
							COMMAND ${CMAKE_COMMAND} -E copy
									${OPENAL_SOFT_DIR}/build/libopenal.so
									${CMAKE_CURRENT_BINARY_DIR}/../thirdparty)
	
	else()
	
		add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
							COMMAND ${CMAKE_COMMAND} -E copy
									${OPENAL_SOFT_DIR}/build/OpenAL32.dll
									${CMAKE_CURRENT_BINARY_DIR})

   endif()
   
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
							   ${CMAKE_SOURCE_DIR}/Resources
							   ${CMAKE_CURRENT_BINARY_DIR}/Resources)

	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
							   ${CMAKE_SOURCE_DIR}/Resources
							   ${CMAKE_CURRENT_BINARY_DIR}/Debug/Resources)

	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_directory
								${CMAKE_SOURCE_DIR}/Resources
								${CMAKE_CURRENT_BINARY_DIR}/Resources)

	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy
								${OPENAL_SOFT_DIR}/build/Debug/OpenAL32.dll
								${CMAKE_CURRENT_BINARY_DIR}/Debug/)

								
endif()