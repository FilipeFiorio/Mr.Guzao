#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoNormal.h"
#include "ResourceManager.h"
#include "Animacao.h"
#include "Colisao.h"

static void desenharAnimacaoInimigoNormal(InimigoNormal *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoNormal(InimigoNormal *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoNormal *criarInimigoNormal(float x, float y, float largura, float altura, Color cor) {

    InimigoNormal *novoInimigoNormal = (InimigoNormal*) malloc(sizeof(InimigoNormal));

    novoInimigoNormal->ret.x = x;
    novoInimigoNormal->ret.y = y;
    novoInimigoNormal->ret.width = largura;
    novoInimigoNormal->ret.height = altura;
    novoInimigoNormal->vel = (Vector2) {100, 0};
    novoInimigoNormal->velMaxQueda = 600;
    novoInimigoNormal->estaVivo = true;
    novoInimigoNormal->noChao = false;
    novoInimigoNormal->paraDireita = false;
    novoInimigoNormal->cor = cor;
    novoInimigoNormal->estado = INIMIGO_NORMAL_ANDANDO;

    int quantidadeAnimacoes = 0;

    novoInimigoNormal->animacaoAndando.quantidadeQuadros = 4;
    novoInimigoNormal->animacaoAndando.quadroAtual = 0;
    novoInimigoNormal->animacaoAndando.contadorTempoQuadro = 0;
    novoInimigoNormal->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigoNormal->animacaoAndando.executarUmaVez = false;
    novoInimigoNormal->animacaoAndando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoNormal->animacaoAndando, novoInimigoNormal->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoNormal->animacaoAndando.quadros,
        novoInimigoNormal->animacaoAndando.quantidadeQuadros,
        200,
        1,
        1,
        21, 
        15,
        false,
        1
    );

    novoInimigoNormal->animacaoMorrendo.quantidadeQuadros = 3;
    novoInimigoNormal->animacaoMorrendo.quadroAtual = 0;
    novoInimigoNormal->animacaoMorrendo.contadorTempoQuadro = 0;
    novoInimigoNormal->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigoNormal->animacaoMorrendo.executarUmaVez = true;
    novoInimigoNormal->animacaoMorrendo.finalizada = false;
    criarQuadroAnimacao(&novoInimigoNormal->animacaoMorrendo, novoInimigoNormal->animacaoMorrendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoNormal->animacaoMorrendo.quadros,
        novoInimigoNormal->animacaoMorrendo.quantidadeQuadros,
        200,
        1,
        1,
        15,
        15,
        false,
        1
    );

    novoInimigoNormal->animacoes[INIMIGO_NORMAL_ANDANDO] = &novoInimigoNormal->animacaoAndando;
    quantidadeAnimacoes++;

    novoInimigoNormal->animacoes[INIMIGO_NORMAL_MORRENDO] = &novoInimigoNormal->animacaoMorrendo;
    quantidadeAnimacoes++;

    novoInimigoNormal->quantidadeAnimacoes = quantidadeAnimacoes;


    return novoInimigoNormal;

}

void atualizarInimigoNormal(InimigoNormal *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoNormal(inimigo);
        atualizarAnimacao(animacaoAtual, delta);
        if(inimigo->estado == INIMIGO_NORMAL_MORRENDO && animacaoAtual->finalizada) {
            inimigo->estaVivo = false;
        }

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


}

void destruirInimigoNormal(InimigoNormal *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoNormal(InimigoNormal *inimigo) {

    if(inimigo->estaVivo) {
        QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoNormal(inimigo);
        desenharAnimacaoInimigoNormal(inimigo, quadro, WHITE);
    } 

}

static void desenharAnimacaoInimigoNormal(InimigoNormal *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_NORMAL_ANDANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoNormal,
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

        } else if(inimigo->estado == INIMIGO_NORMAL_MORRENDO) {
            DrawTexturePro(
                rm.texturaInimigoMorrendo,
                (Rectangle) {
                    quadro->fonte.x,
                    quadro->fonte.y,
                    quadro->fonte.width,
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

static Animacao *getAnimacaoAtualInimigoNormal(InimigoNormal *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoNormal(InimigoNormal *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoNormal(inimigo));
}
