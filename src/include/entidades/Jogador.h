#pragma once

#include "raylib/raylib.h"
#include "core/Tipos.h"

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor);
void destruirJogador(Jogador *j);
void destruirAtualizar(Jogador *j, GameWorld *gw, float delta);
void destruirDesenhar(Jogador *j);