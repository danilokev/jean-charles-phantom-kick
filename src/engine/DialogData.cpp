// -*- coding: utf-8 -*-
#include "../include/DialogData.h"
#pragma execution_character_set("utf-8")

// Pre-Boss Dialogues
static const std::vector<std::vector<DialogEntry>> preBoss = {
  // Nivel 1 – Entrada: Tenebris
  {
    {"resources/enemies/Portrait_Tenebris.png", u8"Has entrado sin permiso.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Solo quiero mi balon.", true},
    {"resources/enemies/Portrait_Tenebris.png", u8"Entonces enfrentate a mi vigilancia.", false}
  },
  // Nivel 2 – Jardin: Cloroth
  {
    {"resources/enemies/Portrait_Cloroth.png", u8"Nadie toca mi piscina.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Que exagerado, solo quiero pasar.", true},
    {"resources/enemies/Portrait_Cloroth.png", u8"Entonces preparate para mojarte.", false}
  },
  // Nivel 3 – Pasillo: Vespera
  {
    {"resources/enemies/Portrait_Vespera.png", u8"No deberias estar aqui, pequeno.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Voy a seguir aunque no quieras.", true},
    {"resources/enemies/Portrait_Vespera.png", u8"Veamos si tus hilos pueden con los mios.", false}
  },
  // Nivel 4 – Cocina: Godolfo
  {
    {"resources/enemies/Portrait_Godolfo.png", u8"No acepto criticas a mi cocina.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"No vine a comer, vine por mi balon.", true},
    {"resources/enemies/Portrait_Godolfo.png", u8"Pues saldras cocido.", false}
  },
  // Nivel 5 – Estudio: Threnody
  {
    {"resources/enemies/Portrait_Theorody.png", u8"Detesto los ruidos fuera de ritmo.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Te falta calle y futbol.", true},
    {"resources/enemies/Portrait_Theorody.png", u8"Prepárate para una sinfonia de dolor.", false}
  },
  // Nivel 6 – Final: Lysander
  {
    {"resources/enemies/Portrait_Lysander.png", u8"Llegaste muy lejos para un mortal.", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Devuelveme el balon y se acaba.", true},
    {"resources/enemies/Portrait_Lysander.png", u8"Te enseñare el verdadero poder.", false}
  }
};

// Post-Boss Dialogues
static const std::vector<std::vector<DialogEntry>> postBoss = {
  // Nivel 1 – Tenebris
  {
    {"resources/enemies/Portrait_Tenebris.png", u8"Mi deber... ha fallado...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"No queria pelear contigo.", true}
  },
  // Nivel 2 – Cloroth
  {
    {"resources/enemies/Portrait_Cloroth.png", u8"Me hundire... en mi propia piscina...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"No deberias haber sido tan toxico.", true}
  },
  // Nivel 3 – Vespera
  {
    {"resources/enemies/Portrait_Vespera.png", u8"Los hilos... se han roto...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Ya basta de marionetas raras.", true}
  },
  // Nivel 4 – Godolfo
  {
    {"resources/enemies/Portrait_Godolfo.png", u8"Mi receta final... fallida...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Ya cocine tu derrota.", true}
  },
  // Nivel 5 – Threnody
  {
    {"resources/enemies/Portrait_Theorody.png", u8"Mi sinfonia... se apago...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"No estuvo tan mal tu concierto.", true}
  },
  // Nivel 6 – Lysander
  {
    {"resources/enemies/Portrait_Lysander.png", u8"No esperaba perder... y aun asi...", false},
    {"resources/enemies/Portrait_JeanCharle.png", u8"Tu hijo solo queria jugar.", true}
  }
};

const std::vector<DialogEntry>& DialogData::get(int level, Stage stage) {
  auto idx = static_cast<size_t>(level);
  return (stage == Stage::PreBoss ? preBoss[idx] : postBoss[idx]);
}
