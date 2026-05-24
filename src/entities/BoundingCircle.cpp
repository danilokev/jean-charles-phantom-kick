#include "../include/BoundingCircle.h"

BoundingCircle::BoundingCircle() 
  : center(0.f, 0.f), radius(0.f) {}

BoundingCircle::BoundingCircle(const sf::Vector2f& c, float r) 
  : center(c),
    radius(r) {}

bool BoundingCircle::collidesWith(const BoundingCircle& other) const {
  // calculamos la diferencia en las coords (x, y) entre los centros
  float dx = other.center.x - center.x;
  float dy = other.center.y - center.y;

  // calculamos la distacnia al cuadrado entre los centros
  float distance = dx * dx + dy * dy;
  // float distance = std::pow(dx, 2) + std::pow(dy, 2);

  // calculamos la suma de los radios
  float radiusSum = radius + other.radius;

  return distance <= (radiusSum * radiusSum);
} 

void BoundingCircle::setCenter(const sf::Vector2f& c) {
  center = c;
}

void BoundingCircle::setRadius(float r) {
  radius = r;
}

sf::Vector2f BoundingCircle::getCenter() const {
  return center;
}

float BoundingCircle::getRadius() const {
  return radius;
}

void BoundingCircle::draw(sf::RenderTarget& t, const  sf::Color& c) const {
  sf::CircleShape circle(radius);
  circle.setPosition(center - sf::Vector2f(radius, radius)); 
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineColor(c);
  circle.setOutlineThickness(2.f);
  t.draw(circle);
}