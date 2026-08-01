#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "MapaMundo.h"
#include "Mapa.h"
#include "ResourceManager.h"
#include "Utils.h"
MapaMundo *criarMapaMundo(int quantidadeFases) {

    MapaMundo *novoMapaMundo = (MapaMundo*) malloc( sizeof(MapaMundo));
    novoMapaMundo->quantidadeFases = quantidadeFases;
    novoMapaMundo->fases = (NodeMapa*) malloc(sizeof(NodeMapa) * novoMapaMundo->quantidadeFases);

    novoMapaMundo->fases[0] = (NodeMapa) { 
        (Vector2) {137,467}, 
        true, 
        false, 
        1
    };

    novoMapaMundo->fases[1] = (NodeMapa) { 
        (Vector2) {266, 466}, 
        false, 
        false, 
        2 
    };
    
    novoMapaMundo->fases[2] = (NodeMapa) {
        (Vector2) {425, 466},
        false,
        false,
        3

    };
    novoMapaMundo->fases[3] = (NodeMapa) { 
        (Vector2) {645, 507}, 
        false, 
        false, 
        4
    };

    novoMapaMundo->fases[4] = (NodeMapa) { 
        (Vector2) {802, 507}, 
        false, 
        false, 
        5 
    };
    
    novoMapaMundo->fases[5] = (NodeMapa) {
        (Vector2) {965, 507},
        false,
        false,
        6

    };
    novoMapaMundo->fases[6] = (NodeMapa) { 
        (Vector2) {1202, 507}, 
        false, 
        false, 
        7
    };

    novoMapaMundo->fases[7] = (NodeMapa) { 
        (Vector2) {1360, 507}, 
        false, 
        false, 
        8 
    };
    
    novoMapaMundo->fases[8] = (NodeMapa) {
        (Vector2) {1510, 507},
        false,
        false,
        9

    };

    novoMapaMundo->faseAtual = 0;

    novoMapaMundo->jogador = (Rectangle) {
        novoMapaMundo->fases[0].pos.x, 
        novoMapaMundo->fases[0].pos.y, 
        48, 
        48
    };

    return novoMapaMundo;
}

void desenharMapaMundo(MapaMundo *mapaMundo) {

    TransformacaoTela t = calcularTransformacaoCover(rm.texturaMapaMundo);

    Rectangle origem = {0, 0, (float) rm.texturaMapaMundo.width, (float) rm.texturaMapaMundo.height};
    Rectangle destino = {t.offsetX, t.offsetY, (float) rm.texturaMapaMundo.width * t.escala, (float) rm.texturaMapaMundo.height * t.escala };
    
    DrawTexturePro(rm.texturaMapaMundo, origem, destino, (Vector2) {0}, 0.0f, WHITE);

    Vector2 posJogadorTela = transformarPonto((Vector2){mapaMundo->jogador.x, mapaMundo->jogador.y}, t);
    DrawTexture(rm.texturaJogadorMapa, posJogadorTela.x, posJogadorTela.y, WHITE);
}

void destruirMapaMundo(MapaMundo *mapaMundo) {

    if(mapaMundo != NULL) {
        free(mapaMundo->fases);
        free(mapaMundo);
    }
}

void atualizarMapaMundo(GameWorld *gw, float delta) {


    MapaMundo *mapaMundo = gw->mapaMundo;

    if (gw->mapa != NULL && gw->mapa->faseCompleta) {
        int idxFase = gw->faseAtual - 1; 
        mapaMundo->fases[idxFase].finalizado = true;
    }

    for(int i = 0; i < mapaMundo->quantidadeFases - 1; i++) {
        if(mapaMundo->fases[i].finalizado) {
            mapaMundo->fases[i+1].liberado = true;
        }
    }
    
    //Andar entre fases
    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {

        int proximaFase = mapaMundo->faseAtual + 1;

        if(proximaFase < mapaMundo->quantidadeFases && mapaMundo->fases[proximaFase].liberado) {
            PlaySound(rm.somMapaMover);
            mapaMundo->faseAtual = proximaFase;
            mapaMundo->jogador.x = mapaMundo->fases[proximaFase].pos.x;
            mapaMundo->jogador.y = mapaMundo->fases[proximaFase].pos.y;
        }
    
    } else if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
    
        int faseAnterior = mapaMundo->faseAtual - 1;

        if(faseAnterior >= 0) {
            PlaySound(rm.somMapaMover);
            mapaMundo->faseAtual = faseAnterior;
            mapaMundo->jogador.x = mapaMundo->fases[faseAnterior].pos.x;
            mapaMundo->jogador.y = mapaMundo->fases[faseAnterior].pos.y;
        }

    }

    //Entrar numa fase
    if(IsKeyPressed(KEY_ENTER)) {
        PlaySound(rm.somBotao);
        StopMusicStream(rm.musicaMundo);
        gw->musicaMundoIniciada = false;
        gw->faseAtual = mapaMundo->fases[mapaMundo->faseAtual].fase;

        mapaMundo->fases[mapaMundo->faseAtual].finalizado = false;

        char caminhoMapa[100];
        sprintf(caminhoMapa, "resources/mapas/fase%d.txt", gw->faseAtual);
        gw->mapa = carregarMapa(caminhoMapa, gw);

        gw->mapa->jogador->vidas = gw->vidasSalvas;
        gw->mapa->jogador->moedas = gw->moedasSalvas;
        gw->timerJogo = 180000;

        gw->proximoEstado = ESTADO_JOGO_GAMEPLAY;
        gw->estado = ESTADO_JOGO_FADE_OUT;
    }
    

}