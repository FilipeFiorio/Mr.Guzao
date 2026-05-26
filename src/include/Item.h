#pragma once

#include "Tipos.h"

Item *criarItem(TipoItem tipo);
void atualizarItem(Item *item, GameWorld *gw, float delta);
void destruirItem(Item *item);
void desenharItem(Item *item);