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

    // Guarda as coordenadas ORIGINAIS (relativas à imagem), sem transformar aqui
    Vector2 posFase1 = (Vector2) {282, 507};
    Vector2 posFase2 = (Vector2) {838, 525};
    Vector2 posFase3 = (Vector2) {1370, 511};

    novoMapaMundo->fases[0] = (NodeMapa) { 
        posFase1, 
        true, 
        false, 1
    };

    novoMapaMundo->fases[1] = (NodeMapa) { 
        posFase2, 
        false, 
        false, 
        2 
    };
    
    novoMapaMundo->fases[2] = (NodeMapa) {
        posFase3,
        false,
        false,
        3

    };

    novoMapaMundo->faseAtual = 0;

    novoMapaMundo->jogador = (Rectangle) {
        posFase1.x, 
        posFase1.y, 
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
        gw->faseAtual = mapaMundo->fases[mapaMundo->faseAtual].fase;

        mapaMundo->fases[mapaMundo->faseAtual].finalizado = false;

        char caminhoMapa[100];
        sprintf(caminhoMapa, "resources/mapas/fase%d.txt", gw->faseAtual);
        gw->mapa = carregarMapa(caminhoMapa);

        gw->mapa->jogador->vidas = gw->vidasSalvas;
        gw->mapa->jogador->moedas = gw->moedasSalvas;
        gw->timerJogo = 180000;

        gw->proximoEstado = ESTADO_JOGO_GAMEPLAY;
        gw->estado = ESTADO_JOGO_FADE_OUT;
    }
    

}