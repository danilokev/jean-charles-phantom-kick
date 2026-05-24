#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum SpriteTemplate
{
	ROUND_BULLET,
	PLAYER,
	TENEBRIS,
	GODOLFO,
	SOUSA,
	THRENODY,
	VESPERA,
	LYSANDER,
	JULIAN,
	HORTENSIA,
	CLOROTH,
	P_GODOLFO,
	P_JULIAN,
	P_LYSANDER,
	P_TENEBRIS,
	ENEMY_1,
	ENEMY_2,
	ENEMY_3,
	ENEMY_4,
	LASER,
	ESFERA,
	OVALO,
	KUNAI,
	DISPARO,
	BALA,
	CARAMBANO,
	ESTRELLA,
	GOTA,
	MONEDA,
	CUCHILLO,
	BALA_DIMINUTA,
	LASER_ABAJO,
	MINI_LASER,
	ESFERA_GRANDE,
	FANTASMA_ANIMADO,
	ORBE_ANIMADO,
	NOTA_ANIMADO,
	ESFERA_AURA,
	NOTA_SILENCIO,
	POWER_UP,
	LIFE_UP,
	POINTS
};

enum ColorType {
	WHITE 	= 0,
	GREY		= 1,
	BLACK 	= 2,
	RED 		= 3,
	ORANGE	= 4,
	YELLOW 	= 5,
	GREEN 	= 6,
	BLUE  = 11,
	PURPLE = 12
};

class Sprite {
public:
	Sprite();
	explicit Sprite(const SpriteTemplate spriteType);
	explicit Sprite(const SpriteTemplate spriteType, ColorType color);

	// Agrega un frame (sub-rectángulo) para animación
	void addFrame(const sf::IntRect &frameRect);

	// Cambia el frame actual a un índice específico
	void setFrame(unsigned int frameIndex);

	// Pasa al siguiente frame, útil para animaciones
	void nextFrame();

	// Actualiza la animación bada en el tiempo transcurrido
	void updateAnimation(float deltaTime);

	// Dibuja el sprite en la ventana
	void draw(sf::RenderWindow &window);

	// Métodos de conveniencia para manipular la posición, escala, etc.
	void setPosition(float x, float y);
	void setRotation(float a);
	void setDirection(float angleDegrees);               // Rotación directa con un ángulo
    void setDirection(const sf::Vector2f& direction);    // Rotación calculada desde un vector
	void setScale(float factorX, float factorY);
	const sf::Sprite &getSprite() const;

private:
	sf::Sprite m_sprite;							 // La clase de SFML que realmente dibujamos
	sf::Texture m_texture;						 // Textura cargada desde archivo
	std::vector<sf::IntRect> m_frames; // Sub-rectángulos para animación
	unsigned int m_currentFrame;			 // Índice del frame actual
	SpriteTemplate m_spriteType;			 // Almacena el tipo de sprite
	ColorType m_color;								 // Almacena el color seleccionado

	float m_animationTime; 	// Tiempo acumulado para el frame actual
	float m_timeFrame;			// Tiempo que dura cada frame de la animación

	// Método interno para configurar el sprite según el tipo recibido
	void configureSprite(const SpriteTemplate spriteType);
	void configureEnemySprite(const SpriteTemplate spriteType);
	void configureItemSprite(const SpriteTemplate spriteType);
	void applyColor(); // Aplica el desplazamiento según el color
};

#endif // SPRITE_H
