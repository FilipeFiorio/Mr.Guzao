/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"
#include "raylib/raylib.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaTerreno = LoadTexture( "resources/images/tiles/terreno.png" );
    rm.texturaFundo = LoadTexture("resources/images/fundos/bg.png");
    rm.texturaFundoNeve = LoadTexture("resources/images/fundos/bg-snow.png");
    rm.texturaFundoDeserto = LoadTexture("resources/images/fundos/bg-desert.png");
    rm.texturaInicio = LoadTexture("resources/images/fundos/inicio.png");
    rm.texturaMapaMundo = LoadTexture("resources/images/fundos/mapaMundo.png");
    
    rm.texturaInimigoNormal = LoadTexture("resources/images/sprites/inimigoNormal.png");
    rm.texturaInimigoDash = LoadTexture("resources/images/sprites/inimigoDash.png");
    rm.texturaInimigoVoador = LoadTexture("resources/images/sprites/inimigoVoador.png");
    rm.texturaInimigoEspinho = LoadTexture("resources/images/sprites/InimigoEspinho.png");
    rm.texturaInimigoFantasma = LoadTexture("resources/images/sprites/InimigoFantasma.png");
    rm.texturaInimigoPedra = LoadTexture("resources/images/sprites/InimigoPedra.png");
    rm.texturaInimigoPlanta = LoadTexture("resources/images/sprites/InimigoPlanta.png");
    rm.texturaInimigoPlantaGelo = LoadTexture("resources/images/sprites/InimigoPlantaGelo.png");
    rm.texturaInimigoMorrendo = LoadTexture("resources/images/sprites/morteInimigo.png");
    rm.texturaTiro = LoadTexture("resources/images/sprites/Tiro.png");

    rm.texturaItens = LoadTexture("resources/images/sprites/itens.png");

    rm.texturaJogador = LoadTexture("resources/images/sprites/jogador.png");
    rm.texturaJogadorMapa = LoadTexture("resources/images/sprites/jogadorMapa.png");

    rm.fonte = LoadFontEx("resources/fontes/PressStart2P-Regular.ttf", 32, 0, 250);

    rm.somMatar = LoadSound("resources/sfx/spin-stomp.wav");
    rm.somMorte = LoadSound("resources/sfx/death.wav");
    rm.somMoeda = LoadSound("resources/sfx/Coin.wav");
    rm.somPulo = LoadSound("resources/sfx/jump.wav");
    rm.somPausa = LoadSound("resources/sfx/Pause.wav");
    rm.somTiro = LoadSound("resources/sfx/flames.wav");
    rm.somBotao = LoadSound("resources/sfx/botao.wav");
    rm.somGameOver = LoadSound("resources/sfx/gameOver.wav");
    rm.somMapaMover = LoadSound("resources/sfx/mapMove.wav");
    rm.somVida = LoadSound("resources/sfx/vida.wav");
    rm.somChegada = LoadSound("resources/sfx/chegada.wav");
    rm.somVitoria = LoadSound("resources/sfx/vitoria.wav");
    rm.somHurryUp = LoadSound("resources/sfx/hurry-up.wav");
    rm.somBatida = LoadSound("resources/sfx/batida.wav");
    
    rm.musicaFase1 = LoadMusicStream("resources/musics/musica-fase1.mp3");
    rm.musicaFase2 = LoadMusicStream("resources/musics/musica-fase2.mp3");
    rm.musicaFase3 = LoadMusicStream("resources/musics/musica-fase3.mp3");
    rm.musicaMundo = LoadMusicStream("resources/musics/musica-mapa-mundo.mp3");
    rm.musicaInicio = LoadMusicStream("resources/musics/musica-tela-inicial.mp3");

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture(rm.texturaTerreno );
    UnloadTexture(rm.texturaFundo );
    UnloadTexture(rm.texturaFundoNeve );
    UnloadTexture(rm.texturaFundoDeserto );
    UnloadTexture(rm.texturaInicio);
    UnloadTexture(rm.texturaMapaMundo);

    SetTextureFilter(rm.texturaTerreno, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInicio, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaMapaMundo, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaFundo, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaFundoDeserto, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaFundoNeve, TEXTURE_FILTER_POINT);

    UnloadTexture(rm.texturaInimigoNormal);
    UnloadTexture(rm.texturaInimigoDash);
    UnloadTexture(rm.texturaInimigoVoador);
    UnloadTexture(rm.texturaInimigoEspinho);
    UnloadTexture(rm.texturaInimigoFantasma);
    UnloadTexture(rm.texturaInimigoPedra);
    UnloadTexture(rm.texturaInimigoPlanta);
    UnloadTexture(rm.texturaInimigoPlantaGelo);
    UnloadTexture(rm.texturaTiro);
    UnloadTexture(rm.texturaInimigoMorrendo);

    SetTextureFilter(rm.texturaInimigoNormal, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoDash, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoVoador, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoFantasma, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoPlantaGelo, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoPlanta, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoPedra, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoEspinho, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaTiro, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaInimigoMorrendo, TEXTURE_FILTER_POINT);
    
    UnloadTexture(rm.texturaJogadorMapa);
    UnloadTexture(rm.texturaItens);

    SetTextureFilter(rm.texturaItens, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaJogadorMapa, TEXTURE_FILTER_POINT);
    
    UnloadTexture(rm.texturaJogador);

    SetTextureFilter(rm.texturaJogador, TEXTURE_FILTER_POINT);

    UnloadFont(rm.fonte);

    UnloadSound(rm.somMatar);
    UnloadSound(rm.somMoeda);
    UnloadSound(rm.somMorte);
    UnloadSound(rm.somPulo);
    UnloadSound(rm.somPausa);
    UnloadSound(rm.somTiro);
    UnloadSound(rm.somVida);
    UnloadSound(rm.somBotao);
    UnloadSound(rm.somGameOver);
    UnloadSound(rm.somMapaMover);
    UnloadSound(rm.somChegada);
    UnloadSound(rm.somVitoria);
    UnloadSound(rm.somHurryUp);
    UnloadSound(rm.somBatida);

    UnloadMusicStream(rm.musicaFase1);
    UnloadMusicStream(rm.musicaFase2);
    UnloadMusicStream(rm.musicaFase3);
    UnloadMusicStream(rm.musicaInicio);
    UnloadMusicStream(rm.musicaMundo);

}