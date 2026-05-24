// -*- coding: utf-8 -*-
#pragma once
#include <vector>
#include <string>

struct DialogEntry {
  std::string imagePath;
  std::string text;
  bool        alignRight;
};

enum class Stage { PreBoss, PostBoss };

namespace DialogData {
  // Devuelven la lista de entradas para un nivel (1‐based) y etapa
  const std::vector<DialogEntry>& get(int level, Stage stage);
}
