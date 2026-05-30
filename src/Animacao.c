#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "Animacao.h"

void criarQuadroAnimacao(Animacao *animacao, int quantidadeQuadros) {

    animacao->quadros = (QuadroAnimacao*) malloc(sizeof(QuadroAnimacao) * quantidadeQuadros);
}

void inicializarQuadroAnimacao(
    QuadroAnimacao *quadros,
    int quantidadeQuadros,
    int duracaoPadrao,
    int xInicioFonte,
    int yInicioFonte,
    int larguraFonte,
    int alturaFonte,
    bool aoContrario,
    int espacamentoFonte
) {

    int passo = abs(larguraFonte) + espacamentoFonte;

    for(int i = 0; i < quantidadeQuadros; i++) {

        int p = !aoContrario ? i : quantidadeQuadros - i - 1;

        quadros[i] = (QuadroAnimacao) {
            .fonte = {
                xInicioFonte + passo * p,
                yInicioFonte,
                larguraFonte,
                alturaFonte
            },
            .duracao = duracaoPadrao,
        };

    }

}


void destruirQuadroAnimacao(Animacao *animacao) {

    if(animacao != NULL) {
        free(animacao->quadros);
    }

}

void atualizarAnimacao(Animacao *animacao, float delta) {

    if(animacao->finalizada) {
        return;
    }

    animacao->contadorTempoQuadro += (int) (delta * 1000);

    QuadroAnimacao *quadro = getQuadroAtualAnimacao(animacao);

    if(animacao->contadorTempoQuadro >= quadro->duracao) {
        
        animacao->contadorTempoQuadro = 0;
        animacao->quadroAtual++;

        if(animacao->pararNoUltimoQuadro && animacao->quadroAtual >= animacao->quantidadeQuadros) {
            animacao->quadroAtual = animacao->quantidadeQuadros - 1;
            animacao->finalizada = true;
            return;
        }

        if(animacao->executarUmaVez && animacao->quadroAtual >= animacao->quantidadeQuadros) {
            animacao->quadroAtual = 0;
            animacao->finalizada = true;
            return;
        }

        animacao->quadroAtual %= animacao->quantidadeQuadros;
    
    }

}

int getNumeroQuadroAtualAnimacao(Animacao *animacao) {

    return animacao->quadroAtual % animacao->quantidadeQuadros;

}

QuadroAnimacao *getQuadroAtualAnimacao(Animacao *animacao) {

    if(animacao != NULL) {
        return &animacao->quadros[getNumeroQuadroAtualAnimacao(animacao)];
    }
    return NULL;

}

void reiniciarAnimacao(Animacao *animacao) {

    animacao->quadroAtual = 0;
    animacao->contadorTempoQuadro = 0;
    animacao->finalizada = false;

}

void sincronizarAnimacao(Animacao *destino, Animacao *origem) {

    if(destino->quantidadeQuadros != origem->quantidadeQuadros) {
        printf("quantidade de quadros diferente");
        return;
    }

    destino->quadroAtual = origem->quadroAtual;
    destino->contadorTempoQuadro = 0;

}