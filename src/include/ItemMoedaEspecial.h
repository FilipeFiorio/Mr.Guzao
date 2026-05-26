#pragma once

#include "Tipos.h"

ItemMoedaEspecial *criarItemMoedaEspecial(float x, float y, float largura, float altura, Color cor);
void atualizarItemMoedaEspecial(ItemMoedaEspecial *item, GameWorld *gw, float delta);
void destruirItemMoedaEspecial(ItemMoedaEspecial *item);
void desenharItemMoedaEspecial(ItemMoedaEspecial *item);