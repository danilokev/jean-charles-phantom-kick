#include "../include/ChaosMovement.h"
#include <cmath>
#include <ctime>

ChaosMovement::ChaosMovement(const sf::FloatRect &bounds, float speed, float targetThreshold, float waitTime)
    : bounds(bounds),
      speed(speed),
      targetThreshold(targetThreshold),
      waitTime(waitTime),
      count(0.0f),
      currentPosition(0.0f, 0.0f),
      targetPosition(0.0f, 0.0f),
      movementVector(0.0f, 0.0f),
      hasReachedTarget(true)
{
  // Inicializamos el generador de números aleatorios con una semilla basada en el tiempo actual
  rng = std::mt19937(static_cast<unsigned int>(std::time(nullptr)));
   
  selectNewTarget();
}

void ChaosMovement::update(float dt) {
  if (hasReachedTarget) {
    // si ya llegamos al objetivo, esperamos antes de seleccionar otro
    count += dt; 

    if (count >= waitTime) {
      selectNewTarget();
      count = 0.0f;
      hasReachedTarget = false;
    }

    // No hay movimiento mientras esperamos
    movementVector = sf::Vector2f(0.0f, 0.0f);
  } else {
    // Calculamos dirección hacia el objetivo
    sf::Vector2f direction = targetPosition - currentPosition;

    // Calculamos distancia al objetivo
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Comprobar si hemos llegado al objetivo
    if (distance <= targetThreshold) {
      hasReachedTarget = true;
      movementVector = sf::Vector2f(0.0f, 0.0f);
    } else {
      if (distance > 0) {
        direction /= distance;
      }

      movementVector = direction * speed;
    }
  }
}

sf::Vector2f ChaosMovement::getMovementVector() const {
  return movementVector;
}

void ChaosMovement::setCurrentPosition(const sf::Vector2f& position) {
  currentPosition = position;
}

void ChaosMovement::setBounds(const sf::FloatRect& newBounds) {
  bounds = newBounds;
}

void ChaosMovement::setSpeed(float newSpeed) {
  speed = newSpeed;
}

float ChaosMovement::getSpeed() const {
  return speed;
}

void ChaosMovement::setDirection(float direction) {
  direction++;
  // Esto no hace nada
} 

std::shared_ptr<MovementStrategy> ChaosMovement::clone() const {
  return std::make_shared<ChaosMovement>(bounds, speed, targetThreshold, waitTime);
}

void ChaosMovement::selectNewTarget()
{
    // Creamos distribuciones uniformes para x e y dentro de los límites
    std::uniform_real_distribution<float> distanceX(bounds.left, bounds.left + bounds.width);
    std::uniform_real_distribution<float> distanceY(bounds.top, bounds.top + bounds.height);

    // Generamos nueva posición objetivo aleatoria
    targetPosition.x = distanceX(rng);
    targetPosition.y = distanceY(rng);
}