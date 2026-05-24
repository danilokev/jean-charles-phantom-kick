#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    #"${CMAKE_CURRENT_SOURCE_DIR}/fichero.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MusicPlayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SoundPlayer.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/Menu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MenuState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DialogSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DialogState.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BulletSpawnEffect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EnemyDeathEffect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PlayerDeathEffect.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TileMap.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/DialogSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DialogState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DialogData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DialogSistem.cpp"


    
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)