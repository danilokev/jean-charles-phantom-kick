#include "../include/Boss.h"
#include "../include/SoundPlayer.h"
#include <cmath>
#include <iostream>
#include <random>

Boss::Boss()
    : position(0.f, 0.f),
      movement(nullptr),
      collision(position, 0.f),
      sprite(),
      maxLifePerBar(0.f),
      currentLife(0.f),
      maxHealthBars(0),
      currentHealthBar(0),
      currentPhaseIndex(0),
      immune(false),
      triggerBomb(false),
      targetPosition(0.f, 0.f)
{}

Boss::Boss(sf::Vector2f p, std::shared_ptr<MovementStrategy> m, float collisionRadius, SpriteTemplate spriteType, int healthBars, float lifePerBar)
: position(p), movement(std::move(m)), collision(p, collisionRadius),
sprite(spriteType), maxHealthBars(healthBars),
currentHealthBar(healthBars), maxLifePerBar(lifePerBar),
currentLife(lifePerBar), currentPhaseIndex(0), immune(false), triggerBomb(false),
targetPosition(p) {sprite.setPosition(p.x, p.y);}

void Boss::update(float deltaTime) {
    switch (state) {
        case BossState::Inactive:
            return;

        case BossState::Dead:
            return;

        case BossState::WaitingForNext:
            return;

        case BossState::Active:
            if (movement) {
                position += movement->getMovementVector() * deltaTime;
                collision.setCenter(position);
                sprite.setPosition(position.x, position.y);
            }
            updateSpawners(deltaTime);
            break;

        case BossState::Transitioning:
            handlePhaseTransition(deltaTime);
            break;
    }
}

void Boss::handlePhaseTransition(float deltaTime) {
    currentLife += maxLifePerBar * deltaTime;

    if (currentLife >= maxLifePerBar) {
        state = BossState::Active;
    }
}

void Boss::takeDamage(float damage) {
    if (state != BossState::Active) return;

    currentLife -= damage;
    SoundPlayer::getInstance().playSound("boss_damage");
    if (currentLife <= 0.f) {
        currentPhaseIndex++;
        if (currentPhaseIndex == phases.size()) {
            triggerBomb = true;
            state = BossState::Dead;
            SoundPlayer::getInstance().playSound("boss_defeat");
            
        } else {
            if (currentPhaseIndex == phases.size()-1) maxLifePerBar *= 2;
            currentLife = 0.f;
            triggerBomb = true;
            SoundPlayer::getInstance().playSound("player_bomb");
            targetPosition = sf::Vector2f(212.f, 90.f);
            state = BossState::Transitioning;
        }
    }
}

bool Boss::isWaitingForNext() const {
    return state == BossState::WaitingForNext;
}

void Boss::render() const {
    // Dibujar el sprite del boss
    RenderManager::getInstance()->draw(sprite.getSprite());

    // Dibujar barra de vida encima
    float lifeRatio = currentLife / maxLifePerBar;

    sf::RectangleShape lifeBarBackground;
    lifeBarBackground.setSize(sf::Vector2f(50.f, 5.f));
    lifeBarBackground.setFillColor(sf::Color(100, 100, 100));
    lifeBarBackground.setPosition(position.x - 25.f, position.y - 50.f);

    sf::RectangleShape lifeBar;
    lifeBar.setSize(sf::Vector2f(50.f * lifeRatio, 5.f));
    lifeBar.setFillColor(sf::Color::Red);
    lifeBar.setPosition(position.x - 25.f, position.y - 50.f);

    RenderManager::getInstance()->draw(lifeBarBackground);
    RenderManager::getInstance()->draw(lifeBar);
}

void Boss::updateSpawners(float deltaTime) {
    if (currentPhaseIndex >= phases.size()) return;

    auto& spawners = phases[currentPhaseIndex].spawners;

    for (auto& spawner : spawners) {
        spawner.update(deltaTime);
        spawner.emitBullets();
    }
}

bool Boss::shouldTriggerBomb() const {
    return triggerBomb;
}

void Boss::resetBombTrigger() {
    triggerBomb = false;
}

bool Boss::bossIsDead() const { return state == BossState::Dead; }
const BoundingCircle& Boss::getBoundingCircle() const { return collision; }
const sf::Vector2f Boss::getPosition() const { return position; }

void Boss::setLifeBarTexture(const sf::Texture& texture) {
    lifeBarTexture = texture;
    lifeBarSprite.setTexture(lifeBarTexture);
}

void Boss::addPhase(const std::vector<BulletSpawner>& spawners) {
    phases.push_back(Phase{spawners});
}


Boss Boss::genBoss1() {
    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss boss(startPos, movement, 50.f, TENEBRIS, 99, 300.f); //Health bars (99) no se usa
    
    BulletEmitter omniemitter;
    omniemitter.setTrackPlayer(true, true);
    omniemitter.setCooldown(4.f);
    auto omnispawner = BulletSpawner(omniemitter,
                                    std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                    Bullet(ORBE_ANIMADO, BLUE, 50.f, 0.f),
                                    true); // Sigue al boss
    omnispawner.setPosition(startPos);

    // === FASE 1 ===

    BulletEmitter emitter1;
    emitter1.setNumBullets(32);
    emitter1.setConeAngle(360.f);
    emitter1.setCooldown(2.5f);
    emitter1.setRotation(2.f);

    auto spawner1 = BulletSpawner(emitter1,
                                    std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                    Bullet(ESFERA, ORANGE, 100.f, 0.f),
                                    true); // Sigue al boss
    spawner1.setPosition(startPos);

    // === FASE 2 ===
    std::vector<BulletSpawner> spawners2;

    const float startX = 64.f;
    const float endX = 380.f;
    const int numPerSide = 6;
    const float spacing = (endX - startX) / (numPerSide - 1);

    // Grupo 2: Spawners en las mismas posiciones pero con burst de 15
    for (int i = 0; i < numPerSide; ++i) {
        float x = startX + i * spacing;

        BulletEmitter burstEmitter;
        burstEmitter.setNumBullets(1);       // Un disparo por bala
        burstEmitter.setNumBurst(20);
        burstEmitter.setBurstCooldown(0.1f);
        burstEmitter.setBurstOriginOffsetVariation(5.f); // Corto intervalo entre cada una
        burstEmitter.setCooldown(6.f);       // Cooldown total entre ráfagas
        burstEmitter.setDirection({0, -1});

        BulletSpawner spawner(burstEmitter,
                            nullptr,
                            Bullet(MINI_LASER, 200.f, 0.f),
                            true);

        spawner.setPosition(sf::Vector2f(x, 8.f));
        spawners2.push_back(spawner);
    }

    // Spawner central (posición del jefe)
    BulletEmitter centerEmitter;
    centerEmitter.setNumBullets(5);
    centerEmitter.setConeAngle(40.f);
    centerEmitter.setCooldown(3.f);
    centerEmitter.setTrackPlayer(true);

    BulletSpawner centerSpawner(centerEmitter,
                                nullptr,
                                Bullet(ROUND_BULLET, RED, 200.f, 30.f),
                                true);

    centerSpawner.setPosition(startPos); // Suponiendo que startPos es el centro del boss
    spawners2.push_back(centerSpawner);
    spawners2.push_back(omnispawner);

    // === FASE 3 ===
    std::vector<BulletSpawner> spawners3;

    // Creamos un emitter independiente para evitar conflictos
    BulletEmitter emitter3;
    emitter3.setNumBullets(4);
    emitter3.setConeAngle(360.f);
    emitter3.setCooldown(0.4f);
    emitter3.setRotation(13.f);

    // Spawner en la izquierda
    BulletSpawner spawner3 = BulletSpawner(emitter3.clone(),
                                           std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                           Bullet(DISPARO, RED, 180.f, 200.f, 0.f, 180.f, 150.f),
                                           true);

    emitter3.setNumBullets(2);
    emitter3.setRotation(23.f);

    spawner3.setPosition(startPos); // Ahora sí se asigna correctamente
    spawners3.emplace_back(spawner3);

    // Spawner en la derecha
    BulletSpawner spawner4 = BulletSpawner(emitter3.clone(),
                                           std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                           Bullet(DISPARO, GREEN, 180.f, 200.f, 0.f, 180.f, 150.f),
                                           true);

    spawner4.setPosition(startPos); // No sobreescribe al anterior
    spawners3.emplace_back(spawner4);
    spawners3.emplace_back(omnispawner);

    boss.addPhase({spawner1, omnispawner});
    boss.addPhase(spawners2);
    boss.addPhase(spawners3);
    return boss;
}

Boss Boss::genSubBoss2() {
    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss subboss(startPos, movement, 20.f, HORTENSIA, 1, 150.f); // 1 barra, 150 vida

    // === FASE ÚNICA ===
    std::vector<BulletSpawner> phase;

    // Ataque de tijeras: Dos flujos convergentes desde los lados superiores
    {
        BulletEmitter scissorEmitter;
        scissorEmitter.setNumBullets(1);
        scissorEmitter.setNumBurst(10);
        scissorEmitter.setBurstCooldown(0.1f);
        scissorEmitter.setCooldown(3.f);
        scissorEmitter.setConeAngle(0.f);
        scissorEmitter.setTrackPlayer(true);

        // Izquierda: Dispara hacia abajo (90°)
        Bullet scissorBulletLeft(GOTA, GREEN, 90.f, 200.f, 0.f);
        BulletSpawner scissorSpawnerLeft(scissorEmitter.clone(),
                                        nullptr,
                                        scissorBulletLeft,
                                        true);
        scissorSpawnerLeft.setPosition({100.f, 50.f});
        phase.push_back(scissorSpawnerLeft);

        // Derecha: Dispara hacia abajo (90°)
        Bullet scissorBulletRight(GOTA, GREEN, 90.f, 200.f, 0.f);
        BulletSpawner scissorSpawnerRight(scissorEmitter.clone(),
                                         nullptr,
                                         scissorBulletRight,
                                         true);
        scissorSpawnerRight.setPosition({356.f, 50.f});
        phase.push_back(scissorSpawnerRight);
    }

    // Lluvia de pétalos: Disparo radial
    {
        BulletEmitter petalEmitter;
        petalEmitter.setNumBullets(8);
        petalEmitter.setConeAngle(360.f);
        petalEmitter.setCooldown(1.5f);
        petalEmitter.setRotation(15.f);
        petalEmitter.setChaosMode(true);
        petalEmitter.setChaosWindow({10.f, 10.f});

        Bullet petalBullet(GOTA, YELLOW, 150.f, 0.f);
        BulletSpawner petalSpawner(petalEmitter.clone(),
                                  nullptr,
                                  petalBullet,
                                  true);
        petalSpawner.setPosition(startPos);
        phase.push_back(petalSpawner);
    }

    // Enredaderas: Disparo continuo para simular línea
    {
        BulletEmitter vineEmitter;
        vineEmitter.setNumBullets(1);
        vineEmitter.setCooldown(0.2f); // Cooldown aumentado
        vineEmitter.setTrackPlayer(true, true);
        vineEmitter.setChaosMode(true);
        vineEmitter.setChaosWindow({10.f, 0.f});
        vineEmitter.setDirection({0, -1}); // Hacia abajo 
        

        Bullet vineBullet(CARAMBANO, GREEN, 90.f, 0.f);
        BulletSpawner vineSpawner(vineEmitter.clone(),
                                 nullptr,
                                 vineBullet,
                                 true);
        vineSpawner.setPosition(startPos);
        phase.push_back(vineSpawner);
    }

    subboss.addPhase(phase);
    return subboss;
}

Boss Boss::genBoss2() {

    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss boss(startPos, movement, 30.f, SpriteTemplate::CLOROTH, 4, 200.f); // 4 barras, 200 vida por barra

    // === FASE 1: Aspersores giratorios ===
    std::vector<BulletSpawner> phase1;
    {
        // Chorros desde abajo, girando como aspersores
        for (float x : {100.f, 356.f}) {
            BulletEmitter sprinklerEmitter;
            sprinklerEmitter.setNumBullets(1);
            sprinklerEmitter.setCooldown(0.1f); // Disparo constante
            sprinklerEmitter.setRotation(5.f); // Giro continuo
            sprinklerEmitter.setConeAngle(0.f);

            Bullet sprinklerBullet(GOTA, BLUE, 45.f, 250.f, 0.f); // Diagonal inicial
            BulletSpawner sprinklerSpawner(sprinklerEmitter.clone(),
                                          nullptr,
                                          sprinklerBullet,
                                          true);
            sprinklerSpawner.setPosition({x, 400.f}); // Desde abajo
            phase1.push_back(sprinklerSpawner);
        }

        // Burbujas radiales desde el jefe
        BulletEmitter bubbleEmitter;
        bubbleEmitter.setNumBullets(8);
        bubbleEmitter.setConeAngle(360.f);
        bubbleEmitter.setCooldown(1.f);
        bubbleEmitter.setRotation(10.f);
        bubbleEmitter.setChaosMode(true);
        bubbleEmitter.setChaosWindow({15.f, 15.f});

        Bullet bubbleBullet(ROUND_BULLET, WHITE, 150.f, 0.f);
        BulletSpawner bubbleSpawner(bubbleEmitter.clone(),
                                   nullptr,
                                   bubbleBullet,
                                   true);
        bubbleSpawner.setPosition(startPos);
        phase1.push_back(bubbleSpawner);
    }
    boss.addPhase(phase1);

    // === FASE 2: Olas de cloro ===
    std::vector<BulletSpawner> phase2;
    {
        // Olas desde los lados
        for (float x : {32.f, 424.f}) {
            BulletEmitter waveEmitter;
            waveEmitter.setNumBullets(3);
            waveEmitter.setConeAngle(30.f);
            waveEmitter.setCooldown(0.5f);
            waveEmitter.setDirection(x == 32.f ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f));

            Bullet waveBullet(CARAMBANO, GREEN, x == 32.f ? 0.f : 180.f, 200.f, 0.f);
            BulletSpawner waveSpawner(waveEmitter.clone(),
                                     nullptr,
                                     waveBullet,
                                     true);
            waveSpawner.setPosition({x, 200.f});
            phase2.push_back(waveSpawner);
        }

        // Chorros rastreadores
        BulletEmitter trackerEmitter;
        trackerEmitter.setNumBullets(1);
        trackerEmitter.setCooldown(0.3f);
        trackerEmitter.setTrackPlayer(true);
        trackerEmitter.setConeAngle(0.f);

        Bullet trackerBullet(MINI_LASER, BLUE, 0.f, 300.f, 0.f);
        BulletSpawner trackerSpawner(trackerEmitter.clone(),
                                    nullptr,
                                    trackerBullet,
                                    true);
        trackerSpawner.setPosition(startPos);
        phase2.push_back(trackerSpawner);
    }
    boss.addPhase(phase2);

    // === FASE 3: Red de limpieza giratoria ===
    std::vector<BulletSpawner> phase3;
    {
        // Red giratoria (balas curvas)
        BulletEmitter netEmitter;
        netEmitter.setNumBullets(4);
        netEmitter.setConeAngle(360.f);
        netEmitter.setCooldown(0.6f);
        netEmitter.setRotation(69.f);

        Bullet netBullet(GOTA, YELLOW, 0.f, 200.f, 1.5f, 180.f, 200.f); // Curva tras 0.5s
        BulletSpawner netSpawner(netEmitter.clone(),
                                nullptr,
                                netBullet,
                                true);
        netSpawner.setPosition(startPos);
        phase3.push_back(netSpawner);

        // Chorros alternantes desde abajo
        for (float x : {100.f, 356.f}) {
            BulletEmitter altEmitter;
            altEmitter.setNumBullets(1);
            altEmitter.setNumBurst(8);
            altEmitter.setBurstCooldown(0.15f);
            altEmitter.setCooldown(2.f);
            altEmitter.setConeAngle(30.f);
            altEmitter.setTrackPlayer(true);

            Bullet altBullet(CARAMBANO, BLUE, 270.f, 250.f, 0.f);
            BulletSpawner altSpawner(altEmitter.clone(),
                                    nullptr,
                                    altBullet,
                                    true);
            altSpawner.setPosition({x, 400.f});
            phase3.push_back(altSpawner);
        }
    }
    boss.addPhase(phase3);

    // === FASE 4: Manguera de presión y caos ===
    std::vector<BulletSpawner> phase4;
    {
        // Manguera rastreadora
        BulletEmitter hoseEmitter;
        hoseEmitter.setNumBullets(1);
        hoseEmitter.setNumBurst(20); // Dispara durante 2s (20 * 0.1f)
        hoseEmitter.setBurstCooldown(0.1f); // Rápido dentro de la ráfaga
        hoseEmitter.setCooldown(3.f); // Pausa de 3s tras ráfaga
        hoseEmitter.setTrackPlayer(true, true);
        hoseEmitter.setChaosMode(true);
        hoseEmitter.setChaosWindow({20.f, 0.f});
        hoseEmitter.setDirection({0, -1});

        Bullet hoseBullet(MINI_LASER, GREEN, 0.f, 400.f, 0.f);
        BulletSpawner hoseSpawner(hoseEmitter.clone(),
                                 nullptr,
                                 hoseBullet,
                                 true);
        hoseSpawner.setPosition(startPos);
        phase4.push_back(hoseSpawner);

        // Explosión de burbujas radiales
        BulletEmitter chaosEmitter;
        chaosEmitter.setNumBullets(12);
        chaosEmitter.setConeAngle(360.f);
        chaosEmitter.setCooldown(1.2f);
        chaosEmitter.setRotation(20.f);
        chaosEmitter.setChaosMode(true);
        chaosEmitter.setChaosWindow({30.f, 30.f});

        Bullet chaosBullet(ROUND_BULLET, WHITE, 180.f, 0.f);
        BulletSpawner chaosSpawner(chaosEmitter.clone(),
                                  nullptr,
                                  chaosBullet,
                                  true);
        chaosSpawner.setPosition(startPos);
        phase4.push_back(chaosSpawner);
    }
    boss.addPhase(phase4);

    return boss; 
}

Boss Boss::genBoss3() {
    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto inicialmente

    Boss boss(startPos, movement, 28.f, VESPERA, 60, 170.f); // HP, Sprite, TriggerHP

    // Fase 1: Onda expansiva rebotadora
    BulletEmitter emitter1;
    emitter1.setNumBullets(8);
    emitter1.setConeAngle(360.f);
    emitter1.setCooldown(0.4f);
    emitter1.setRotation(8.f);
    
    BulletSpawner spawner1(emitter1, nullptr, Bullet(ORBE_ANIMADO, BLACK, 150.f, 0.f), true);
    spawner1.setPosition(startPos);

    // === FASE 2 - Láseres direccionales ===
    std::vector<BulletSpawner> phase2;
    BulletEmitter phase2Emitter;
    phase2Emitter.setNumBullets(10);
    phase2Emitter.setConeAngle(30.f);
    phase2Emitter.setTrackPlayer(true);
    phase2Emitter.setCooldown(2.0f);
    
    Bullet laserBullet(LASER, GREY, 400.f, 0.f);
    for(int i = 0; i < 4; i++) {
        BulletSpawner spawner(
            phase2Emitter,
            nullptr,
            laserBullet,
            true
        );
        spawner.setPosition({startPos.x + i*40 - 60, startPos.y});
        phase2.push_back(spawner);
    }

    // Fase 3: Patrones en espiral
    BulletEmitter emitter3;
    emitter3.setNumBullets(2);
    emitter3.setConeAngle(360.f);
    emitter3.setCooldown(0.2f);
    emitter3.setRotation(10.f);

    BulletSpawner spawner3(emitter3,
                          nullptr,
                          Bullet(ESFERA, BLACK, 0.f, 150.f),
                          true);
    spawner3.setPosition(startPos);

    // === FASE 3 === - Campo de asteroides corregido
    std::vector<BulletSpawner> phaseThreeSpawners;
    
    // Corregimos los proyectiles grandes tipo asteroide
    BulletEmitter asteroidEmitter;
    asteroidEmitter.setNumBullets(1);
    asteroidEmitter.setNumBurst(3);
    asteroidEmitter.setBurstCooldown(0.5f);
    asteroidEmitter.setCooldown(3.2f);
    asteroidEmitter.setTrackPlayer(true, false); // Aseguramos que apuntan directamente al jugador
    
    // Usamos un constructor más simple sin rotación para evitar el "atasco"
    BulletSpawner asteroidSpawner(asteroidEmitter.clone(),
                                nullptr,
                                Bullet(MONEDA, GREY, 0.f, 120.f), // Sin giro, solo dirección directa
                                true);
    asteroidSpawner.setPosition(startPos);
    phaseThreeSpawners.push_back(asteroidSpawner);
    
    // Añadimos ataques direccionales desde múltiples ángulos
    for (int angle = 0; angle < 4; ++angle) {
        BulletEmitter dirEmitter;
        dirEmitter.setNumBullets(2);
        dirEmitter.setConeAngle(30.f);
        dirEmitter.setCooldown(1.2f);
        dirEmitter.setTrackPlayer(true, false); // Apunta al jugador
        
        // Diferentes tipos de proyectil según el ángulo
        SpriteTemplate sprite;
        ColorType color;
        
        switch (angle) {
            case 0: sprite = MINI_LASER; color = BLUE; break;
            case 1: sprite = BALA_DIMINUTA; color = RED; break;
            case 2: sprite = ROUND_BULLET; color = PURPLE; break;
            default: sprite = GOTA; color = ORANGE; break;
        }
        
        BulletSpawner dirSpawner(dirEmitter.clone(),
                                nullptr,
                                Bullet(sprite, color, 0.f, 200.f + angle * 15.f), // Velocidades variadas
                                true);
        
        // Posicionamos los emisores en diferentes puntos alrededor del centro
        float angleRad = angle * 3.14159f / 2.f; // 0, 90, 180, 270 grados
        float offsetX = 80.f * std::cos(angleRad);
        float offsetY = 80.f * std::sin(angleRad);
        
        dirSpawner.setPosition({startPos.x + offsetX, startPos.y + offsetY});
        phaseThreeSpawners.push_back(dirSpawner);
    }
    
    // Agregamos una onda expansiva periódica
    BulletEmitter waveEmitter;
    waveEmitter.setNumBullets(16);  // 16 balas en 360 grados
    waveEmitter.setConeAngle(360.f);
    waveEmitter.setCooldown(4.0f);  // Cada 4 segundos
    
    BulletSpawner waveSpawner(waveEmitter.clone(),
                            nullptr,
                            Bullet(CUCHILLO, YELLOW, 0.f, 180.f),
                            true);
    waveSpawner.setPosition(startPos);
    phaseThreeSpawners.push_back(waveSpawner);

    // === FASE 4 === - Elipse temporal
    std::vector<BulletSpawner> phaseFourSpawners;
    
    for (int ring = 0; ring < 3; ++ring) {
        BulletEmitter elipseEmitter;
        elipseEmitter.setNumBullets(12);
        elipseEmitter.setConeAngle(360.f);
        elipseEmitter.setCooldown(1.8f + ring * 0.4f);
        elipseEmitter.setRotation(20.f - ring * 15.f);
        
        ColorType ringColor;
        switch (ring) {
            case 0: ringColor = RED; break;
            case 1: ringColor = ORANGE; break;
            default: ringColor = YELLOW; break;
        }
        
        BulletSpawner elipseSpawner(elipseEmitter.clone(),
                                   nullptr,
                                   Bullet(ROUND_BULLET, ringColor, 0.f, 300.f, 0.8f, 180.f, 180.f),
                                   true);
        elipseSpawner.setPosition(startPos);
        phaseFourSpawners.push_back(elipseSpawner);
    }
    
    BulletEmitter chaosEmitter;
    chaosEmitter.setNumBullets(1);
    chaosEmitter.setNumBurst(8);
    chaosEmitter.setBurstCooldown(0.15f);
    chaosEmitter.setCooldown(3.5f);
    chaosEmitter.setChaosMode(true);
    chaosEmitter.setChaosWindow({200.f, 150.f});
    
    BulletSpawner chaosSpawner(chaosEmitter.clone(),
                              nullptr,
                              Bullet(GOTA, BLUE, 0.f, 180.f, 0.4f, 270.f, 180.f),
                              true);
    chaosSpawner.setPosition({startPos.x, startPos.y - 30.f});
    phaseFourSpawners.push_back(chaosSpawner);
    
    BulletEmitter finalEmitter;
    finalEmitter.setNumBullets(3);
    finalEmitter.setConeAngle(30.f);
    finalEmitter.setCooldown(2.4f);
    finalEmitter.setTrackPlayer(true, false);
    
    BulletSpawner finalSpawner(finalEmitter.clone(),
                             nullptr,
                             Bullet(CUCHILLO, RED, 0.f, 260.f),
                             true);
    finalSpawner.setPosition(startPos);
    phaseFourSpawners.push_back(finalSpawner);
    
    // Añadimos las fases al jefe
    boss.addPhase({spawner1});
    boss.addPhase(phase2);
    boss.addPhase(phaseThreeSpawners);
    boss.addPhase(phaseFourSpawners);
    
    return boss;
}

Boss Boss::genSubBoss4() {
    sf::Vector2f startPos(228.f, 100.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss subboss(startPos, movement, 15.f, SOUSA, 30, 100.f); // Vida, sprite, triggerHP

    // === FASE 1 ===
    std::vector<BulletSpawner> phase1;

    for (float offset : {-10.f, 10.f}) {
        BulletEmitter burstEmitter;
        burstEmitter.setNumBullets(1);               // Un cuchillo por burst
        burstEmitter.setNumBurst(12);                 // 4 ráfagas instantáneas
        burstEmitter.setBurstCooldown(0.1f);          // Sin tiempo entre ráfagas
        burstEmitter.setCooldown(1.f);               // Cada 2 segundos
        burstEmitter.setTrackPlayer(true);        // Hacia abajo
        burstEmitter.setBurstOffset({offset, 0.f}); // Espaciado entre ráfagas
        burstEmitter.setConeAngle(0.f);              // Recto
    
        Bullet knife(CUCHILLO, offset <0 ? RED : GREEN, 90.f, 300.f, 0.f);
    
        BulletSpawner knifeSpawner(burstEmitter.clone(),
                                   nullptr,
                                   knife,
                                   true);
        knifeSpawner.setPosition({startPos.x + offset, startPos.y + 32.f});
        phase1.push_back(knifeSpawner);
    }

    // Rastreador
    BulletEmitter tracker;
    tracker.setNumBullets(1);
    tracker.setCooldown(1.2f);
    tracker.setTrackPlayer(true);
    tracker.setConeAngle(0.f);

    BulletSpawner trackingSpawner(tracker.clone(),
                                  nullptr,
                                  Bullet(MONEDA, WHITE, 90.f, 220.f, 0.f),
                                  true);

    trackingSpawner.setPosition(startPos);
    phase1.push_back(trackingSpawner);

    subboss.addPhase(phase1);

    // === FASE 2 ===
    std::vector<BulletSpawner> phase2;
    std::vector<float> leftSideX  = {32.f, 64.f, 96.f};
    std::vector<float> rightSideX = {328.f, 376.f, 424.f};

    for (float x : leftSideX) {
        BulletEmitter emitter;
        emitter.setNumBullets(1);
        emitter.setNumBurst(12);
        emitter.setConeAngle(10.f);
        emitter.setCooldown(0.2f);                  // Continuo
        emitter.setTrackPlayer(true);
        emitter.setChaosMode(true);
        emitter.setChaosWindow({30.f, 0.f});


        // Bala con giro a la derecha después de 0.3s
        Bullet bullet(GOTA, ORANGE, 200.f, 180.f, 0.8f, 90.f, 140.f);

        BulletSpawner spawner(emitter.clone(),
                            nullptr,
                            bullet,
                            true);
        spawner.setPosition({x, startPos.y + 32.f});
        phase2.push_back(spawner);
    }

    for (float x : rightSideX) {
        BulletEmitter emitter;
        emitter.setNumBullets(1);
        emitter.setNumBurst(12);
        emitter.setConeAngle(10.f);
        emitter.setCooldown(0.2f);                  // Continuo
        emitter.setTrackPlayer(true, true);
        emitter.setChaosMode(true);
        emitter.setChaosWindow({30.f, 0.f});

        // Bala con giro a la izquierda después de 0.3s
        Bullet bullet(GOTA, YELLOW, 200.f, -180.f, 0.8f, -90.f, 140.f);

        BulletSpawner spawner(emitter.clone(),
                            nullptr,
                            bullet,
                            true);
        spawner.setPosition({x, startPos.y + 32.f});
        phase2.push_back(spawner);
    }

    for (float offset : {-10.f, 10.f}) {
        BulletEmitter burstEmitter;
        burstEmitter.setNumBullets(1);               // Un cuchillo por burst
        burstEmitter.setNumBurst(4);                 // 4 ráfagas instantáneas
        burstEmitter.setBurstCooldown(0.f);          // Sin tiempo entre ráfagas
        burstEmitter.setCooldown(2.f);               // Cada 2 segundos
        burstEmitter.setTrackPlayer(true);        // Hacia abajo
        burstEmitter.setBurstOffset({offset, 0.f}); // Espaciado entre ráfagas
        burstEmitter.setConeAngle(0.f);              // Recto
    
        Bullet knife(CUCHILLO, RED, 90.f, 300.f, 0.f);
    
        BulletSpawner knifeSpawner(burstEmitter.clone(),
                                   nullptr,
                                   knife,
                                   true);
        knifeSpawner.setPosition({startPos.x + offset, startPos.y + 32.f});
        phase2.push_back(knifeSpawner);
    }
    

    subboss.addPhase(phase2);
    return subboss;
}

Boss Boss::genBoss4() {
    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss boss(startPos, movement, 30.f, GODOLFO, 70, 180.f); // HP, Sprite, TriggerHP

    // === FASE 1 === - Disparo horizontal clásico + soporte lateral
    std::vector<BulletSpawner> angularSpawners;

    for (int i = 0; i < 3; ++i) {
        float angleDeg = 180.f + i * 30.f;
        float bulletAngle = angleDeg + 180.f; // Corrige la dirección del bullet
        float angleRad = angleDeg * 3.14159265f / 180.f;

        sf::Vector2f dir(std::cos(angleRad), std::sin(angleRad));

        BulletEmitter emitter;
        emitter.setNumBullets(2);
        emitter.setConeAngle(30);
        emitter.setDirection(dir);
        emitter.setCooldown(0.5f);
        emitter.setRotation(10 + i * 5);

        ColorType color;
        if (i == 0) color = RED;
        else if (i == 1) color = ORANGE;
        else color = YELLOW;

        BulletSpawner spawner(emitter.clone(),
                            nullptr,
                            Bullet(MINI_LASER, color, bulletAngle, 200.f, 0.f),
                            true);
        spawner.setPosition(startPos);
        angularSpawners.push_back(spawner);
    }

    BulletEmitter supportEmitter;
    supportEmitter.setNumBullets(1); // 1 por ráfaga, 10 ráfagas
    supportEmitter.setNumBurst(20);
    supportEmitter.setBurstCooldown(0.f);
    supportEmitter.setTrackPlayer(true, true);
    supportEmitter.setChaosMode(true);
    supportEmitter.setChaosWindow({120.f, 120.f});
    supportEmitter.setCooldown(4.f);

    // Spawner izquierdo
    BulletSpawner sideSpawnerLeft(supportEmitter,
                                nullptr,
                                Bullet(BALA_DIMINUTA, WHITE, 120.f, 80.f),
                                true);
    sideSpawnerLeft.setPosition({startPos.x - 180.f, 32.f});

    // Spawner derecho
    BulletSpawner sideSpawnerRight(supportEmitter,
                                nullptr,
                                Bullet(BALA_DIMINUTA, BLACK, 120.f, 80.f),
                                true);
    sideSpawnerRight.setPosition({startPos.x + 180.f, 32.f});

    angularSpawners.push_back(sideSpawnerLeft);
    angularSpawners.push_back(sideSpawnerRight);

    // === FASE 2 === - Disparo dirigido al jugador con ángulo
    std::vector<BulletSpawner> spawners2;

    for (int i = 0; i < 6; ++i) {
        BulletEmitter emitter;
        emitter.setNumBullets(1);
        emitter.setCooldown(0.4f);
        emitter.setRotation(8.f + i); // ligera variación por spawner
        emitter.setDirection({0, -1}); // ligera variación por spawner
    
        // Valores base
        float baseDir = 0.f;
        float speed = 500.f;
        float triggerTime = 0.1f;
        float degreesToTurn = 1000.f + 30.f * i;       // 1080° → 1130°
        float turnSpeed = 500.f + 20.f * i;            // 200 → 300
    
        Bullet bullet(ROUND_BULLET, RED, baseDir, speed, triggerTime, degreesToTurn, turnSpeed);
    
        BulletSpawner spawner(emitter.clone(),
                              nullptr,  // Movimiento lo gestiona el bullet
                              bullet,
                              true);
        spawner.setPosition(startPos);
        spawners2.push_back(spawner);
    }

    // === FASE 3 === - Radial giratorio
    BulletEmitter emitter3;
    emitter3.setNumBullets(5);
    emitter3.setConeAngle(360.f);
    emitter3.setCooldown(0.4f);
    emitter3.setRotation(9.f);

    BulletEmitter emitter4;
    emitter4.setNumBullets(2);
    emitter4.setConeAngle(360.f);
    emitter4.setCooldown(0.2f);
    emitter4.setRotation(-37.f);

    BulletSpawner spawner3(emitter3,
                           nullptr,
                           Bullet(FANTASMA_ANIMADO, RED, 300.f, -120.f, -300.f, 300.f),
                           true);

    BulletSpawner spawner4(emitter4,
                           nullptr,
                           Bullet(CUCHILLO, BLACK, 200.f, -30.f),
                           true);
    spawner3.setPosition(startPos);
    spawner4.setPosition(startPos);

    // === FASE 4 === - Láseres lentos + balas curvas
    std::vector<BulletSpawner> spawners5;

    for (int i = 4; i > 0; --i) {
        BulletEmitter burstEmitter;
        burstEmitter.setNumBullets(3);
        burstEmitter.setNumBurst(6);
        burstEmitter.setBurstRotation(6.f);
        burstEmitter.setConeAngle(360.f);
        burstEmitter.setCooldown(0.f);
        burstEmitter.setRotation(12.f);
        burstEmitter.setBurstOriginOffsetVariation(i * 3.f);

        BulletSpawner spawner(burstEmitter.clone(),
                            nullptr,
                            Bullet(CUCHILLO, BLACK, 0.f, 500.f),
                            true);

        spawner.setPosition({startPos.x, startPos.y + 100});
        spawners5.push_back(spawner);
    }

    for (int i = 0; i < 12; ++i) {
        BulletEmitter turningEmitter;
        turningEmitter.setNumBullets(1);
        turningEmitter.setCooldown(1.f);
        turningEmitter.setDirection({-1, 0});
    
        float degreesToTurn = 720.f + i * 30.f;
    
        Bullet turnBullet(ROUND_BULLET, RED, 0.f, 550.f, 0.f, degreesToTurn, 160.f);
    
        BulletSpawner turningSpawner(turningEmitter,
                                     nullptr,
                                     turnBullet,
                                     true);
    
        turningSpawner.setPosition(startPos);
        spawners5.push_back(turningSpawner);
    }

    // === FASE 5 === - Presión total, mezcla de dirigido y radial
    std::vector<BulletSpawner> spawners6;

    // Inicialización del generador de números aleatorios con semilla fija
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> xDist(32.f, 440.f);
    std::uniform_real_distribution<float> cooldownDist(2.f, 10.f);
    std::uniform_real_distribution<float> dirYDist(-0.8f, -1.2f);
    std::uniform_int_distribution<int> numBulletsDist(1, 3);
    std::uniform_int_distribution<int> spriteDist(0, 5);
    std::uniform_int_distribution<int> colorDist(0, 5);
    
    // Opciones disponibles
    std::vector<SpriteTemplate> spriteOptions = {
        CUCHILLO, ROUND_BULLET, MONEDA, BALA_DIMINUTA, GOTA, CARAMBANO
    };
    
    std::vector<ColorType> colorOptions = {
        RED, BLACK, WHITE, ORANGE, RED, YELLOW
    };
    
    for (int i = 0; i < 24; ++i) {
        float xPos = xDist(rng);
        float cooldown = cooldownDist(rng);
        int numBullets = numBulletsDist(rng);
        float dirY = dirYDist(rng);
        sf::Vector2f direction(0.f, dirY);
    
        SpriteTemplate sprite = spriteOptions[spriteDist(rng)];
        ColorType color = colorOptions[colorDist(rng)];
    
        BulletEmitter emitter;
        emitter.setNumBullets(numBullets);
        emitter.setCooldown(cooldown);
        emitter.setDirection(direction);
        emitter.setConeAngle(30.f);
        
        Bullet bullet(sprite, color, 50.f, -150.f, -400.f, 50.f); // puedes usar esto si bullet lleva el movement dentro
    
        BulletSpawner spawner(emitter.clone(),
                              nullptr,
                              bullet,
                              true);
    
        spawner.setPosition({xPos, startPos.y});
        spawners6.push_back(spawner);
    }
    
    boss.addPhase(angularSpawners);
    boss.addPhase(spawners2);
    boss.addPhase({spawner3, spawner4});
    boss.addPhase(spawners5);
    boss.addPhase(spawners6);
    
    return boss;
}


Boss Boss::genBoss5() {
    sf::Vector2f startPos(228.f, 90.f);
    auto movement = std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f); // Quieto

    Boss boss(startPos, movement, 30.f, THRENODY, 99, 200.f);

    // === FASE 1 ===
    BulletEmitter emitter1;
    emitter1.setNumBullets(8);
    emitter1.setConeAngle(360.f);
    emitter1.setCooldown(0.4f);
    emitter1.setRotation(13.f);
    emitter1.setChaosMode(true);
    emitter1.setChaosWindow({20.f, 20.f});

    auto spawner1 = BulletSpawner(emitter1,
                                  std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                  Bullet(NOTA_ANIMADO, YELLOW, 150.f, 0.f),
                                  true); // Sigue al boss
    spawner1.setPosition(startPos);

    // === FASE 2 ===
    std::vector<BulletSpawner> spawners2;

    for (int y = 64; y <= 448; y += 48) {
        BulletEmitter emitter;
        emitter.setNumBullets(1);
        emitter.setCooldown(0.2f);

        auto spawner = BulletSpawner(emitter,
                                     std::make_shared<DirectionalMovement>(180.f, 100.f, 0.f),
                                     Bullet(MINI_LASER, 200.f, 0.f),
                                     true);

        spawner.setPosition(sf::Vector2f(424.f, y));
        spawners2.push_back(spawner);
    }

    BulletEmitter emitter2;
    emitter2.setNumBullets(3);
    emitter2.setNumBurst(12);
    emitter2.setChaosMode(true);
    emitter2.setChaosWindow({40, 0});
    emitter2.setConeAngle(15.f);
    emitter2.setCooldown(2.f);
    emitter2.setTrackPlayer(true);

    auto spawner2 = BulletSpawner(emitter2,
                                  std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                  Bullet(ROUND_BULLET, BLACK, 500.f, 0.f),
                                  true); // También desde el boss

    spawner2.setPosition(startPos);
    spawners2.push_back(spawner2);

    // === FASE 3 ===
    std::vector<BulletSpawner> spawners3;

    // Creamos un emitter independiente para evitar conflictos
    BulletEmitter emitter3;
    emitter3.setNumBullets(8);
    emitter3.setConeAngle(360.f);
    emitter3.setCooldown(0.4f);
    emitter3.setRotation(13.f);
    emitter3.setChaosMode(true);
    emitter3.setChaosWindow({20.f, 20.f});

    // Spawner en la izquierda
    BulletSpawner spawner3 = BulletSpawner(emitter3,
                                           std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                           Bullet(NOTA_ANIMADO, RED, 250.f, -50.f),
                                           true);

    spawner3.setPosition({96.f, 0.f}); // Ahora sí se asigna correctamente
    spawners3.emplace_back(spawner3);

    // Spawner en la derecha
    BulletSpawner spawner4 = BulletSpawner(emitter3,
                                           std::make_shared<DirectionalMovement>(0.f, 0.f, 0.f),
                                           Bullet(NOTA_ANIMADO, GREEN, 250.f, -50.f),
                                           true);

    spawner4.setPosition({360.f, 0.f}); // No sobreescribe al anterior
    spawners3.emplace_back(spawner4);

    // === FASE 4 ===
    std::vector<BulletSpawner> spawners4;

    // Posiciones iniciales de los Spawners
    sf::Vector2f leftSpawnerPos = startPos + sf::Vector2f(-20.f, 0.f);
    sf::Vector2f rightSpawnerPos = startPos + sf::Vector2f(20.f, 0.f);

    // Emisor de balas para ambos Spawners
    BulletEmitter baseEmitter;
    baseEmitter.setNumBullets(1);
    baseEmitter.setCooldown(0.3f);
    baseEmitter.setChaosMode(true);
    baseEmitter.setChaosWindow({60, 120});
    baseEmitter.setDirection({-1,1});

    // Spawner Izquierdo - Dispara hacia la izquierda y curva 180º
    BulletSpawner leftSpawner(baseEmitter,
                            nullptr, // Ya no necesita MovementStrategy, lo define el Bullet
                            Bullet(MINI_LASER, 180.f, 500.f, 0.f, 180.f, 250.f),
                            true);

    leftSpawner.setPosition(leftSpawnerPos);
    spawners4.push_back(leftSpawner);

    BulletEmitter baseEmitterCopy = baseEmitter.clone();
    baseEmitterCopy.setDirection({1,1});

    // Spawner Derecho - Dispara hacia la derecha y curva 180º
    BulletSpawner rightSpawner(baseEmitterCopy,
                            nullptr, // Ya no necesita MovementStrategy, lo define el Bullet
                            Bullet(MINI_LASER, 0.f, 500.f, 0.f, -180.f, 250.f),
                            true);

    rightSpawner.setPosition(rightSpawnerPos);
    spawners4.push_back(rightSpawner);

    BulletEmitter radialEmitter;
    radialEmitter.setNumBullets(6);
    radialEmitter.setCooldown(0.8f);
    radialEmitter.setNumBurst(4);
    radialEmitter.setBurstRotation(3);
    radialEmitter.setBurstCooldown(0.f);
    radialEmitter.setRotation(12);

    BulletSpawner radialSpawner(radialEmitter,
                                nullptr,
                                Bullet(NOTA_SILENCIO, PURPLE, 160.f, 60.f),
                                true);

    radialSpawner.setPosition(startPos);
    spawners4.push_back(radialSpawner);

    // === FASE 5 ===
    std::vector<BulletSpawner> spawners5;
    spawners5.push_back(radialSpawner);
    spawners5.push_back(spawner1);

    // === FASE 6 ===
    std::vector<BulletSpawner> spawners6;

    // Emisor base
    BulletEmitter fastEmitter;
    fastEmitter.setNumBullets(1);
    fastEmitter.setCooldown(0.1f);    // Rápido disparo
    fastEmitter.setChaosMode(true);
    fastEmitter.setChaosWindow({30.f, 0.f}); // Chaos en X
    fastEmitter.setDirection({0, -1});

    float spacing = 40.f;

    // Grupo 1 (32, 16) -> (152, 16)
    float startX1 = 48.f;
    for (int i = 0; i < 3; ++i) {
        float xPos = startX1 + (i * spacing);

        BulletSpawner spawner(fastEmitter.clone(),
                            std::make_shared<DirectionalMovement>(90.f, 500.f, 0.f),
                            Bullet(NOTA_SILENCIO, PURPLE, 500.f, 0.f),
                            true);
        
        spawner.setPosition(sf::Vector2f(xPos, 16.f));
        spawners6.push_back(spawner);
    }

    // Grupo 2 (304, 16) -> (424, 16)
    float startX2 = 330.f;
    for (int i = 0; i < 3; ++i) {
        float xPos = startX2 + (i * spacing);

        BulletSpawner spawner(fastEmitter.clone(),
                            std::make_shared<DirectionalMovement>(90.f, 500.f, 0.f),
                            Bullet(NOTA_SILENCIO, PURPLE, 500.f, 0.f),
                            true);
        
        spawner.setPosition(sf::Vector2f(xPos, 16.f));
        spawners6.push_back(spawner);
    }


    // Emisor base para los ocultos
    BulletEmitter hiddenEmitter;
    hiddenEmitter.setNumBullets(1);
    hiddenEmitter.setTrackPlayer(true, true);

    // Lista de posiciones estratégicas
    std::vector<sf::Vector2f> hiddenPositions = {
        {20.f, 200.f},
        {10.f, 300.f},
        {14.f, 180.f},
        {31.f, 220.f},
        {415.f, 380.f},
        {410.f, 140.f},
        {420.f, 260.f}
    };

    // Lista de cooldowns para variar los disparos
    std::vector<float> cooldowns = {5.f, 10.f, 15.f, 7.5f, 12.5f, 6.f, 12.f};

    // Creación de los Spawners
    for (size_t i = 0; i < hiddenPositions.size(); ++i) {
        BulletEmitter emitter = hiddenEmitter.clone();
        emitter.setCooldown(cooldowns[i]);

        BulletSpawner hiddenSpawner(emitter,
                                    std::make_shared<DirectionalMovement>(270.f, 300.f, 0.f),
                                    Bullet(NOTA_ANIMADO, PURPLE, 200.f, 0.f),
                                    true);

        hiddenSpawner.setPosition(hiddenPositions[i]);
        spawners6.push_back(hiddenSpawner);
    }

    hiddenEmitter.setNumBullets(3);
    hiddenEmitter.setConeAngle(30);
    hiddenEmitter.setCooldown(2.f);
    hiddenEmitter.setTrackPlayer(true, false);

    BulletSpawner bossSpawner(hiddenEmitter, std::make_shared<DirectionalMovement>(0.f, 300.f, 0.f), Bullet(ESFERA_GRANDE, WHITE, 200.f, 0.f), true);
    bossSpawner.setPosition(startPos);
    spawners6.push_back(bossSpawner);

    // === Añadimos todas las fases al boss ===
    boss.addPhase({spawner1});
    boss.addPhase(spawners2);
    boss.addPhase(spawners3);
    boss.addPhase(spawners4);
    boss.addPhase(spawners5);
    boss.addPhase(spawners6);
    
    return boss;
}

size_t Boss::getCurrentPhaseIndex() const {
    return currentPhaseIndex;
}