#pragma once

#include "Tipos.h"

ObstaculoGelo *criarObstaculoGelo(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura);
void destruirObstaculoGelo(ObstaculoGelo *obstaculoGelo);
void desenharObstaculoGelo(ObstaculoGelo *obstaculoGelo);