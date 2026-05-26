#pragma once

#include "Tipos.h"

ItemMoeda *criarItemMoeda(float x, float y, float largura, float altura, Color cor);
void atualizarItemMoeda(ItemMoeda *item, GameWorld *gw, float delta);
void destruirItemMoeda(ItemMoeda *item);
void desenharItemMoeda(ItemMoeda *item);