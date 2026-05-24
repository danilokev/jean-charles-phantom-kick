#ifndef BOUNDINGCIRCLE_H
#define BOUNDINGCIRCLE_H

#include <SFML/Graphics.hpp>

class BoundingCircle {
public:
  BoundingCircle();
  BoundingCircle(const sf::Vector2f& c, float r);

  // verifica si este circulo colisiona con otro 
  bool collidesWith(const BoundingCircle& o) const;

  void setCenter(const sf::Vector2f& c);
  void setRadius(float r);
  sf::Vector2f getCenter() const;
  float getRadius() const;

  // dibujar círculo para hacer pruebas
  void draw(sf::RenderTarget& t, const sf::Color& c = sf::Color::White) const;

private:
  sf::Vector2f center;  // Centro círculo (coords: x,y)
  float radius;         // radio círculo
};

#endif