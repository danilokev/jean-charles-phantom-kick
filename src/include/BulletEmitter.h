#ifndef BULLETEMITTER_H
#define BULLETEMITTER_H

#include "Bullet.h"
#include <SFML/System.hpp>
#include <vector>

/**
 * @class BulletEmitter
 * @brief Clase responsable de emitir balas para enemigos en un bullet hell.
 */
class BulletEmitter {
public:
    BulletEmitter();

    /// Reinicia el temporizador (útil si eliges la opción 1)
    void resetTimer(float offset = 0.f);

    /** @brief Dispara un conjunto de balas desde el origen usando una plantilla. */
    void emit(const sf::Vector2f& origin, const Bullet& templateBullet);

    // --- Setters Generales ---

    /** @brief Establece el ángulo del cono de disparo en grados. */
    void setConeAngle(float angle);

    /** @brief Establece el número de balas por emisión. */
    void setNumBullets(int count);

    /** @brief Establece la dirección base del disparo. */
    void setDirection(const sf::Vector2f& dir);

    /** @brief Habilita o deshabilita el seguimiento al jugador. */
    void setTrackPlayer(bool enable, bool revert=false);

    /** @brief Establece la distancia de desplazamiento respecto al centro de disparo. */
    void setOffsetDistance(float d);

    /** @brief Establece el tiempo de recarga entre disparos. */
    void setCooldown(float seconds);

    /** @brief Establece la rotación constante del patrón de disparo. */
    void setRotation(float rot);

    // --- Setters de Ráfagas (Burst) ---

    /** @brief Establece el número de balas dentro de una ráfaga. */
    void setNumBurst(int count);

    /** @brief Establece el tiempo entre balas dentro de una ráfaga. */
    void setBurstCooldown(float seconds);

    /** @brief Establece la rotación adicional por bala dentro del burst. */
    void setBurstRotation(float degrees);

    /** @brief Establece la variación en la posición relativa por bala dentro del burst. */
    void setBurstOffset(const sf::Vector2f& offset);

    /** @brief Establece la variación del ángulo del cono entre balas dentro del burst. */
    void setBurstConeVariation(float degrees);

    /** @brief Establece la cantidad adicional de balas por iteración del burst. */
    void setBurstBulletAdd(int count);

    /** @brief Establece la variación del offset respecto al origen por bala del burst. */
    void setBurstOriginOffsetVariation(float d);

    // --- Estado y lógica de tiempo ---

    /** @brief Actualiza el estado del emisor. */
    void update(float deltaTime);

    /** @brief Indica si se puede disparar actualmente. */
    bool canEmit() const;

    /** @brief Activa o desactiva el modo caos.*/
    void setChaosMode(bool enable);

    /** @brief Establece la ventana de offset para el modo caos. */
    void setChaosWindow(const sf::Vector2f& window);

    BulletEmitter clone() const;

private:
    // --- Configuración de Disparo General ---
    float coneAngle = 0.f;
    int numBullets = 1;
    sf::Vector2f direction;
    bool bulletTracksPlayer = false;
    float offsetDistance = 0.f;
    float cooldown = 1.f;
    // No tocar
    float timer = 0.f;

    // --- Rotación del patrón ---
    float rotation = 0.f;

    // --- Parámetros de Ráfaga (Burst) ---
    int numBurst = 1;
    float burstCooldown = 0.1f;
    //No tocar
    float burstTimer = 0.f;
    bool inBurst = false;
    int burstCount = 0;

    float burstRotation = 0.f;
    sf::Vector2f burstOffset;
    float burstConeVariation = 0.f;
    int burstBulletAdd = 0;
    float burstOriginOffsetVariation = 0.f;

    // --- Almacenamiento temporal durante burst ---
    sf::Vector2f burstDirection;
    sf::Vector2f burstPosition;
    float burstConeAngle = 0.f;
    int burstBulletNum = 0;
    float burstOriginOffset = 0.f;

    // --- Modo caos ---
    bool chaosMode = false;                 // Whether chaos mode is active.
    sf::Vector2f chaosWindow = {5.f, 5.f};  // Chaos offset range (x: perpendicular, y: forward).


    /** @brief Normaliza un vector. */
    sf::Vector2f normalize(const sf::Vector2f& v);

    bool revert = false;
};

#endif
