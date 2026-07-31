#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoEspinho.h"
#include "ResourceManager.h"
#include "Animacao.h"
#include "Colisao.h"

static void desenharAnimacaoInimigoEspinho(InimigoEspinho *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoEspinho *criarInimigoEspinho(float x, float y, float largura, float altura, Color cor) {

    InimigoEspinho *novoInimigoEspinho = (InimigoEspinho*) malloc(sizeof(InimigoEspinho));

    novoInimigoEspinho->ret.x = x;
    novoInimigoEspinho->ret.y = y;
    novoInimigoEspinho->ret.width = largura;
    novoInimigoEspinho->ret.height = altura;
    novoInimigoEspinho->vel = (Vector2) {100, 0};
    novoInimigoEspinho->velMaxQueda = 600;
    novoInimigoEspinho->noChao = false;
    novoInimigoEspinho->paraDireita = false;
    novoInimigoEspinho->cor = cor;
    novoInimigoEspinho->estado = INIMIGO_ESPINHO_ANDANDO;

    int quantidadeAnimacoes = 0;

    novoInimigoEspinho->animacaoAndando.quantidadeQuadros = 5;
    novoInimigoEspinho->animacaoAndando.quadroAtual = 0;
    novoInimigoEspinho->animacaoAndando.contadorTempoQuadro = 0;
    novoInimigoEspinho->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigoEspinho->animacaoAndando.executarUmaVez = false;
    novoInimigoEspinho->animacaoAndando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoEspinho->animacaoAndando, novoInimigoEspinho->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoEspinho->animacaoAndando.quadros,
        novoInimigoEspinho->animacaoAndando.quantidadeQuadros,
        200,
        1,
        1,
        26, 
        28,
        false,
        1
    );

    novoInimigoEspinho->animacoes[INIMIGO_ESPINHO_ANDANDO] = &novoInimigoEspinho->animacaoAndando;
    quantidadeAnimacoes++;

    novoInimigoEspinho->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoEspinho;

}

void atualizarInimigoEspinho(InimigoEspinho *inimigo, GameWorld *gw, float delta) {

    Animacao *animacaoAtual = getAnimacaoAtualInimigoEspinho(inimigo);
    atualizarAnimacao(animacaoAtual, delta);

    inimigo->ret.x += inimigo->vel.x * delta;
    resolverColisaoInimigoMapaX(&inimigo->ret, &inimigo->vel, gw->mapa);

    inimigo->vel.y += gw->gravidade * delta;

    inimigo->ret.y += inimigo->vel.y * delta;
    resolverColisaoInimigoMapaY(&inimigo->ret, &inimigo->vel, &inimigo->noChao, gw->mapa);

    if(inimigo->vel.y > inimigo->velMaxQueda) {
        inimigo->vel.y = inimigo->velMaxQueda;
    }

    if(inimigo->noChao && !verificarSeTemChao(&inimigo->ret, &inimigo->vel, gw->mapa)) {
        inimigo->vel.x = -inimigo->vel.x;
    }

    resolverColisaoInimigoComInimigo(&inimigo->ret, &inimigo->vel, gw->mapa);

    inimigo->paraDireita = inimigo->vel.x > 0;

}

void destruirInimigoEspinho(InimigoEspinho *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoEspinho(InimigoEspinho *inimigo) {

    QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoEspinho(inimigo);
    desenharAnimacaoInimigoEspinho(inimigo, quadro, WHITE);
    
}

static void desenharAnimacaoInimigoEspinho(InimigoEspinho *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_ESPINHO_ANDANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoEspinho,
                (Rectangle) {
                    quadro->fonte.x,
                    quadro->fonte.y,
                    inimigo->paraDireita ? -quadro->fonte.width : quadro->fonte.width,
                    quadro->fonte.height
                },
                inimigo->ret,
                (Vector2) {0},
                0.0f,
                tonalidade
            );

        } 

       if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(inimigo->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoEspinho(inimigo));
}
