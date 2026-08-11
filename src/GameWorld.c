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
#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
#include "raylib/raygui.h"       // other compilation units must only include
#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void atualizarCamera(GameWorld *gw);
static void desenharFundo(GameWorld *gw);
static void verificarMorteJogador(GameWorld *gw);
static void verificarGameOver(GameWorld *gw);
static void reiniciarFase(GameWorld *gw);
static void voltarParaCheckpoint(GameWorld *gw);
static void salvarCheckpoint(GameWorld *gw);
static void inicializarGW(GameWorld *gw);
static void desenharHud(GameWorld *gw);
static void passarFase(GameWorld *gw);
static void iniciarTransicao(GameWorld *gw, EstadoJogo proximoEstado);
static void desenharMiniMapa(GameWorld *gw);

static bool inverterAlpha = false;
static EstadoJogo estadoAntesPause = 67;

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

        
        case ESTADO_JOGO_APRESENTACAO:

            HideCursor();

            if(!inverterAlpha) {
                gw->alphaTransicao += 100 * delta;
                if(gw->alphaTransicao >= 255) {
                    inverterAlpha = true;
                }
            } else {
                gw->alphaTransicao -= 100 * delta;
                if(gw->alphaTransicao <= 0) {
                    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
                    gw->estado = ESTADO_JOGO_INICIO;
                }
            }

            break;
        
        case ESTADO_JOGO_GAMEPLAY:

            HideCursor();

            if(gw->mapa->jogador->estado != JOGADOR_MORRENDO) {

                switch (gw->faseAtual) {
                    case 1:
                    case 2:
                    case 3:
                        if(!IsMusicStreamPlaying(rm.musicaFase1)) {
                            PlayMusicStream(rm.musicaFase1);
                        } else {
                            UpdateMusicStream(rm.musicaFase1);
                        }
                        break;
                    case 4:
                    case 5:
                    case 6:
                        if(!IsMusicStreamPlaying(rm.musicaFase2)) {
                            PlayMusicStream(rm.musicaFase2);
                        } else {
                            UpdateMusicStream(rm.musicaFase2);
                        }
                        break;
                    case 7:
                    case 8:
                    case 9:
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


            if(IsKeyPressed(KEY_ESCAPE)) {
                SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
                estadoAntesPause = ESTADO_JOGO_GAMEPLAY;
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
                gw->mapaMundo->fases[gw->faseAtual - 1].finalizado = true;
                salvarCheckpoint(gw);
            }

            verificarMorteJogador(gw);
            atualizarCamera(gw);
            verificarGameOver(gw);

            if(gw->mapaMundo->fases[gw->faseAtual - 1].finalizado) {
                passarFase(gw);
            }

            break;

        case ESTADO_JOGO_MAPA_MUNDO:

            HideCursor();

            if(IsKeyPressed(KEY_ESCAPE)) {
                SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
                estadoAntesPause = ESTADO_JOGO_MAPA_MUNDO;
                gw->estado = ESTADO_JOGO_PAUSE;
                PlaySound(rm.somPausa);
            }

            if(!IsMusicStreamPlaying(rm.musicaMundo)) {
                PlayMusicStream(rm.musicaMundo);
            } else {
                UpdateMusicStream(rm.musicaMundo);
            }

            atualizarMapaMundo(gw, delta);

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            ShowCursor();

            if(gw->checkpointAtivo) {
                if(IsKeyPressed(KEY_ENTER)) {
                    PlaySound(rm.somBotao);
                    voltarParaCheckpoint(gw);
                }
            }

            if(IsKeyPressed(KEY_BACKSPACE)) {
                PlaySound(rm.somBotao);
                inicializarGW(gw);
                iniciarTransicao(gw, ESTADO_JOGO_INICIO);
            }
            
            break;

        case ESTADO_JOGO_INICIO:

            ShowCursor();

            if(!IsMusicStreamPlaying(rm.musicaInicio)) {
                PlayMusicStream(rm.musicaInicio);
            } else {
                UpdateMusicStream(rm.musicaInicio);
            }

            break;

        case ESTADO_JOGO_PERSONAGEM:

            ShowCursor();

            break;

        case ESTADO_JOGO_PAUSE:

            ShowCursor();

            if(IsKeyPressed(KEY_ESCAPE)) {
                PlaySound(rm.somPausa);
                gw->estado = estadoAntesPause;
                estadoAntesPause = 67;
            }

            break;

        case ESTADO_JOGO_MORTE:

            HideCursor();

            gw->timerMorte += delta;

            if(!IsSoundPlaying(rm.somMorte)) {
                if(gw->mapa->jogador->vidas >= 1) {
                    reiniciarFase(gw);
                } else {
                    PlaySound(rm.somGameOver);
                    destruirMapa(gw->mapa);
                    gw->mapa = NULL;
                    gw->estado = ESTADO_JOGO_GAME_OVER;
                }
            }

            break;

        case ESTADO_JOGO_FIM:

            HideCursor();

            if(IsKeyPressed(KEY_ENTER)){
                if(!IsSoundPlaying(rm.somVitoria)) {
                    gw->estado = ESTADO_JOGO_INICIO;
                }
            }

            break;
        
        case ESTADO_JOGO_FADE_IN:

            HideCursor();
            
            gw->alphaTransicao -= 600 * delta;

            if (gw->alphaTransicao <= 0) {
                gw->alphaTransicao = 0;
                gw->estado = gw->proximoEstado;
            }

            break;
        
        case ESTADO_JOGO_FADE_OUT:

            HideCursor();

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
    
    GuiSetFont(rm.fonte);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 3);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt((Color){45, 92, 170, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){70, 130, 220, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){20, 60, 120, 255}));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(YELLOW));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(ORANGE));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));

    switch (gw->estado) {

        case ESTADO_JOGO_APRESENTACAO:

        drawTextAlinhado("BLTS Studios", 300, 80, (Color) {255, 255, 255, gw->alphaTransicao}, CENTRO);
        drawTextAlinhado("Apresenta", 400, 25, (Color) {255, 255, 255, gw->alphaTransicao}, CENTRO);

        break;

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
            desenharHud(gw);

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            ClearBackground(BLACK);
            drawTextAlinhado("GAME OVER", 160, 50, WHITE, CENTRO);

            drawTextAlinhado("[ENTER] Voltar para o último checkpoint", 350, 25, gw->checkpointAtivo ? WHITE : DARKGRAY, CENTRO);
            drawTextAlinhado("[BACKSPACE] Voltar ao menu inicial", 400, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_INICIO:

            ClearBackground(WHITE);

            TransformacaoTela t = calcularTransformacaoCover(rm.texturaInicio);

            Rectangle origem = {0, 0, (float) rm.texturaInicio.width, (float) rm.texturaInicio.height};
            Rectangle destino = {t.offsetX, t.offsetY, (float) rm.texturaInicio.width * t.escala, (float) rm.texturaInicio.height * t.escala };

            DrawTexturePro(rm.texturaInicio, origem, destino, (Vector2) {0}, 0.0f, WHITE);

            drawTextAlinhado("Mr. Guzão", 200, 72, WHITE, CENTRO);


            Rectangle botaoIniciar = {GetRenderWidth() * 0.5f - 150, GetRenderHeight() * 0.6f, 300, 100};
            Rectangle botaoSair = {botaoIniciar.x, GetRenderHeight() * 0.7f, botaoIniciar.width, botaoIniciar.height};

            if(GuiButton(botaoIniciar, "JOGAR")) {
                PlaySound(rm.somBotao);
                StopMusicStream(rm.musicaInicio);
                iniciarTransicao(gw, ESTADO_JOGO_PERSONAGEM);
            }

            if(GuiButton(botaoSair, "SAIR")) {
                gw->fecharJogo = true;
            }

            if(CheckCollisionPointRec(GetMousePosition(), botaoIniciar) || CheckCollisionPointRec(GetMousePosition(), botaoSair)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            } else {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            break;

        case ESTADO_JOGO_PERSONAGEM:

            ClearBackground(BLACK);

            drawTextAlinhado("Escolha seu Personagem:", 200, 25, WHITE, CENTRO);

            Rectangle ret1 = {
                    GetRenderWidth() * 0.2f,
                    GetRenderHeight() * 0.4f,
                    GetRenderWidth() * 0.2f,
                    GetRenderHeight() * 0.5f 
            };

            Rectangle ret2 = {
                    GetRenderWidth() * 0.8f - ret1.width,
                    GetRenderHeight() * 0.4f,
                    GetRenderWidth() * 0.2f,
                    GetRenderHeight() * 0.5f 
            };

            if(CheckCollisionPointRec(GetMousePosition(), ret1)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRounded(ret1, 0.1f, 0, LIGHTGRAY);
                DrawTextPro(rm.fonte, 
                    "Mr. Guzão",
                    (Vector2) {
                        ret1.x + (ret1.width - MeasureTextEx(rm.fonte, "Mr. Guzão", 15, 0.0f).x) / 2,
                        ret1.y + ret1.height + 10
                    },
                    (Vector2) {0},
                    0.0f,
                    15,
                    0, 
                    WHITE
                );
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    PlaySound(rm.somBotao);
                    gw->personagemAtual = 0;
                    iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
                }

            } else if(CheckCollisionPointRec(GetMousePosition(), ret2)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRounded(ret2, 0.1f, 0, LIGHTGRAY);
                DrawTextPro(rm.fonte, 
                    "Mr. Guzinho",
                    (Vector2) {
                        ret2.x + (ret2.width - MeasureTextEx(rm.fonte, "Mr. Guzinho", 15, 0.0f).x) / 2,
                        ret2.y + ret2.height + 10
                    },
                    (Vector2) {0},
                    0.0f,
                    15,
                    0, 
                    WHITE
                );
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    PlaySound(rm.somBotao);
                    gw->personagemAtual = 1;
                    iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
                }
            } else {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            DrawTexturePro(
                rm.texturaJogador,
                (Rectangle) {0, 0, 16, 16},
                (Rectangle) {
                    ret1.x + 5,
                    ret1.y + 5,
                    ret1.width - 10,
                    ret1.height - 10
                },
                (Vector2) {0},
                0.0f,
                WHITE
            );

            DrawTexturePro(
                rm.texturaJogador,
                (Rectangle) {0, 80, 16, 16},
                (Rectangle) {
                    ret2.x + 5,
                    ret2.y + 5,
                    ret2.width - 10,
                    ret2.height - 10
                },
                (Vector2) {0},
                0.0f,
                WHITE
            );
                              
            break;

        case ESTADO_JOGO_PAUSE:  

            switch(estadoAntesPause) {

                case ESTADO_JOGO_MAPA_MUNDO:

                    desenharMapaMundo(gw->mapaMundo);
                    desenharHud(gw);
                    drawTextAlinhado("JOGO PAUSADO", GetRenderHeight() * 0.3f, 40, WHITE, CENTRO);

                    Rectangle botaoResumir = {GetRenderWidth() * 0.5f - 150, GetRenderHeight() * 0.4f, 300, 100};
                    Rectangle botaoPersonagem = {botaoResumir.x, GetRenderHeight() * 0.5f, botaoResumir.width, botaoResumir.height};
                    Rectangle botaoReiniciar = {botaoResumir.x, GetRenderHeight() * 0.6f, botaoResumir.width, botaoResumir.height};
                    Rectangle botaoSair = {botaoResumir.x, GetRenderHeight() * 0.7f, botaoResumir.width, botaoResumir.height};

                    if(GuiButton(botaoResumir, "RESUMIR")) {
                        PlaySound(rm.somBotao);
                        gw->estado = estadoAntesPause;
                        estadoAntesPause = 67;
                    }

                    if(GuiButton(botaoPersonagem, "PERSONAGEM")) {
                        PlaySound(rm.somBotao);
                        gw->estado = ESTADO_JOGO_PERSONAGEM;
                        estadoAntesPause = 67;
                    }

                    // talvez tenha memory leak
                    if(GuiButton(botaoReiniciar, "REINICIAR")) {
                        PlaySound(rm.somBotao);
                        inicializarGW(gw);
                    }

                    if(GuiButton(botaoSair, "SAIR")) {
                        PlaySound(rm.somBotao);
                        gw->fecharJogo = true;
                    }

                    if(CheckCollisionPointRec(GetMousePosition(), botaoResumir)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    }else if(CheckCollisionPointRec(GetMousePosition(), botaoPersonagem)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    }else if(CheckCollisionPointRec(GetMousePosition(), botaoReiniciar)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    }else if(CheckCollisionPointRec(GetMousePosition(), botaoSair)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    } else {
                        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    }

                    break;
                case ESTADO_JOGO_GAMEPLAY:
                    ClearBackground( (Color) {175, 231, 255, 255} );
        
                    BeginMode2D(gw->camera);
                    
                    desenharFundo(gw);
                    desenharMapa(gw->mapa);
                    
                    EndMode2D();
                    
                    drawTextAlinhado("JOGO PAUSADO", GetRenderHeight() * 0.3f, 40, WHITE, CENTRO);
                    desenharHud(gw);
                    desenharMiniMapa(gw);

                    Rectangle botaoResumirFase = {GetRenderWidth() * 0.5f - 150, GetRenderHeight() * 0.4f, 300, 100};
                    Rectangle botaoReiniciarFase = {botaoResumirFase.x, GetRenderHeight() * 0.5f, botaoResumirFase.width, botaoResumirFase.height};
                    Rectangle botaoSairFase = {botaoResumirFase.x, GetRenderHeight() * 0.6f, botaoResumirFase.width, botaoResumirFase.height};

                    if(GuiButton(botaoResumirFase, "RESUMIR")) {
                        PlaySound(rm.somBotao);
                        gw->estado = estadoAntesPause;
                        estadoAntesPause = 67;
                    }

                    if(GuiButton(botaoReiniciarFase, "REINICIAR")) {
                        PlaySound(rm.somBotao);
                        reiniciarFase(gw);
                    }

                    if(GuiButton(botaoSairFase, "SAIR")) {
                        PlaySound(rm.somBotao);
                        iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);
                    }

                    if(CheckCollisionPointRec(GetMousePosition(), botaoResumirFase)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    }else if(CheckCollisionPointRec(GetMousePosition(), botaoReiniciarFase)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    }else if(CheckCollisionPointRec(GetMousePosition(), botaoSairFase)) {
                        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                    } else {
                        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    }
                    break;

                default:
                    break;

            }

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
        case 2:
        case 3:
            fundo = rm.texturaFundo;
            break;
        case 4:
        case 5:
        case 6:
            fundo = rm.texturaFundoNeve;
            break;
        case 7:
        case 8:
        case 9:
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
    int maxX = calcularLarguraMapa(gw->mapa) - GetRenderWidth() / 2 - 100;
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
            case 2:
            case 3:
                StopMusicStream(rm.musicaFase1);
                break;
            case 4:
            case 5:
            case 6:
                StopMusicStream(rm.musicaFase2);
                break;
            case 7:
            case 8:
            case 9:
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

static void reiniciarFase(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;

    int vidaAtual = j->vidas;
    int moedaAtual = j->moedas;
    int faseAtual = gw->faseAtual;

    destruirMapa(gw->mapa);

    char caminhoMapa[100];
    sprintf(caminhoMapa, "resources/mapas/fase%d.txt", faseAtual);
    gw->mapa = carregarMapa(caminhoMapa, gw);

    gw->gravidade = 600;
    gw->timerJogo = 180000;
    gw->faseAtual = faseAtual;
    gw->estado = ESTADO_JOGO_GAMEPLAY;
    
    gw->mapa->jogador->vidas = vidaAtual;
    gw->mapa->jogador->moedas = moedaAtual;
    
}

static void salvarCheckpoint(GameWorld *gw) {

    if(gw->mapa == NULL || gw->mapa->jogador == NULL) {
        return;
    }

    if(gw->faseAtual == 3 || gw->faseAtual == 6) {
        gw->checkpointFase = gw->faseAtual;
        gw->checkpointVidas = gw->mapa->jogador->vidas;
        gw->checkpointMoedas = gw->mapa->jogador->moedas;
        gw->checkpointAtivo = true;
    }

}

static void voltarParaCheckpoint(GameWorld *gw) {

    if(!gw->checkpointAtivo) {
        return;
    }

    if(gw->mapa != NULL) {
        destruirMapa(gw->mapa);
        gw->mapa = NULL;
    }

    StopMusicStream(rm.musicaFase1);
    StopMusicStream(rm.musicaFase2);
    StopMusicStream(rm.musicaFase3);

    gw->mapaMundo->fases[gw->checkpointFase].finalizado = false;
    gw->mapaMundo->fases[gw->checkpointFase + 1].finalizado = false;
    gw->mapaMundo->fases[gw->checkpointFase + 1].liberado = false;
    gw->mapaMundo->fases[gw->checkpointFase + 2].finalizado = false;
    gw->mapaMundo->fases[gw->checkpointFase + 2].liberado = false;

    gw->mapaMundo->jogador.x = gw->mapaMundo->fases[gw->checkpointFase].pos.x;
    gw->mapaMundo->jogador.y = gw->mapaMundo->fases[gw->checkpointFase].pos.y;
    gw->mapaMundo->faseAtual = gw->checkpointFase;

    gw->vidasSalvas = gw->checkpointVidas;
    gw->moedasSalvas = gw->checkpointMoedas;
    iniciarTransicao(gw, ESTADO_JOGO_MAPA_MUNDO);

}

static void inicializarGW(GameWorld *gw) {

    if (gw->mapaMundo != NULL) {
        destruirMapaMundo(gw->mapaMundo);
    }

    gw->fecharJogo = false;
    gw->faseAtual = 1;
    gw->personagemAtual = 0;
    gw->mapaMundo = criarMapaMundo(9);
    gw->gravidade = 600;
    gw->timerJogo = 180000;
    gw->alphaTransicao = 0;
    gw->timerMorte = 0;
    gw->mapa = NULL;
    gw->estado = ESTADO_JOGO_APRESENTACAO;
    gw->proximoEstado = ESTADO_JOGO_APRESENTACAO;
    gw->estadoAnterior = ESTADO_JOGO_APRESENTACAO;
    gw->vidasSalvas = 5;
    gw->moedasSalvas = 0;
    gw->checkpointFase = 0;
    gw->checkpointVidas = 5;
    gw->checkpointMoedas = 0;
    gw->checkpointAtivo = false;

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

    if(gw->faseAtual > 9) {
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

    if(estadoAntesPause != ESTADO_JOGO_MAPA_MUNDO && gw->estado != ESTADO_JOGO_MAPA_MUNDO) {

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
