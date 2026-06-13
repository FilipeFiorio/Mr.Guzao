#pragma once

#include "Tipos.h"

MapaMundo *criarMapaMundo(int quantidadeFases);
void desenharMapaMundo(MapaMundo *mapaMundo);
void destruirMapaMundo(MapaMundo *mapaMundo);
void atualizarMapaMundo(GameWorld *gw, float delta);