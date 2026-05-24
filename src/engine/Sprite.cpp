#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "../include/Sprite.h"
#include <cmath>

Sprite::Sprite() : m_currentFrame(0), m_animationTime(0.f), m_timeFrame(0.15f) {}

Sprite::Sprite(const SpriteTemplate spriteType) : m_currentFrame(0), m_animationTime(0.f), m_timeFrame(0.15f) { configureSprite(spriteType); }

Sprite::Sprite(const SpriteTemplate spriteType, ColorType color)
    : m_currentFrame(0), m_spriteType(spriteType), m_color(color), m_animationTime(0.f), m_timeFrame(0.15f)
{
  configureSprite(spriteType);
  applyColor();
}

void Sprite::configureSprite(const SpriteTemplate spriteType) {
  try {
    static bool spritesheetLoaded = false;
    static sf::Texture spritesheet;

    if (spriteType >= ENEMY_1 && spriteType <= ENEMY_4) {
      configureEnemySprite(spriteType);
      return;
    }

    if (spriteType == POINTS || spriteType == POWER_UP || spriteType == LIFE_UP) {
      configureItemSprite(spriteType);
      return;
    }

    if (!spritesheetLoaded) {
      if (!spritesheet.loadFromFile("resources/bullets_spritesheet.png")) {
        throw std::runtime_error("Error al cargar el spritesheet");
      }

      spritesheetLoaded = true;
    }

    m_spriteType = spriteType;
    m_frames.clear();
    
    m_sprite.setTexture(spritesheet);
    
    switch (spriteType) {
    case ROUND_BULLET:
    {
      // Frame: (left, top, width, height)
      m_frames.push_back(sf::IntRect(0, 273, 16, 30));
      m_sprite.setOrigin(8, 15);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case LASER:
    {
      m_frames.push_back(sf::IntRect(0, 0, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case ESFERA:
    {
      m_frames.push_back(sf::IntRect(0, 16, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case OVALO:
    {
      m_frames.push_back(sf::IntRect(0, 48, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case KUNAI:
    {
      m_frames.push_back(sf::IntRect(0, 80, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case DISPARO:
    {
      m_frames.push_back(sf::IntRect(0, 96, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case BALA:
    {
      m_frames.push_back(sf::IntRect(0, 112, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case CARAMBANO:
    {
      m_frames.push_back(sf::IntRect(0, 144, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case ESTRELLA:
    {
      m_frames.push_back(sf::IntRect(0, 160, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case GOTA:
    {
      m_frames.push_back(sf::IntRect(0, 176, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case MONEDA:
    {
      m_frames.push_back(sf::IntRect(0, 192, 16, 16));
      m_sprite.setOrigin(8, 8);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case CUCHILLO:
    {
      m_frames.push_back(sf::IntRect(0, 369, 16, 31));
      m_sprite.setOrigin(8, 15.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case BALA_DIMINUTA:
    {
      m_frames.push_back(sf::IntRect(0, 432, 8, 8));
      m_sprite.setOrigin(4, 4);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case LASER_ABAJO:
    {
      m_frames.push_back(sf::IntRect(0, 899, 16, 250));
      m_sprite.setOrigin(8, 125);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case MINI_LASER:
    {
      int sectionLeft = 288; // Pos. base
      int frameWidth = 16;   // Ancho por color
      m_frames.push_back(sf::IntRect(sectionLeft, 310, frameWidth, 22));
      m_sprite.setOrigin(8, 11);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case ESFERA_GRANDE:
    {
      int sectionLeft = 576;
      int frameWidth = 32;
      m_frames.push_back(sf::IntRect(sectionLeft, 1, frameWidth, 30));
      m_sprite.setOrigin(16, 15);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case FANTASMA_ANIMADO:
    {
      m_frames.push_back(sf::IntRect(580, 352, 29, 31));
      m_frames.push_back(sf::IntRect(580, 383, 29, 31));
      m_frames.push_back(sf::IntRect(580, 416, 29, 31));
      m_sprite.setOrigin(16, 15.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case NOTA_ANIMADO:
    {
      m_frames.push_back(sf::IntRect(577, 929, 32, 31));
      m_frames.push_back(sf::IntRect(577, 961, 32, 31));
      m_frames.push_back(sf::IntRect(577, 994, 32, 31));
      m_sprite.setOrigin(16, 15.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case ORBE_ANIMADO:
    {
      m_frames.push_back(sf::IntRect(576, 1056, 32, 32));
      m_frames.push_back(sf::IntRect(576, 1088, 32, 32));
      m_frames.push_back(sf::IntRect(576, 1120, 32, 32));
      m_sprite.setOrigin(16, 16);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case NOTA_SILENCIO:
    {
      int sectionLeft = 577;
      int frameWidth = 32;
      m_frames.push_back(sf::IntRect(sectionLeft, 1024, frameWidth, 32));
      m_sprite.setOrigin(16, 16);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case PLAYER:
    {
      if (!m_texture.loadFromFile("resources/JeanCharles.png")) {
        throw std::runtime_error("Error al cargar la textura: JeanCharles.png");
      }
      m_sprite.setTexture(m_texture);
      // Tras cargar la textura y definir el frame:
      m_sprite.setOrigin(15.f, 35.f);

      // Definimos 4 frames, uno por cada dirección:
      //   0 --> mirando hacia abajo  (Down)
      //   1 --> mirando hacia la derecha (Right)
      //   2 --> mirando hacia la izquierda (Left)
      //   3 --> mirando hacia arriba   (Up)

      // Cada frame: (left, top, width, height)
      m_frames.push_back(sf::IntRect(0, 0, 60, 210));   // Down
      m_frames.push_back(sf::IntRect(125, 0, 70, 210)); // Right
      m_frames.push_back(sf::IntRect(65, 0, 60, 210));  // Left
      m_frames.push_back(sf::IntRect(0, 0, 60, 210));   // Up

      // Ajustamos el frame inicial (por ejemplo, mirando hacia abajo).
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case THRENODY:
    {
      static bool textureLoaded = false;
      static sf::Texture bossTexture; // Es una textura estática

      if (!textureLoaded)
      {
        if (!bossTexture.loadFromFile("resources/Boss.png"))
        {
          throw std::runtime_error("Error al cargar la textura: Boss.png");
        }
        textureLoaded = true;
      }

      m_sprite.setTexture(bossTexture);
      sf::Vector2u textureSize = bossTexture.getSize();
      m_sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);

      m_frames.clear();
      m_frames.push_back(sf::IntRect(0, 0, textureSize.x, textureSize.y));
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case GODOLFO: 
    {
      static bool godolfoTextureLoaded = false;
      static sf::Texture godolfoTexture;

      if (!godolfoTextureLoaded) {
        if (!godolfoTexture.loadFromFile("resources/enemies/Boss_Godolfo.png")) {
          throw std::runtime_error("Error al cargar Boss Godolfo");
        } godolfoTextureLoaded = true;
      }

      m_sprite.setTexture(godolfoTexture);

      int sectionLeft = 9;
      int frameWidth = 108;
      int frameHeight = 152;
      int topOffset = 3;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case JULIAN: 
    {
      static bool julianTextureLoaded = false;
      static sf::Texture julianTexture;

      if (!julianTextureLoaded)
      {
        if (!julianTexture.loadFromFile("resources/enemies/Boss_Julian.png"))
        {
          throw std::runtime_error("Error al cargar Boss Julian");
        }
        julianTextureLoaded = true;
      }

      m_sprite.setTexture(julianTexture);

      int sectionLeft = 33;
      int frameWidth = 74;
      int frameHeight = 146;
      int topOffset = 38;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case LYSANDER:
    {
      static bool lysanderTextureLoaded = false;
      static sf::Texture lysanderTexture;

      if (!lysanderTextureLoaded)
      {
        if (!lysanderTexture.loadFromFile("resources/enemies/Boss_lysander.png"))
        {
          throw std::runtime_error("Error al cargar Boss Lysander");
        }
        lysanderTextureLoaded = true;
      }

      m_sprite.setTexture(lysanderTexture);

      int sectionLeft = 29;
      int frameWidth = 102;
      int frameHeight = 187;
      int topOffset = 4;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case TENEBRIS:
    {
      static bool tenebrisTextureLoaded = false;
      static sf::Texture tenebrisTexture;

      if (!tenebrisTextureLoaded)
      {
        if (!tenebrisTexture.loadFromFile("resources/enemies/Boss_Tenebris.png"))
        {
          throw std::runtime_error("Error al cargar Boss Tenebris");
        }
        tenebrisTextureLoaded = true;
      }

      m_sprite.setTexture(tenebrisTexture);

      int sectionLeft = 4;
      int frameWidth = 156;
      int frameHeight = 185;
      int topOffset = 4;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case HORTENSIA:
    {
      static bool hortensiaTextureLoaded = false;
      static sf::Texture hortensiaTexture;

      if (!hortensiaTextureLoaded)
      {
        if (!hortensiaTexture.loadFromFile("resources/enemies/Boss_Hortensia.png"))
        {
          throw std::runtime_error("Error al cargar Boss HORTENSIA");
        }
        hortensiaTextureLoaded = true;
      }

      m_sprite.setTexture(hortensiaTexture);

      int sectionLeft = 12;
      int frameWidth = 95;
      int frameHeight = 200;
      int topOffset = 4;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case CLOROTH:
    {
      static bool clorothTextureLoaded = false;
      static sf::Texture clorothTexture;

      if (!clorothTextureLoaded)
      {
        if (!clorothTexture.loadFromFile("resources/enemies/Boss_Cloroth.png"))
        {
          throw std::runtime_error("Error al cargar Boss CLOROTH");
        }
        clorothTextureLoaded = true;
      }

      m_sprite.setTexture(clorothTexture);

      int sectionLeft = 31;
      int frameWidth = 80;
      int frameHeight = 133;
      int topOffset = 2;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.6f, 0.6f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case VESPERA:
    {
      static bool vesperaTextureLoaded = false;
      static sf::Texture vesperaTexture;

      if (!vesperaTextureLoaded)
      {
        if (!vesperaTexture.loadFromFile("resources/enemies/Boss_Vespera.png"))
        {
          throw std::runtime_error("Error al cargar Boss VESPERA");
        }
        vesperaTextureLoaded = true;
      }

      m_sprite.setTexture(vesperaTexture);

      int sectionLeft = 9;
      int frameWidth = 93;
      int frameHeight = 154;
      int topOffset = 8;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
     case SOUSA:
    {
      static bool sousaTextureLoaded = false;
      static sf::Texture sousaTexture;

      if (!sousaTextureLoaded)
      {
        if (!sousaTexture.loadFromFile("resources/enemies/Boss_Sousa.png"))
        {
          throw std::runtime_error("Error al cargar Boss SOUSA");
        }
        sousaTextureLoaded = true;
      }

      m_sprite.setTexture(sousaTexture);

      int sectionLeft = 46;
      int frameWidth = 73;
      int frameHeight = 176;
      int topOffset = 6;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.5f, 0.5f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case P_GODOLFO:
    {
      static bool tenebrisTextureLoaded = false;
      static sf::Texture tenebrisTexture;

      if (!tenebrisTextureLoaded)
      {
        if (!tenebrisTexture.loadFromFile("resources/enemies/Portrait_Godolfo.png"))
        {
          throw std::runtime_error("Error al cargar Boss Tenebris");
        }
        tenebrisTextureLoaded = true;
      }

      m_sprite.setTexture(tenebrisTexture);

      int sectionLeft = 298;
      int frameWidth = 402;
      int frameHeight = 739;
      int topOffset = 7;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.15f, 0.15f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case P_JULIAN:
    {
      static bool tenebrisTextureLoaded = false;
      static sf::Texture tenebrisTexture;

      if (!tenebrisTextureLoaded)
      {
        if (!tenebrisTexture.loadFromFile("resources/enemies/Portrait_Julian.png"))
        {
          throw std::runtime_error("Error al cargar Boss Tenebris");
        }
        tenebrisTextureLoaded = true;
      }

      m_sprite.setTexture(tenebrisTexture);

      int sectionLeft = 597;
      int frameWidth = 574;
      int frameHeight = 739;
      int topOffset = 36;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.15f, 0.15f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case P_LYSANDER:
    {
      static bool tenebrisTextureLoaded = false;
      static sf::Texture tenebrisTexture;

      if (!tenebrisTextureLoaded)
      {
        if (!tenebrisTexture.loadFromFile("resources/enemies/Portrait_Lysander.png"))
        {
          throw std::runtime_error("Error al cargar Boss Tenebris");
        }
        tenebrisTextureLoaded = true;
      }

      m_sprite.setTexture(tenebrisTexture);

      int sectionLeft = 213;
      int frameWidth = 533;
      int frameHeight = 739;
      int topOffset = 19;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.15f, 0.15f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    case P_TENEBRIS:
    {
      static bool tenebrisTextureLoaded = false;
      static sf::Texture tenebrisTexture;

      if (!tenebrisTextureLoaded)
      {
        if (!tenebrisTexture.loadFromFile("resources/enemies/Portrait_Tenebris.png"))
        {
          throw std::runtime_error("Error al cargar Boss Tenebris");
        }
        tenebrisTextureLoaded = true;
      }

      m_sprite.setTexture(tenebrisTexture);

      int sectionLeft = 338;
      int frameWidth = 834;
      int frameHeight = 739;
      int topOffset = 45;

      m_frames.push_back(sf::IntRect(sectionLeft, topOffset, frameWidth, frameHeight));
      m_sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
      m_sprite.setScale(0.15f, 0.15f);
      m_sprite.setTextureRect(m_frames[0]);
      break;
    }
    default:
    {
      throw std::runtime_error("Tipo de sprite no reconocido: " + std::to_string(spriteType));
    }
    }
  }
  catch (const std::exception &e) {
    std::cerr << "Excepción capturada: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE); // Detiene la ejecución del programa
  }
}

void Sprite::configureEnemySprite(const SpriteTemplate spriteType) {
  try {
    static bool enemiesTextureLoaded  = false;
    static sf::Texture enemiesTexture;

    if (!enemiesTextureLoaded) {
      if (!enemiesTexture.loadFromFile("resources/enemies.png")) {
        throw std::runtime_error("Error al cargar enemies.png");
      }
      enemiesTextureLoaded = true;
    }
    
    m_frames.clear();
    m_sprite.setTexture(enemiesTexture);

    float originX = 0.f, originY = 0.f;
    float scale = 0.6f; // Ajustar tamaño de todos los sprites
    
    switch (spriteType) {
      case ENEMY_1:
        m_frames.push_back(sf::IntRect(0, 0, 94, 71));    // Frame: 1
        m_frames.push_back(sf::IntRect(0, 97, 93, 69));   // Frame: 2
        m_frames.push_back(sf::IntRect(0, 193, 90, 69));  // Frame: 3
        m_frames.push_back(sf::IntRect(6, 289, 78, 78));  // Frame: 4

        m_frames.push_back(sf::IntRect(0, 193, 90, 69));  // Frame: 3
        m_frames.push_back(sf::IntRect(0, 97, 93, 69));   // Frame: 2
        m_frames.push_back(sf::IntRect(0, 0, 94, 71));    // Frame: 1
        if (!m_frames.empty()) { 
          originX = m_frames[0].width / 2.0f;
          originY = m_frames[0].height / 2.0f;
        }
        break;
      case ENEMY_2:
        m_frames.push_back(sf::IntRect(108, 10, 90, 69));
        m_frames.push_back(sf::IntRect(108, 121, 90, 57));
        m_frames.push_back(sf::IntRect(123, 220, 60, 51));
        m_frames.push_back(sf::IntRect(123, 316, 60, 51));

        m_frames.push_back(sf::IntRect(123, 220, 60, 51));
        m_frames.push_back(sf::IntRect(108, 121, 90, 57));
        m_frames.push_back(sf::IntRect(108, 10, 90, 69));
        if (!m_frames.empty()) { 
          originX = m_frames[0].width / 2.0f;
          originY = m_frames[0].height / 2.0f;
        }
        break;
      case ENEMY_3:
        m_frames.push_back(sf::IntRect(224, 0, 84, 79));
        m_frames.push_back(sf::IntRect(227, 91, 81, 84));
        m_frames.push_back(sf::IntRect(224, 187, 84, 81));
        m_frames.push_back(sf::IntRect(224, 283, 81, 84));

        m_frames.push_back(sf::IntRect(224, 187, 84, 81));
        m_frames.push_back(sf::IntRect(227, 91, 81, 84));
        m_frames.push_back(sf::IntRect(224, 0, 84, 79));
        if (!m_frames.empty()) { 
          originX = m_frames[0].width / 2.0f;
          originY = m_frames[0].height / 2.0f;
        }
        break;
      case ENEMY_4:
        m_frames.push_back(sf::IntRect(318, 0, 84, 79));
        m_frames.push_back(sf::IntRect(321, 91, 81, 84));
        m_frames.push_back(sf::IntRect(318, 187, 84, 81));
        m_frames.push_back(sf::IntRect(318, 283, 81, 84));

        m_frames.push_back(sf::IntRect(318, 187, 84, 81));
        m_frames.push_back(sf::IntRect(321, 91, 81, 84));
        m_frames.push_back(sf::IntRect(318, 0, 84, 79));
        if (!m_frames.empty()) { 
          originX = m_frames[0].width / 2.0f;
          originY = m_frames[0].height / 2.0f;
        }
        break;
      default:
        throw std::runtime_error("Tipo de enemigo no reconocido");
    }

    if (!m_frames.empty()) {
      m_sprite.setOrigin(originX, originY);
      m_sprite.setTextureRect(m_frames[0]); 
      m_sprite.setScale(scale, scale); 
    } else {
      throw std::runtime_error("No se cargaron frames para el enemigo: " + std::to_string(spriteType));
    }
  }
  catch (const std::exception &e) {
    std::cerr << "Excepción capturada en configureEnemySprite: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void Sprite::configureItemSprite(const SpriteTemplate spriteType) {
  try {
    static bool enemiesTextureLoaded  = false;
    static sf::Texture itemsTexture;

    if (!enemiesTextureLoaded) {
      if (!itemsTexture.loadFromFile("resources/items.png")) {
        throw std::runtime_error("Error al cargar enemies.png");
      }
      enemiesTextureLoaded = true;
    }
    
    m_frames.clear();
    m_sprite.setTexture(itemsTexture);

    switch (spriteType) {
      case POINTS:
      {
        // Frame: (left, top, width, height)
        m_frames.push_back(sf::IntRect(17, 0, 16, 16));
        m_sprite.setOrigin(8, 8);
        m_sprite.setTextureRect(m_frames[0]);
        break;
      }
      case POWER_UP:
      {
        // Frame: (left, top, width, height)
        m_frames.push_back(sf::IntRect(0, 0, 16, 16));
        m_sprite.setOrigin(8, 8);
        m_sprite.setTextureRect(m_frames[0]);
        break;
      }
      case LIFE_UP:
      {
        // Frame: (left, top, width, height)
        m_frames.push_back(sf::IntRect(33, 0, 16, 16));
        m_sprite.setOrigin(8, 8);
        m_sprite.setTextureRect(m_frames[0]);
        break;
      }
    }
  }
  catch (const std::exception &e) {
    std::cerr << "Excepción capturada en configureEnemySprite: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


void Sprite::applyColor() {
  if (m_frames.empty()) return;

  // Almaceno el rectángulo actual del frame
  sf::IntRect frame = m_frames[m_currentFrame];
  int sectionLeft = frame.left;
  int frameWidth = frame.width;

  // posición base + (color * ancho del frame)
  frame.left = sectionLeft + (static_cast<int>(m_color) * frameWidth);

  // Actualizamos
  m_sprite.setTextureRect(frame);
}

void Sprite::addFrame(const sf::IntRect &frameRect) { m_frames.push_back(frameRect); }

void Sprite::setFrame(unsigned int frameIndex) {
  if (frameIndex < m_frames.size()) {
    m_currentFrame = frameIndex;
    m_sprite.setTextureRect(m_frames[m_currentFrame]);
  }
}

void Sprite::nextFrame() {
  // Avanza al siguiente frame cíclicamente
  m_currentFrame = (m_currentFrame + 1) % m_frames.size();
  m_sprite.setTextureRect(m_frames[m_currentFrame]);
}

void Sprite::updateAnimation(float deltaTime) {
  if (m_frames.size() <= 1) {
    return;
  }

  m_animationTime += deltaTime;
  if (m_animationTime >= m_timeFrame) {
    m_animationTime = 0.f;
    nextFrame();
  }
}

const sf::Sprite &Sprite::getSprite() const { return m_sprite; }

void Sprite::setPosition(float x, float y) { m_sprite.setPosition(x, y); }

void Sprite::setRotation(float angle) { m_sprite.setRotation(angle); }

void Sprite::setScale(float factorX, float factorY) { m_sprite.setScale(factorX, factorY); }

const float PI = 3.14159265358979323846f;

/**
 * @brief Asigna una rotación directa en grados al sprite.
 *        Compensa el desfase de 90° para apuntar correctamente.
 * @param angleDegrees Ángulo en grados, donde 0 apunta a la derecha.
 */
void Sprite::setDirection(float angleDegrees) {
  // Compensación de 90° inicial
  float adjustedAngle = angleDegrees - 90.f;

  // Calcula el ajuste adicional para corregir el eje Y invertido
  if (angleDegrees > 90.f) {
      float offset = (angleDegrees - 90.f) * 2;
      adjustedAngle -= offset;
  } else {
      float offset = (90.f - angleDegrees) * 2;
      adjustedAngle += offset;
  }

  m_sprite.setRotation(adjustedAngle);
}

/**
* @brief Calcula la rotación en función de un vector de movimiento.
*        Compensa el desfase de 90° para apuntar correctamente.
* @param direction Vector de dirección, debe estar normalizado.
*/
void Sprite::setDirection(const sf::Vector2f& direction) {
  // Evitar división por 0
  if (direction.x == 0.f && direction.y == 0.f) return;

  // Calcula el ángulo usando atan2 (recuerda que SFML tiene el eje Y invertido)
  float angleRad = std::atan2(-direction.y, direction.x); // ← -Y por el sistema de SFML
  float angleDeg = angleRad * 180.f / PI;

  // Compensación de 90° inicial
  float adjustedAngle = angleDeg - 90.f;

  // Calcula el ajuste adicional para corregir el eje Y invertido
  if (angleDeg > 90.f) {
      float offset = (angleDeg - 90.f) * 2;
      adjustedAngle -= offset;
  } else {
      float offset = (90.f - angleDeg) * 2;
      adjustedAngle += offset;
  }

  m_sprite.setRotation(adjustedAngle);
}
