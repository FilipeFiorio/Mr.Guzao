#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ItemVida.h"
#include "Animacao.h"
#include "ResourceManager.h"

static void desenharAnimacaoItemVida(ItemVida *item, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualItemVida(ItemVida *item);
static QuadroAnimacao *getQuadroAnimacaoAtualItemVida(ItemVida *item);


ItemVida *criarItemVida(float x, float y, float largura, float altura, Color cor) {

    ItemVida *novoItem = (ItemVida*) malloc(sizeof(ItemVida));

    novoItem->ativo = true;
    novoItem->cor = cor;
    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;
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
        33,
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
        81,
        15,
        15,
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

void atualizarItemVida(ItemVida *item, GameWorld *gw, float delta) {

    if(item->ativo) {
        Animacao *animacaoAtual = getAnimacaoAtualItemVida(item);
        atualizarAnimacao(animacaoAtual, delta);
        if(item->estado == ITEM_COLETADO && animacaoAtual->finalizada) {
            item->ativo = false;
        }
    }

}

void destruirItemVida(ItemVida *item) {

    if(item != NULL) {
        for(int i = 0; i < item->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(item->animacoes[i]);
        }
        free(item);
    }

}

void desenharItemVida(ItemVida *item) {

    if(item->ativo) {
        QuadroAnimacao *quadro = getQuadroAnimacaoAtualItemVida(item);
        desenharAnimacaoItemVida(item, quadro, WHITE);
    }

}

static void desenharAnimacaoItemVida(ItemVida *item, QuadroAnimacao *quadro, Color tonalidade) {

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

static Animacao *getAnimacaoAtualItemVida(ItemVida *item) {

    return item->animacoes[item->estado];

}

static QuadroAnimacao *getQuadroAnimacaoAtualItemVida(ItemVida *item) {

    return getQuadroAtualAnimacao(getAnimacaoAtualItemVida(item));

}