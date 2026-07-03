#pragma once

#include "raylib/raylib.h"

#define CENTRO 0
#define DIREITA 1
#define ESQUERDA 2

void drawTextAlinhado(const char *texto, int y, int tamanhoFonte, Color cor, int posicao);
void desenharTextoContornado(const char *texto, int x, int y, int tamanhoFonte, Color cor);
void desenharIconeMoeda(Vector2 pos, float raio);
void desenharCoracao(Vector2 pos, float tamanho, Color cor);