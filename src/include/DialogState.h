#ifndef DIALOGSTATE_H
#define DIALOGSTATE_H

#include "GameState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "DialogSystem.h" // Asegúrate de incluir el header de tu DialogSystem

class DialogState : public GameState {
private:
    static DialogState* instance;
    DialogSystem dialog; // Nuestro sistema de diálogos
    // Puedes guardar aquí una copia del estado anterior si lo necesitas (por ejemplo, para volver a él)
    
    // Constructor privado
    DialogState(const std::vector<std::string>& dialogLines);

public:
    // Retorna la instancia (Si queremos pasarle un set de diálogos fijo, lo hacemos aquí)
    static DialogState* getInstance();

    // Métodos obligatorios del estado
    virtual void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    virtual void update(sf::RenderWindow &window, StateManager &manager) override;
    virtual void render(sf::RenderWindow &window) override;
};

#endif // DIALOGSTATE_H