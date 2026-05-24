#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/DirectionalMovement.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/InputMovement.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChaosMovement.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TurningMovement.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/BoundingCircle.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/Player.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PlayerShoot.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Bullet.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BulletEmitter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Enemy.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/Level.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Pattern.cpp"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/ScreenFlashEffect.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/Bomb.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Boss.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Item.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)