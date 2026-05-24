#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

enum class SoundType {
  Bonus, Border, Cancel, Card, Damage, EnemyAttack, Extend, Graze, Gunshot,
  Invalid, Item, MagicEffect, Laser, Confirmation, Hide, Show, Option, Pause,
  PlayerDeath, PlayerShoot, EnemyDeath, PowerUp, Select, Slash, Explosion, Timeout
};

class SoundPlayer {
public:
  static SoundPlayer& getInstance();  // Singleton access

  // Métodos de uso
  bool loadSound(const std::string &id, const std::string &filePath, SoundType type);
  void playSound(const std::string &id, bool loop = false);
  void stopSound(const std::string &id);
  void stopAllSounds();

  void setVolume(const std::string &id, float volume);
  float getVolume(const std::string &id) const;
  void setCategoryVolume(SoundType type, float volume);
  float getCategoryVolume(SoundType type) const;
  void setMasterVolume(float volume);
  float getMasterVolume() const;
  static void loadAllSounds();
private:
  SoundPlayer();  // Constructor privado
  ~SoundPlayer() = default;

  // No copiables ni movibles
  SoundPlayer(const SoundPlayer&) = delete;
  SoundPlayer& operator=(const SoundPlayer&) = delete;

  struct SoundPlayerData {
    sf::SoundBuffer buffer;
    std::vector<sf::Sound> sounds;
    SoundType type;
  };

  std::map<std::string, SoundPlayerData> soundEffects;
  std::map<SoundType, float> categoryVolumes;
  float masterVolume;

  void updateVolumes();
};

#endif
