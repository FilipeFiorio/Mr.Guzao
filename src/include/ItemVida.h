#pragma once

#include "Tipos.h"

ItemVida *criarItemVida(float x, float y, float largura, float altura, Color cor);
void atualizarItemVida(ItemVida *item, GameWorld *gw, float delta);
void destruirItemVida(ItemVida *item);
void desenharItemVida(ItemVida *item);