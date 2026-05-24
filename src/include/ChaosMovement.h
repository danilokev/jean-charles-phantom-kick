#ifndef CHAOSMOVEMENT_H
#define CHAOSMOVEMENT_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include "MovementStrategy.h"
#include <random>
#include <memory>

class ChaosMovement : public MovementStrategy {
public:
  /**
   * bounds: rectángulo que delimita el área de movimiento
   * speed: velocidad de desplazamiento
   * targetThreshold: distancia a la  que se considera que se ha llegado al objetivo
   * waitTime: tiempo de espera antes de seleccionar una nueva posición
   */
  ChaosMovement(const sf::FloatRect& bounds, float speed = 2.0f, float targetThreshold = 10.0f, float waitTime = 0.5f);

  // Actualizar el vector de movimiento
  virtual void update(float dt) override;

  // Devolver vector de movimiento que se aplica a la entidad
  virtual sf::Vector2f getMovementVector() const override;

  // Establecer posición actual de la entidad
  void setCurrentPosition(const sf::Vector2f& pos);

  // Establecer área de movimiento
  void setBounds(const sf::FloatRect& bounds);

  // Establecer velocidad de movimeinto
  void setSpeed(float speed);

  // Obtener velocidad actual
  float getSpeed() const;

  // No hace nada
  virtual void setDirection(float direction) override;

  virtual std::shared_ptr<MovementStrategy> clone() const override;

private:
  sf::FloatRect bounds;   
  float speed;
  float targetThreshold;
  float waitTime;
  float count;                  // contador tiempo espera

  sf::Vector2f currentPosition; // Posición actual entidad
  sf::Vector2f targetPosition;  // Posición objetivo
  sf::Vector2f movementVector;  // Vector movimiento resultante
  bool hasReachedTarget;        // Si se ha alcanzado el objetivo

  std::mt19937 rng;             // Generador números aleatorios

  // Seleccionar nueva posición objetivo aleatoria
  void selectNewTarget();
};

#endif