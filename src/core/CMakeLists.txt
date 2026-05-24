#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/RenderManager.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/GamePlayState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ScoreState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/WeaponSelectState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameOverState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PauseState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/StateManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DifficultySelectState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameCompletedState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/LevelSelectState.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)