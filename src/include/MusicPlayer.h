#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory> 

class MusicPlayer {
public:
  static MusicPlayer &getInstance();
  void loadPlaylist(const std::vector<std::pair<std::string, std::string>> &trackFiles);
  void playPlaylist(); // Empieza a reproducir el álbum desde la primera pista o la actual
  void nextTrack();    // Pasa a la siguiente pista del álbum
  void update();

  bool loadTrack(const std::string &id, const std::string &filePath);
  void playTrack(const std::string &id); // Reproduce una pista específica (la detiene si está en modo playlist)

  void pause();
  void resume(); // Reanuda la pista actual si estaba pausada
  void stop();
  bool isPlaying() const;
  bool isPaused() const;

  void setVolume(float volume);
  float getVolume() const;

  MusicPlayer(const MusicPlayer &) = delete;
  MusicPlayer &operator=(const MusicPlayer &) = delete;

private:
  MusicPlayer();
  ~MusicPlayer();

  sf::Music m_currentMusic;
  std::map<std::string, std::string> m_trackFilePaths; // Almacena id -> ruta_archivo
  std::vector<std::string> m_playlistOrder;            // IDs de las pistas en orden de reproducción

  int m_currentTrackIndex;
  float m_volume;
  bool m_isPlaylistPlaying;
  bool m_paused; // Para manejar el estado de pausa de forma explícita

  void playCurrentPlaylistTrack();
};

#endif