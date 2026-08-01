#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Jogador.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "Mapa.h"
#include "Animacao.h"
#include "ResourceManager.h"
#include "Colisao.h"

static void desenharAnimacaoJogador(Jogador *j, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualJogador(Jogador *j);
static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j);

static bool MOSTRAR_RETANGULO_COLISAO = false;

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor, GameWorld *gw) {

    Jogador *novoJogador = (Jogador*) malloc(sizeof(Jogador));

    novoJogador->ret = (Rectangle) {
        .x = x,
        .y = y,
        .width = largura,
        .height = altura,
    };
    novoJogador->vel = (Vector2) {0};
    novoJogador->cor = BLUE;
    novoJogador->plataformaAtual = NULL;

    novoJogador->vidas = 5;
    novoJogador->moedas = 0;

    novoJogador->velAndando = 225;
    novoJogador->velCorrendo = 325;
    novoJogador->velPulo = -425;
    novoJogador->velPuloCorrendo = -525;
    novoJogador->velMaxQueda = 600;
    novoJogador->velFreio = 1100;
    novoJogador->velDesacelarar = 800;
    novoJogador->velAcelerar = 600;
    novoJogador->velMax = 0;

    novoJogador->noChao = false;
    novoJogador->morto = false;
    novoJogador->paraDireita = true;
    novoJogador->freando = false;

    novoJogador->congelado = false;
    novoJogador->contadorTempoCongelado = 0;

    novoJogador->acelerado = false;
    novoJogador->contadorTempoAcelerado = 0;

    novoJogador->estado = JOGADOR_PARADO;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0;
    novoJogador->animacaoParado.finalizada = false;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    criarQuadroAnimacao(&novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,
        0,
        gw->personagemAtual == 0 ? 0 : 80,
        16,
        16,
        false,
        0
    );

    
    novoJogador->animacaoAndando.quantidadeQuadros = 3;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0;
    novoJogador->animacaoAndando.finalizada = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        90,
        0,
        gw->personagemAtual == 0 ? 16 : 96,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 3;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0;
    novoJogador->animacaoCorrendo.finalizada = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        60,
        0,
        gw->personagemAtual == 0 ? 16 : 96,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 1;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0;
    novoJogador->animacaoPulando.finalizada = false;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    criarQuadroAnimacao(&novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        1000,
        0,
        gw->personagemAtual == 0 ? 32 : 112,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoMorrendo.quantidadeQuadros = 1;
    novoJogador->animacaoMorrendo.quadroAtual = 0;
    novoJogador->animacaoMorrendo.contadorTempoQuadro = 0;
    novoJogador->animacaoMorrendo.finalizada = false;
    novoJogador->animacaoMorrendo.executarUmaVez = true;
    novoJogador->animacaoMorrendo.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoMorrendo, novoJogador->animacaoMorrendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoMorrendo.quadros,
        novoJogador->animacaoMorrendo.quantidadeQuadros,
        2000,
        0,
        gw->personagemAtual == 0 ? 48 : 128,
        16,
        16,
        false,
        0
    );

    novoJogador->animacaoFreando.quantidadeQuadros = 1;
    novoJogador->animacaoFreando.quadroAtual = 0;
    novoJogador->animacaoFreando.contadorTempoQuadro = 0;
    novoJogador->animacaoFreando.finalizada = false;
    novoJogador->animacaoFreando.executarUmaVez = true;
    novoJogador->animacaoFreando.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoJogador->animacaoFreando, novoJogador->animacaoFreando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoJogador->animacaoFreando.quadros,
        novoJogador->animacaoFreando.quantidadeQuadros,
        500,
        0,
        gw->personagemAtual == 0 ? 64 : 144,
        16,
        16,
        false,
        1
    );

    novoJogador->animacoes[JOGADOR_PARADO] = &novoJogador->animacaoParado;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_ANDANDO] = &novoJogador->animacaoAndando;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_PULANDO] = &novoJogador->animacaoPulando;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_MORRENDO] = &novoJogador->animacaoMorrendo;
    quantidadeAnimacoes++;

    novoJogador->animacoes[JOGADOR_FREANDO] = &novoJogador->animacaoFreando;
    quantidadeAnimacoes++;

    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;
    
    return novoJogador;
}

void destruirJogador(Jogador *j) {
    if(j != NULL) {
        for(int i = 0; i < j->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(j->animacoes[i]);
        }
        free(j);
    }
}


// Implementado para WASD e setinhas
void entradaJogador(Jogador *j) {
    
    if(j->estado != JOGADOR_MORRENDO) {

        EstadoJogador estadoAnterior = j->estado;

        bool correr = IsKeyDown(KEY_LEFT_SHIFT);
        bool esquerda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
        bool direita = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
           

        if(!j->acelerado) {
            j->velMax = correr? j->velCorrendo : j->velAndando;
        }

        if(esquerda) {
            j->paraDireita = false;

            if(j->vel.x > 0) {
                j->vel.x -= j->velFreio * GetFrameTime();
                j->freando = true;
                 if(j->vel.x < 0) {
                    j->vel.x = 0;
                    
                }
            } else {
                j->vel.x -= j->velAcelerar * GetFrameTime();
                if(j->vel.x < -j->velMax) {
                    j->vel.x = -j->velMax;
                }
            }
                
        } else if (direita) {
            j->paraDireita = true;
            
            if(j->vel.x < 0) {
                j->vel.x += j->velFreio * GetFrameTime();
                j->freando = true;
                if(j->vel.x > 0) {
                    j->vel.x = 0;
                   
                }
    
            } else {
                j->vel.x += j->velAcelerar * GetFrameTime();
                if(j->vel.x > j->velMax) {
                    j->vel.x = j->velMax;
                }
            }

        } else {
            if ( j->vel.x > 0 ) {
                j->vel.x -= j->velDesacelarar * GetFrameTime();
                    if ( j->vel.x < 0 ) {
                        j->vel.x = 0;
                    }
            } else if ( j->vel.x < 0 ) {
                j->vel.x += j->velDesacelarar * GetFrameTime();
                if ( j->vel.x > 0 ) {
                    j->vel.x = 0;
                }
            }
        }
            
        if(j->vel.x != 0 && j->noChao) {
            if(correr) {
                j->estado = JOGADOR_CORRENDO;
            } else {
                j->estado = JOGADOR_ANDANDO;
            }
        } 

        if(j->freando && j->noChao) {
            j->estado = JOGADOR_FREANDO;
        }
        
        bool pular = IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(0) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
        
        if(pular && j->noChao) {
            PlaySound(rm.somPulo);
            j->vel.y = correr ? j->velPuloCorrendo : j->velPulo;
            j->noChao = false;
            j->plataformaAtual = NULL;
        }
        
        if(j->vel.y != 0) {
            j->estado = JOGADOR_PULANDO;
        }
        
        if(!direita && !esquerda && j->noChao) {
            j->estado = JOGADOR_PARADO;
        }

        if(estadoAnterior == JOGADOR_ANDANDO && j->estado == JOGADOR_CORRENDO) {
            sincronizarAnimacao(&j->animacaoCorrendo, &j->animacaoAndando);
        } else if(estadoAnterior == JOGADOR_CORRENDO && j->estado == JOGADOR_ANDANDO) {
            sincronizarAnimacao(&j->animacaoAndando, &j->animacaoCorrendo);
        }

    }

}

void atualizarJogador(Jogador *j, GameWorld *gw, float delta) {

    if(!j->morto) {

        Animacao *animacaoAtual = getAnimacaoAtualJogador(j);
        atualizarAnimacao(animacaoAtual, delta);

        if(j->estado == JOGADOR_MORRENDO && animacaoAtual->finalizada && j->vidas > 0) {
            j->morto = true;
        } else if(j->estado == JOGADOR_MORRENDO && !(j->vidas > 0)) {
            j->morto = true;
        }

        if(j->congelado) {
            j->velAcelerar = 100;
            j->velDesacelarar = 100;
            j->velFreio = 100;

            j->contadorTempoCongelado += delta * 1000;

            if(j->contadorTempoCongelado >= 1750) {
                j->congelado = false;
                j->contadorTempoCongelado = 0;
                j->velFreio = 1100;
                j->velDesacelarar = 800;
                j->velAcelerar = 600;
            }
        }

        if(j->acelerado) {
            j->velAcelerar = 1000;
            j->velDesacelarar = 1200;
            j->velFreio = 1500;
            j->velMax = 600;

            j->contadorTempoAcelerado += delta * 1000;

            if(j->contadorTempoAcelerado >= 1750) {
                j->acelerado = false;
                j->contadorTempoAcelerado = 0;
                j->velFreio = 1100;
                j->velDesacelarar = 800;
                j->velAcelerar = 600;
            }
        }
        

        if(j->freando && animacaoAtual->finalizada) {
            j->freando = false;
        }

        if(j->estado != JOGADOR_MORRENDO) {

            verificarColisaoJogadorItem(gw);

            if (j->plataformaAtual != NULL) {
                ObstaculoMovel *o = j->plataformaAtual;

                bool aindaSobre = (j->ret.x + j->ret.width > o->ret.x) && (j->ret.x < o->ret.x + o->ret.width);

                if (aindaSobre && j->noChao) {
                    float dx = o->ret.x - o->posAnterior.x;
                    float dy = o->ret.y - o->posAnterior.y;
                    j->ret.x += dx;
                    j->ret.y += dy;
                } else {
                    j->plataformaAtual = NULL;
                }
            }
            
            j->ret.x += j->vel.x * delta;
            resolverColisaoJogadorMapaX(gw);

            j->vel.y += gw->gravidade * delta;
        
            if(j->vel.y > j->velMaxQueda) {
                j->vel.y = j->velMaxQueda;
            }
        
            j->ret.y += j->vel.y * delta;
            j->noChao = false;
            resolverColisaoJogadorMapaY(gw, delta);
        
            verificarColisaoJogadorInimigo(gw);
        
            if(j->moedas >= 100) {
                PlaySound(rm.somVida);
                j->moedas -= 100;
                j->vidas++;
            }

        }

    }

}

void desenharJogador(Jogador *j) {

    Color cor = {0};
    Rectangle retStatus = {0};
    
    if(j->acelerado) {

        cor = RED;

        retStatus = (Rectangle) {
            .x = j->ret.x,
            .y = j->ret.y - 15,
            .width = j->ret.width * ((1750.0f - j->contadorTempoAcelerado) / 1750.0f),
            .height = 7
        };

        DrawRectangleRec(
            (Rectangle) {
                .x = retStatus.x - 2,
                .y = retStatus.y + 2,
                .width = j->ret.width,
                .height = retStatus.height
            },
            BLACK
        );

        DrawRectangleRec(retStatus, cor);
        
    } else if (j->congelado) {

        cor = BLUE;

        retStatus = (Rectangle) {
            .x = j->ret.x,
            .y = j->ret.y - 15,
            .width = j->ret.width * ((1750.0f - j->contadorTempoCongelado) / 1750.0f),
            .height = 7
        };

        DrawRectangleRec(
            (Rectangle) {
                .x = retStatus.x - 2,
                .y = retStatus.y + 2,
                .width = j->ret.width,
                .height = retStatus.height
            },
            BLACK
        );

        DrawRectangleRec(retStatus, cor);
        
    } else {
        cor = WHITE;
    }
 
    QuadroAnimacao *quadro = getQuadroAnimacaoAtualJogador(j);
    desenharAnimacaoJogador(j, quadro, cor);

}

static void desenharAnimacaoJogador(Jogador *j, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle) {
                quadro->fonte.x,
                quadro->fonte.y,
                j->paraDireita ? quadro->fonte.width : -quadro->fonte.width,
                quadro->fonte.height
            },
            j->ret,
            (Vector2) {0},
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(j->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualJogador(Jogador *j) {

    return j->animacoes[j->estado];

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j) {

    return getQuadroAtualAnimacao(getAnimacaoAtualJogador(j));

}