#include "../include/SoundPlayer.h"
#include <iostream>

// Singleton
SoundPlayer& SoundPlayer::getInstance() {
    static SoundPlayer instance;
    return instance;
}

// Constructor privado
SoundPlayer::SoundPlayer()
    : masterVolume(100.0f)
{
    for (int i = 0; i <= static_cast<int>(SoundType::Timeout); ++i) {
        categoryVolumes[static_cast<SoundType>(i)] = 100.0f;
    }
}

// Cargar un sonido y su categoría
bool SoundPlayer::loadSound(const std::string &id, const std::string &filePath, SoundType type) {
    if (soundEffects.find(id) != soundEffects.end())
        return true;

    SoundPlayerData data;
    data.type = type;

    if (!data.buffer.loadFromFile(filePath)) {
        std::cerr << "Error: No se pudo cargar el archivo de sonido: " << filePath << std::endl;
        return false;
    }

    soundEffects[id] = std::move(data);
    return true;
}

// Reproducir sonido
void SoundPlayer::playSound(const std::string &id, bool loop) {
    auto it = soundEffects.find(id);
    if (it == soundEffects.end()) {
        std::cerr << "Error: Intentando reproducir un sonido no cargado: " << id << std::endl;
        return;
    }

    sf::Sound* soundToPlay = nullptr;

    for (auto& sound : it->second.sounds) {
        if (sound.getStatus() != sf::Sound::Playing) {
            soundToPlay = &sound;
            break;
        }
    }

    if (!soundToPlay) {
        it->second.sounds.emplace_back(sf::Sound());
        soundToPlay = &it->second.sounds.back();
        soundToPlay->setBuffer(it->second.buffer);
    }

    soundToPlay->setLoop(loop);
    float categoryVol = categoryVolumes[it->second.type];
    soundToPlay->setVolume(categoryVol * masterVolume / 100.0f);
    soundToPlay->play();
}

// Detener un sonido específico
void SoundPlayer::stopSound(const std::string &id) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        for (auto& sound : it->second.sounds) {
            sound.stop();
        }
    }
}

// Detener todos los sonidos
void SoundPlayer::stopAllSounds() {
    for (auto& [id, data] : soundEffects) {
        for (auto& sound : data.sounds) {
            sound.stop();
        }
    }
}

// Establecer volumen individual de un sonido
void SoundPlayer::setVolume(const std::string &id, float volume) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        for (auto& sound : it->second.sounds) {
            float adjustedVolume = volume * categoryVolumes[it->second.type] * masterVolume / 10000.0f;
            sound.setVolume(adjustedVolume);
        }
    }
}

// Obtener volumen del primer canal del sonido
float SoundPlayer::getVolume(const std::string &id) const {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end() && !it->second.sounds.empty()) {
        return it->second.sounds[0].getVolume();
    }
    return 0.0f;
}

// Volumen por tipo de sonido
void SoundPlayer::setCategoryVolume(SoundType type, float volume) {
    categoryVolumes[type] = volume;
    updateVolumes();
}

float SoundPlayer::getCategoryVolume(SoundType type) const {
    auto it = categoryVolumes.find(type);
    if (it != categoryVolumes.end()) {
        return it->second;
    }
    return 0.0f;
}

// Volumen maestro
void SoundPlayer::setMasterVolume(float volume) {
    masterVolume = volume;
    updateVolumes();
}

float SoundPlayer::getMasterVolume() const {
    return masterVolume;
}

// Actualizar todos los volúmenes tras un cambio
void SoundPlayer::updateVolumes() {
    for (auto& [id, data] : soundEffects) {
        float categoryVol = categoryVolumes[data.type];

        for (auto& sound : data.sounds) {
            float individualVolume = sound.getVolume() * 100.0f / (categoryVol * masterVolume);
            float adjustedVolume = individualVolume * categoryVol * masterVolume / 10000.0f;
            sound.setVolume(adjustedVolume);
        }
    }
}

void SoundPlayer::loadAllSounds() {
    SoundPlayer::getInstance().loadSound("bullet_spawn", "resources/sounds/se_plst00.wav", SoundType::PlayerShoot);
    SoundPlayer::getInstance().loadSound("enemy_death", "resources/sounds/se_enep00.wav", SoundType::EnemyDeath);
    SoundPlayer::getInstance().loadSound("player_death", "resources/sounds/se_pldead00.wav", SoundType::PlayerDeath);
    SoundPlayer::getInstance().loadSound("pause_sound", "resources/sounds/se_pause.wav", SoundType::Pause);
    SoundPlayer::getInstance().loadSound("player_bomb", "resources/sounds/se_extend.wav", SoundType::Extend);
    SoundPlayer::getInstance().loadSound("item_powerup", "resources/sounds/se_item00.wav", SoundType::PowerUp);
    SoundPlayer::getInstance().loadSound("boss_phase", "resources/sounds/se_power0.wav", SoundType::MagicEffect);
    SoundPlayer::getInstance().loadSound("boss_defeat", "resources/sounds/se_enep01.wav", SoundType::Explosion);
    SoundPlayer::getInstance().loadSound("menu_move", "resources/sounds/se_select00.wav", SoundType::Select);
    SoundPlayer::getInstance().loadSound("boss_damage", "resources/sounds/se_damage00.wav", SoundType::Damage);

    SoundPlayer::getInstance().setCategoryVolume(SoundType::PlayerDeath, 10.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::PlayerShoot, 20.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::EnemyDeath, 10.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::MagicEffect, 35.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::Extend, 20.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::PowerUp, 40.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::Pause, 20.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::Explosion, 15.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::Select, 15.f);
    SoundPlayer::getInstance().setCategoryVolume(SoundType::Damage, 9.f);
}