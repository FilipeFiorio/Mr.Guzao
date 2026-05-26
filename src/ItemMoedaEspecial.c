#include <stdlib.h>

#include "raylib/raygui.h"

#include "Tipos.h"
#include "ItemMoedaEspecial.h"

ItemMoedaEspecial *criarItemMoedaEspecial(float x, float y, float largura, float altura, Color cor) {

    ItemMoedaEspecial *novoItem = (ItemMoedaEspecial*) malloc(sizeof(ItemMoedaEspecial));

    novoItem->ativo = true;
    novoItem->cor = cor;
    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;
    novoItem->valor = 10;

    return novoItem;

}

void atualizarItemMoedaEspecial(ItemMoedaEspecial *item, GameWorld *gw, float delta) {

    if(item->ativo) {
        // Depois
    }

}

void destruirItemMoedaEspecial(ItemMoedaEspecial *item) {

}

void desenharItemMoedaEspecial(ItemMoedaEspecial *item) {

    if(item->ativo) {
        DrawRectangleRec(item->ret, item->cor);
    }

}