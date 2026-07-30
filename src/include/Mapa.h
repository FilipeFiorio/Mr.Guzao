#pragma once

#include "Tipos.h"

Mapa *carregarMapa(const char *caminhoArquivo, GameWorld *gw);
void destruirMapa(Mapa *m);
void desenharMapa(Mapa *m);
void atualizarMapa(Mapa *m,GameWorld *gw, float delta );
int calcularLarguraMapa(Mapa *m);
int calcularAlturaMapa(Mapa *m);