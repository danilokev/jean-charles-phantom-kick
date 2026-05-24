#include "../include/MusicPlayer.h"
#include <iostream>

MusicPlayer &MusicPlayer::getInstance()
{
  static MusicPlayer instance;
  return instance;
}

MusicPlayer::MusicPlayer()
    : m_currentTrackIndex(-1),
      m_volume(50.f), // Volumen por defecto (0-100)
      m_isPlaylistPlaying(false),
      m_paused(false)
{
}

MusicPlayer::~MusicPlayer()
{
  m_currentMusic.stop();
}

void MusicPlayer::loadPlaylist(const std::vector<std::pair<std::string, std::string>> &trackFiles)
{
  m_trackFilePaths.clear();
  m_playlistOrder.clear();
  m_currentTrackIndex = -1;
  m_isPlaylistPlaying = false;
  m_paused = false;
  m_currentMusic.stop();

  for (const auto &pair : trackFiles)
  {
    m_trackFilePaths[pair.first] = pair.second;
    m_playlistOrder.push_back(pair.first);
  }

  if (!m_playlistOrder.empty())
  {
    m_currentTrackIndex = 0; // Preparamos para reproducir la primera pista
  }
  std::cout << "Playlist cargada con " << m_playlistOrder.size() << " pistas." << std::endl;
}

void MusicPlayer::playPlaylist()
{
  if (m_playlistOrder.empty() || m_currentTrackIndex < 0)
  {
    std::cerr << "MusicPlayer Error: No hay playlist cargada o índice inválido para reproducir." << std::endl;
    return;
  }
  m_isPlaylistPlaying = true;
  m_paused = false;
  playCurrentPlaylistTrack();
}

void MusicPlayer::playCurrentPlaylistTrack() {
  if (m_currentTrackIndex < 0 || m_currentTrackIndex >= m_playlistOrder.size())
  {
    std::cerr << "MusicPlayer Error: Índice de pista fuera de rango." << std::endl;
    m_isPlaylistPlaying = false;
    return;
  }

  const std::string &trackId = m_playlistOrder[m_currentTrackIndex];
  const auto &it = m_trackFilePaths.find(trackId);

  if (it == m_trackFilePaths.end())
  {
    std::cerr << "MusicPlayer Error: No se encontró la ruta del archivo para la pista ID: " << trackId << std::endl;
    nextTrack(); // Intentar la siguiente pista
    return;
  }

  if (!m_currentMusic.openFromFile(it->second))
  {
    std::cerr << "MusicPlayer Error: No se pudo cargar la música desde: " << it->second << std::endl;
    nextTrack(); // Intentar la siguiente pista
    return;
  }

  m_currentMusic.setVolume(m_volume);
  m_currentMusic.play();
  std::cout << "Reproduciendo pista: " << trackId << " (" << it->second << ")" << std::endl;
}

void MusicPlayer::nextTrack()
{
  if (!m_isPlaylistPlaying || m_playlistOrder.empty())
  {
    return; // No estamos en modo playlist o no hay pistas
  }

  m_currentMusic.stop();

  m_currentTrackIndex++;
  if (m_currentTrackIndex >= m_playlistOrder.size())
  {
    m_currentTrackIndex = 0; // Volver al inicio del álbum
  }

  if (!m_paused)
  { // Si no estábamos pausados globalmente, reproducir la siguiente
    playCurrentPlaylistTrack();
  }
}

void MusicPlayer::update() {
  if (m_isPlaylistPlaying && !m_paused && m_currentMusic.getStatus() == sf::Music::Stopped) {
    std::cout << "Pista terminada, pasando a la siguiente." << std::endl;
    nextTrack();
  }
}

bool MusicPlayer::loadTrack(const std::string &id, const std::string &filePath)
{
  m_trackFilePaths[id] = filePath; // Añade o actualiza la pista
  sf::Music tempMusic;             // Para verificar si el archivo es válido
  if (!tempMusic.openFromFile(filePath))
  {
    std::cerr << "MusicPlayer Error: No se pudo pre-cargar/verificar la música desde: " << filePath << std::endl;
    return false;
  }
  std::cout << "Pista '" << id << "' mapeada a '" << filePath << "'." << std::endl;
  return true;
}

void MusicPlayer::playTrack(const std::string &id)
{
  m_currentMusic.stop();
  m_isPlaylistPlaying = false; // Salimos del modo playlist
  m_currentMusic.setLoop(true);
  m_paused = false;

  const auto &it = m_trackFilePaths.find(id);
  if (it == m_trackFilePaths.end())
  {
    std::cerr << "MusicPlayer Error: No se encontró la pista con ID: " << id << std::endl;
    return;
  }

  if (!m_currentMusic.openFromFile(it->second))
  {
    std::cerr << "MusicPlayer Error: No se pudo cargar la música para la pista ID: " << id << " desde: " << it->second << std::endl;
    return;
  }

  m_currentMusic.setVolume(m_volume);
  m_currentMusic.play();
  m_currentTrackIndex = -1; // Indicamos que no estamos siguiendo el playlistOrder

  // std::cout << "Reproduciendo pista individual: " << id << std::endl;
}

void MusicPlayer::pause()
{
  if (isPlaying())
  {
    m_currentMusic.pause();
    m_paused = true;
    // std::cout << "Música pausada." << std::endl;
  }
}

void MusicPlayer::resume()
{
  if (m_paused && (m_isPlaylistPlaying || m_currentMusic.getStatus() == sf::Music::Paused))
  {
    if (!m_playlistOrder.empty() && m_currentTrackIndex != -1 && m_currentMusic.getStatus() != sf::Music::Playing)
    {
      if (m_currentMusic.getStatus() == sf::Music::Stopped && m_isPlaylistPlaying)
      {
        std::cout << "Música estaba detenida, intentando reanudar playlist." << std::endl;
        playCurrentPlaylistTrack(); // Esto la reproducirá
      }
      else
      {
        m_currentMusic.play(); // Esto reanuda desde la pausa
      }
    }
    else if (m_currentMusic.getStatus() == sf::Music::Paused) {
      m_currentMusic.play();
    }
    m_paused = false;
    // std::cout << "Música reanudada." << std::endl;
  }
  else if (!isPlaying() && m_isPlaylistPlaying && !m_playlistOrder.empty()) {
    // Caso: el juego se pausó, luego se paró la música (no solo pausó), y se quiere reanudar el playlist
    std::cout << "Playlist estaba detenido, reanudando..." << std::endl;
    playCurrentPlaylistTrack();
    m_paused = false;
  }
}

void MusicPlayer::stop() {
  m_currentMusic.stop();
  m_isPlaylistPlaying = false;
  m_paused = false;
  // m_currentTrackIndex = -1; // Para resetearlo o mantenerlo para reanudar el playlist desde ahí
  // std::cout << "Música detenida." << std::endl;
}

bool MusicPlayer::isPlaying() const {
  return m_currentMusic.getStatus() == sf::Music::Playing && !m_paused;
}

bool MusicPlayer::isPaused() const {
  return m_paused;
}

void MusicPlayer::setVolume(float volume) {
  m_volume = std::max(0.f, std::min(100.f, volume)); // volumen entre 0 y 100
  m_currentMusic.setVolume(m_volume);
  // std::cout << "Volumen establecido a: " << m_volume << std::endl;
}

float MusicPlayer::getVolume() const { return m_volume; }