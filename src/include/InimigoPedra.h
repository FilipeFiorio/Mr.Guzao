#pragma once

#include "Tipos.h"

InimigoPedra *criarInimigoPedra(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoPedra(InimigoPedra *inimigo, GameWorld *gw, float delta);
void destruirInimigoPedra(InimigoPedra *inimigo);
void desenharInimigoPedra(InimigoPedra *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPedra(InimigoPedra *inimigo);