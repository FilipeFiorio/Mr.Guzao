#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ItemMoeda.h"


ItemMoeda *criarItemMoeda(float x, float y, float largura, float altura, Color cor) {

    ItemMoeda *novoItem = (ItemMoeda*) malloc(sizeof(ItemMoeda));

    novoItem->ativo = true;
    novoItem->cor = cor;
    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;
    novoItem->valor = 1;

    return novoItem;
}

void atualizarItemMoeda(ItemMoeda *item, GameWorld *gw, float delta) {

    if(item->ativo) {
        // Depois
    }

}

void destruirItemMoeda(ItemMoeda *item) {

    if(item != NULL) {
        free(item);
    }

}

void desenharItemMoeda(ItemMoeda *item) {

    if(item->ativo) {
        DrawRectangleRec(item->ret, item->cor);
    }

}