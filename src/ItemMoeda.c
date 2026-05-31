#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ItemMoeda.h"
#include "Animacao.h"
#include "ResourceManager.h"

static void desenharAnimacaoItemMoeda(ItemMoeda *item, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualItemMoeda(ItemMoeda *item);
static QuadroAnimacao *getQuadroAnimacaoAtualItemMoeda(ItemMoeda *item);


ItemMoeda *criarItemMoeda(float x, float y, float largura, float altura, Color cor) {

    ItemMoeda *novoItem = (ItemMoeda*) malloc(sizeof(ItemMoeda));

    novoItem->ativo = true;
    novoItem->cor = cor;
    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;
    novoItem->valor = 1;
    novoItem->estado = ITEM_GIRANDO;

    int quantidadeAnimacoes = 0;

    novoItem->animacaoGirando.quantidadeQuadros = 4;
    novoItem->animacaoGirando.quadroAtual = 0;
    novoItem->animacaoGirando.contadorTempoQuadro = 0;
    novoItem->animacaoGirando.pararNoUltimoQuadro = false;
    novoItem->animacaoGirando.executarUmaVez = false;
    novoItem->animacaoGirando.finalizada = false;
    criarQuadroAnimacao(&novoItem->animacaoGirando, novoItem->animacaoGirando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoItem->animacaoGirando.quadros,
        novoItem->animacaoGirando.quantidadeQuadros,
        200,
        1, 
        1,
        15,
        15,
        false, 
        1
    );

    novoItem->animacaoColetado.quantidadeQuadros = 4;
    novoItem->animacaoColetado.quadroAtual = 0;
    novoItem->animacaoColetado.contadorTempoQuadro = 0;
    novoItem->animacaoColetado.pararNoUltimoQuadro = false;
    novoItem->animacaoColetado.finalizada = false;
    novoItem->animacaoColetado.executarUmaVez = true;
    criarQuadroAnimacao(&novoItem->animacaoColetado, novoItem->animacaoColetado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoItem->animacaoColetado.quadros,
        novoItem->animacaoColetado.quantidadeQuadros,
        50,
        1,
        49,
        13,
        14,
        false,
        1
    );

    novoItem->animacoes[ITEM_GIRANDO] = &novoItem->animacaoGirando;
    quantidadeAnimacoes++;

    novoItem->animacoes[ITEM_COLETADO] = &novoItem->animacaoColetado;
    quantidadeAnimacoes++;

    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;
    

    return novoItem;
}


void atualizarItemMoeda(ItemMoeda *item, GameWorld *gw, float delta) {
    if(item->ativo) {
        Animacao *animacaoAtual = getAnimacaoAtualItemMoeda(item);
        atualizarAnimacao(animacaoAtual, delta);
        if(item->estado == ITEM_COLETADO && animacaoAtual->finalizada) {
            item->ativo = false;
        }
    }
}

void destruirItemMoeda(ItemMoeda *item) {
    
    if(item != NULL) {
        for(int i = 0; i <item->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(item->animacoes[i]);
        }
        free(item);
    }

}

void desenharItemMoeda(ItemMoeda *item) {

    if(item->ativo) {
        QuadroAnimacao *quadro = getQuadroAnimacaoAtualItemMoeda(item);
        desenharAnimacaoItemMoeda(item, quadro, WHITE);
    }

}

static void desenharAnimacaoItemMoeda(ItemMoeda *item, QuadroAnimacao *quadro, Color tonalidade) {


    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaItens,
            (Rectangle) {
                quadro->fonte.x,
                quadro->fonte.y,
                quadro->fonte.width,
                quadro->fonte.height
            },
            item->ret,
            (Vector2) {0},
            0.0f,
            tonalidade
        );

    }
}

static Animacao *getAnimacaoAtualItemMoeda(ItemMoeda *item) {

    return item->animacoes[item->estado];

}

static QuadroAnimacao *getQuadroAnimacaoAtualItemMoeda(ItemMoeda *item) {

    return getQuadroAtualAnimacao(getAnimacaoAtualItemMoeda(item));

}