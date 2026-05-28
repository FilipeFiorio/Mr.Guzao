#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ItemVida.h"


ItemVida *criarItemVida(float x, float y, float largura, float altura, Color cor) {

    ItemVida *novoItem = (ItemVida*) malloc(sizeof(ItemVida));

    novoItem->ativo = true;
    novoItem->cor = cor;
    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;

    return novoItem;
}

void atualizarItemVida(ItemVida *item, GameWorld *gw, float delta) {

    if(item->ativo) {
        // Depois
    }

}

void destruirItemVida(ItemVida *item) {

    if(item != NULL) {
        free(item);
    }

}

void desenharItemVida(ItemVida *item) {

    if(item->ativo) {
        DrawRectangleRec(item->ret, item->cor);
    }

}