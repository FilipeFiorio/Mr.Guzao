#pragma once

#include "Tipos.h"

void resolverColisaoJogadorMapaX(GameWorld *gw);
void resolverColisaoJogadorMapaY(GameWorld *gw, float delta);
void verificarColisaoJogadorItem(GameWorld *gw);
void verificarColisaoJogadorInimigo(GameWorld *gw);
void resolverColisaoInimigoMapaX(Rectangle *ret, Vector2 *vel, Mapa *m);
void resolverColisaoInimigoMapaY(Rectangle *ret, Vector2 *vel, bool *noChao, Mapa *m);
bool verificarSeTemChao(Rectangle *retInimigo, Vector2 *velInimigo, Mapa *m);