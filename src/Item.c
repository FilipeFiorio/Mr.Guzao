#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Item.h"
#include "ItemMoeda.h"
#include "ItemMoedaEspecial.h"
#include "ItemVida.h"

Item *criarItem(TipoItem tipo) {

    Item *novoItem = (Item*) malloc(sizeof(Item));

    novoItem->objeto = NULL;
    novoItem->tipo = tipo;

    return novoItem;

}

void atualizarItem(Item *item, GameWorld *gw, float delta) {

    if(item->tipo == ITEM_MOEDA) {
        atualizarItemMoeda((ItemMoeda*) item->objeto, gw, delta);
    } else if(item->tipo == ITEM_MOEDA_ESPECIAL) {
        atualizarItemMoedaEspecial((ItemMoedaEspecial*) item->objeto, gw, delta);
    } else if(item->tipo == ITEM_VIDA) {
        atualizarItemVida((ItemVida*) item->objeto, gw, delta);
    }

}

void destruirItem(Item *item) {
    
    if(item == NULL) {
        return;
    }

    if(item->tipo == ITEM_MOEDA) {
        destruirItemMoeda((ItemMoeda*) item->objeto);
    } else if(item->tipo == ITEM_MOEDA_ESPECIAL) {
        destruirItemMoedaEspecial((ItemMoedaEspecial*) item->objeto);
    } else if(item->tipo == ITEM_VIDA) {
        destruirItemVida((ItemVida*) item->objeto);
    }

}

void desenharItem(Item *item) {

    if(item == NULL) {
        return;
    }

    if(item->tipo == ITEM_MOEDA) {
        desenharItemMoeda((ItemMoeda*) item->objeto);
    } else if(item->tipo == ITEM_MOEDA_ESPECIAL) {
        desenharItemMoedaEspecial((ItemMoedaEspecial*) item->objeto);
    } else if(item->tipo == ITEM_VIDA) {
        desenharItemVida((ItemVida*) item->objeto);
    }

}