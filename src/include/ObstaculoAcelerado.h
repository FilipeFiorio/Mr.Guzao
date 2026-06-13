#pragma once

#include "Tipos.h"

ObstaculoAcelerado *criarObstaculoAcelerado(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura);
void destruirObstaculoAcelerado(ObstaculoAcelerado *obstaculoAcelerado);
void desenharObstaculoAcelerado(ObstaculoAcelerado *obstaculoAcelerado);