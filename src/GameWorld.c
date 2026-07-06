/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GameWorld.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Jogador.h"
#include "Inimigo.h"
#include "Mapa.h"
#include "MapaMundo.h"
#include "Utils.h"


#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void atualizarCamera(GameWorld *gw);
static void desenharFundo(GameWorld *gw);
static void verificarMorteJogador(GameWorld *gw);
static void verificarGameOver(GameWorld *gw);
static void reiniciarJogo(GameWorld *gw);
static void inicializarGW(GameWorld *gw);
static void desenharHud(GameWorld *gw);
static void passarFase(GameWorld *gw);
static void iniciarTransicao(GameWorld *gw, EstadoJogo proximoEstado);
static void desenharMiniMapa(GameWorld *gw);

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) calloc( 1, sizeof( GameWorld ) );
    if ( gw == NULL ) {
        return NULL;
    }

    inicializarGW( gw );

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {

    if ( gw == NULL ) {
        return;
    }

    if (gw->mapa != NULL) {
        destruirMapa(gw->mapa);
    }

    if (gw->mapaMundo != NULL) {
        destruirMapaMundo(gw->mapaMundo);
    }

    StopMusicStream(rm.musicaFase1);
    StopMusicStream(rm.musicaFase2);
    StopMusicStream(rm.musicaFase3);
    StopMusicStream(rm.musicaInicio);
    StopMusicStream(rm.musicaMundo);

    free(gw);

}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    switch (gw->estado) {

        case ESTADO_JOGO_GAMEPLAY:

            if(gw->mapa->jogador->estado != JOGADOR_MORRENDO) {

                switch (gw->faseAtual) {
                    case 1:
                        if(!IsMusicStreamPlaying(rm.musicaFase1)) {
                            PlayMusicStream(rm.musicaFase1);
                        } else {
                            UpdateMusicStream(rm.musicaFase1);
                        }
                        break;
                    case 2:
                        if(!IsMusicStreamPlaying(rm.musicaFase2)) {
                            PlayMusicStream(rm.musicaFase2);
                        } else {
                            UpdateMusicStream(rm.musicaFase2);
                        }
                        break;
                    case 3:
                        if(!IsMusicStreamPlaying(rm.musicaFase3)) {
                            PlayMusicStream(rm.musicaFase3);
                        } else {
                            UpdateMusicStream(rm.musicaFase3);
                        }
                        break;
                    default:
                        TraceLog(LOG_ERROR, "Numero de fase atual inesperado");
                        break;
                }
            }


            if(IsKeyPressed(KEY_P)) {
                gw->estado = ESTADO_JOGO_PAUSE;
                PlaySound(rm.somPausa);
            }

            gw->timerJogo -= (int) (1000 * delta);

            if(gw->timerJogo == 60000) {
                PlaySound(rm.somHurryUp);
            }
        
            if(gw->timerJogo <= 0) {
                gw->mapa->jogador->morto = true;
            }
        
            atualizarMapa(gw->mapa, gw, delta);

            if (gw->mapa->faseCompleta) {
                gw->mapaMundo->fases[gw->faseAtual - 1].finalizado = true; // faseAtual é 1-based
            }

            verificarMorteJogador(gw);
            atualizarCamera(gw);
            verificarGameOver(gw);

            if(gw->mapaMundo->fases[gw->faseAtual - 1].finalizado) {
                passarFase(gw);
            }

            break;

        case ESTADO_JOGO_MAPA_MUNDO:

            if(!IsMusicStreamPlaying(rm.musicaMundo)) {
                PlayMusicStream(rm.musicaMundo);
            } else {
                UpdateMusicStream(rm.musicaMundo);
            }

            atualizarMapaMundo(gw, delta);

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            if(IsKeyPressed(KEY_ENTER)) {
                PlaySound(rm.somBotao);
                inicializarGW(gw);
                gw->estado = ESTADO_JOGO_INICIO;
            }
            
            break;

        case ESTADO_JOGO_INICIO:

            if(!IsMusicStreamPlaying(rm.musicaInicio)) {
                PlayMusicStream(rm.musicaInicio);
            } else {
                UpdateMusicStream(rm.musicaInicio);
            }

            if(IsKeyPressed(KEY_ENTER)) {
                PlaySound(rm.somBotao);
                StopMusicStream(rm.musicaInicio);
                iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
            }

            break;

        case ESTADO_JOGO_PAUSE:

            if(IsKeyPressed(KEY_P)) {
                PlaySound(rm.somPausa);
                gw->estado = ESTADO_JOGO_GAMEPLAY;
            }

            if(IsKeyPressed(KEY_BACKSPACE)) {
                PlaySound(rm.somBotao); 
                switch (gw->faseAtual) {
                case 1:
                    StopMusicStream(rm.musicaFase1);
                    break;
                case 2:
                    StopMusicStream(rm.musicaFase2);
                    break;
                case 3:
                    StopMusicStream(rm.musicaFase3);
                    break;
                default:
                    TraceLog(LOG_ERROR, "Numero de fase atual inesperado");
                    break;
                }

                gw->vidasSalvas = gw->mapa->jogador->vidas;
                gw->moedasSalvas = gw->mapa->jogador->moedas;

                iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
            }

            break;

        case ESTADO_JOGO_MORTE:

            gw->timerMorte += delta;

            if(!IsSoundPlaying(rm.somMorte)) {
                if(gw->mapa->jogador->vidas >= 1) {
                    reiniciarJogo(gw);
                } else {
                    PlaySound(rm.somGameOver);
                    destruirMapa(gw->mapa);
                    gw->mapa = NULL;
                    gw->estado = ESTADO_JOGO_GAME_OVER;
                }
            }

            break;

        case ESTADO_JOGO_FIM:

            if(IsKeyPressed(KEY_ENTER)){
                if(!IsSoundPlaying(rm.somVitoria)) {
                    gw->estado = ESTADO_JOGO_INICIO;
                }
            }

            break;
        
        case ESTADO_JOGO_FADE_IN:
            
            gw->alphaTransicao -= 600 * delta;

            if (gw->alphaTransicao <= 0) {
                gw->alphaTransicao = 0;
                gw->estado = gw->proximoEstado;
            }

            break;
        
        case ESTADO_JOGO_FADE_OUT:

            gw->alphaTransicao += 600 * delta;

            if (gw->alphaTransicao >= 255) {
                gw->alphaTransicao = 255;
                gw->estado = ESTADO_JOGO_FADE_IN;
            }

            break;

        default:
            TraceLog(LOG_ERROR, "Estado de jogo inesperado");
            break;
    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {
    
    BeginDrawing();

    switch (gw->estado) {

        case ESTADO_JOGO_GAMEPLAY:

            ClearBackground( (Color) {175, 231, 255, 255} );
        
            BeginMode2D(gw->camera);

            desenharFundo(gw);
            desenharMapa(gw->mapa);

            EndMode2D();

            desenharHud(gw);
            desenharMiniMapa(gw);

            break;

        case ESTADO_JOGO_MAPA_MUNDO:

            desenharMapaMundo(gw->mapaMundo);
            desenharHud(gw); // dps

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            ClearBackground(BLACK);
            drawTextAlinhado("GAME OVER", 200, 50, WHITE, CENTRO);
            drawTextAlinhado("Aperte [ENTER] para continuar", 400, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_INICIO:

            ClearBackground(WHITE);

            TransformacaoTela t = calcularTransformacaoCover(rm.texturaInicio);

            Rectangle origem = {0, 0, (float) rm.texturaInicio.width, (float) rm.texturaInicio.height};
            Rectangle destino = {t.offsetX, t.offsetY, (float) rm.texturaInicio.width * t.escala, (float) rm.texturaInicio.height * t.escala };

            DrawTexturePro(rm.texturaInicio, origem, destino, (Vector2) {0}, 0.0f, WHITE);

            drawTextAlinhado("Mr. Guzão", 200, 72, WHITE, CENTRO);
            drawTextAlinhado("Aperte [ENTER] para iniciar", 500, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_PAUSE:    

            ClearBackground( (Color) {175, 231, 255, 255} );

            BeginMode2D(gw->camera);
            
            desenharFundo(gw);
            desenharMapa(gw->mapa);
            
            EndMode2D();
            
            drawTextAlinhado("Jogo Pausado", 200, 25, WHITE, CENTRO);
            desenharHud(gw);
            desenharMiniMapa(gw);

            break;

        case ESTADO_JOGO_MORTE:

            ClearBackground( (Color) {175, 231, 255, 255} );
        
            BeginMode2D(gw->camera);

            desenharFundo(gw);
            desenharMapa(gw->mapa);

            EndMode2D();

            desenharHud(gw);

            break;

        case ESTADO_JOGO_FIM:

            ClearBackground(BLACK);

            drawTextAlinhado("Você Venceu!!!", 200, 50, WHITE, CENTRO);
            if(!IsSoundPlaying(rm.somVitoria)) {
                drawTextAlinhado("Aperte ENTER para voltar para o início", 400, 25, WHITE, CENTRO);
            }

            break;

        case ESTADO_JOGO_FADE_IN:
        case ESTADO_JOGO_FADE_OUT:

            DrawRectangle(
                0,
                0,
                GetRenderWidth(),
                GetRenderHeight(),
                (Color){0,0,0,gw->alphaTransicao}
            );

            drawTextAlinhado("Carregando...", GetRenderHeight() * 0.95f, 25, WHITE, DIREITA);

            break;

        default:
            TraceLog(LOG_ERROR, "Estado de jogo inesperado");
            break;

    }

    EndDrawing();

}

static void desenharFundo( GameWorld *gw ) {

    Texture2D fundo = {0};

    switch (gw->faseAtual) {
        case 1:
            fundo = rm.texturaFundo;
            break;
        case 2:
            fundo = rm.texturaFundoNeve;
            break;
        case 3:
            fundo = rm.texturaFundoDeserto;
            break;
        default:
            TraceLog(LOG_ERROR, "Numero de fase inesperado");
            break;
    }

    int larguraFundo = fundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * 200 );

    for ( int i = 0; i <= repeticoes + 1; i++ ) {
        DrawTexture( fundo, larguraFundo * i - deslocamentoParallax, alturaMapa - fundo.height, WHITE );
    }

}

static void atualizarCamera(GameWorld *gw) {

    Jogador *jogador = gw->mapa->jogador;
    Camera2D *camera = &gw->camera;

    camera->offset.x = GetRenderWidth() / 2;
    camera->offset.y = GetRenderHeight() / 2;

    camera->target.x = (int) roundf(jogador->ret.x + jogador->ret.width);
    camera->target.y = (int) roundf(jogador->ret.y + jogador->ret.height);

    int minX = GetRenderWidth() / 2 + 50;
    int maxX = calcularLarguraMapa(gw->mapa) - GetRenderWidth() / 2 - 50;
    int maxY = calcularAlturaMapa(gw->mapa) - GetRenderHeight() / 2;

    if(camera->target.x < minX) {
        camera->target.x = minX;
    } else if (camera->target.x > maxX) {
        camera->target.x = maxX;
    }

    if(camera->target.y > maxY) {
        camera->target.y = maxY;
    }

}

static void verificarMorteJogador(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;

    bool foraDoMapa = (j->ret.y > calcularAlturaMapa(gw->mapa));

    if(foraDoMapa && j->vidas > 1) {
        PlaySound(rm.somMorte);
    }


    if(foraDoMapa || j->morto) {

        switch (gw->faseAtual) { 
            case 1:
                StopMusicStream(rm.musicaFase1);
                break;
            case 2:
                StopMusicStream(rm.musicaFase2);
                break;
            case 3:
                StopMusicStream(rm.musicaFase3);
                break;
            default:
                TraceLog(LOG_ERROR, "Numero de fase inesperado");
                break;
        }

        j->vidas--;
        gw->estado = ESTADO_JOGO_MORTE;
        gw->timerMorte = 0;
        j->morto = false;
        
    } 

}

static void verificarGameOver(GameWorld *gw) {

    if(gw->mapa->jogador->vidas <= 0) {
        PlaySound(rm.somGameOver);
        destruirMapa(gw->mapa);
        gw->mapa = NULL;
        gw->estado = ESTADO_JOGO_GAME_OVER;
    }

}

static void reiniciarJogo(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;

    int vidaAtual = j->vidas;
    int moedaAtual = j->moedas;
    int faseAtual = gw->faseAtual;

    destruirMapa(gw->mapa);

    char caminhoMapa[100];
    sprintf(caminhoMapa, "resources/mapas/fase%d.txt", faseAtual);
    gw->mapa = carregarMapa(caminhoMapa);

    gw->gravidade = 600;
    gw->timerJogo = 180000;
    gw->faseAtual = faseAtual;
    gw->estado = ESTADO_JOGO_GAMEPLAY;
    
    gw->mapa->jogador->vidas = vidaAtual;
    gw->mapa->jogador->moedas = moedaAtual;
    
}

static void inicializarGW(GameWorld *gw) {

    if (gw->mapaMundo != NULL) {
        destruirMapaMundo(gw->mapaMundo);
    }

    
    gw->faseAtual = 1;
    gw->mapaMundo = criarMapaMundo(3);
    gw->gravidade = 600;
    gw->timerJogo = 180000;
    gw->alphaTransicao = 0;
    gw->timerMorte = 0;
    gw->proximoEstado = ESTADO_JOGO_INICIO;
    gw->mapa = NULL;
    gw->estado = ESTADO_JOGO_INICIO;
    gw->vidasSalvas = 5;
    gw->moedasSalvas = 0;

    gw->camera = (Camera2D) {
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

}

static void passarFase(GameWorld *gw) {

    gw->vidasSalvas = gw->mapa->jogador->vidas;
    gw->moedasSalvas = gw->mapa->jogador->moedas;

    destruirMapa(gw->mapa);
    gw->mapa = NULL;

    gw->faseAtual++;

    if(gw->faseAtual > 3) {
        PlaySound(rm.somVitoria);
        gw->estado = ESTADO_JOGO_FIM;
        return;
    }

    gw->timerJogo = 180000;
    iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
}

static void iniciarTransicao(GameWorld *gw, EstadoJogo proximoEstado) {

    gw->proximoEstado = proximoEstado;
    gw->estado = ESTADO_JOGO_FADE_OUT;
    gw->alphaTransicao = 0;

}

static void desenharHud(GameWorld *gw) {

    int vidas = 0;
    int moedas = 0;

    if(gw->mapa != NULL) {
        vidas = gw->mapa->jogador->vidas;
        moedas = gw->mapa->jogador->moedas;
    } else {
        vidas = gw->vidasSalvas;
        moedas = gw->moedasSalvas;
    }

    const int margem = 25;     
    const int espacoEntreGrupos = 25; 
    const float tamanhoCoracao = 25;
    const float espacamentoCoracao = 28;

    int maxCoracoesExibidos = 10;
    int vidasParaDesenhar = vidas < maxCoracoesExibidos ? vidas : maxCoracoesExibidos;

    int cursorX = margem;

    for (int i = 0; i < vidasParaDesenhar; i++) {
        desenharCoracao((Vector2){ cursorX, 25 }, tamanhoCoracao, (Color){ 220, 40, 60, 255 });
        cursorX += espacamentoCoracao;
    }

    if (vidas > maxCoracoesExibidos) {
        const char *textoExtra = TextFormat("x%d", vidas);
        desenharTextoContornado(textoExtra, cursorX, 25, 18, WHITE);
        cursorX += MeasureText(textoExtra, 18);
    } else if (vidasParaDesenhar > 0) {
        cursorX = margem + (vidasParaDesenhar - 1) * espacamentoCoracao + tamanhoCoracao;
    }

    cursorX += espacoEntreGrupos;

    desenharIconeMoeda((Vector2){ cursorX, 25 }, 12);
    cursorX += 12 * 2 + 6; 

    const char *textoMoedas = TextFormat("x %d", moedas);
    desenharTextoContornado(textoMoedas, cursorX, 25, 20, (Color){ 255, 230, 150, 255 });

    if(gw->estado == ESTADO_JOGO_GAMEPLAY || gw->estado == ESTADO_JOGO_PAUSE || gw->estado == ESTADO_JOGO_MORTE) {

        int segundosRestantes = gw->timerJogo / 1000;
        int minutos = segundosRestantes / 60;
        int segundos = segundosRestantes % 60;

        Color corTempo = WHITE;

        if (segundosRestantes <= 60) {
            if(segundosRestantes % 2 == 0) {
                corTempo = RED;
            } else {
                corTempo = (Color) {130, 31, 45, 255};
            }
        }

        const char *textoTempo = TextFormat("%02d:%02d", minutos, segundos);
        drawTextAlinhado(textoTempo, 25, 24, corTempo, CENTRO);
    }

}

static void desenharMiniMapa(GameWorld *gw) {
    
    Mapa *m = gw->mapa;
    Jogador *j = m->jogador;

    const int tamanhoFase = calcularLarguraMapa(m);

    const int tamanhoMiniMapa = 300;
    const int alturaMiniMapa = 15;
    const int xIniMiniMapa = GetRenderWidth() - tamanhoMiniMapa - 25;
    const int yIniMiniMapa = 30;

    int xChegada = xIniMiniMapa + tamanhoMiniMapa - alturaMiniMapa;
    int yChegada = 30;

    Rectangle retMapa = {xIniMiniMapa, yIniMiniMapa, tamanhoMiniMapa, alturaMiniMapa};
    Rectangle retChegada = {xChegada, yChegada, alturaMiniMapa, alturaMiniMapa};

    DrawRectangleRec(retMapa, (Color) {255, 255, 255, 150});
    DrawRectangleRec(retChegada, RED);

    int xJogador = xIniMiniMapa;
    int yJogador = 30;

    Rectangle retJogador = {xJogador, yJogador, alturaMiniMapa, alturaMiniMapa};
    
    retJogador.x = (xIniMiniMapa + (j->ret.x / tamanhoFase)  * tamanhoMiniMapa) - alturaMiniMapa;

    DrawTexturePro(
        rm.texturaJogadorMapa,
        (Rectangle) {
            0,
            0,
            rm.texturaJogadorMapa.width,
            rm.texturaJogadorMapa.height
        },
        (Rectangle) {
            retJogador.x, 
            retJogador.y,
            alturaMiniMapa,
            alturaMiniMapa 
        },
        (Vector2) {0},
        0.0f,
        WHITE
    );


}
