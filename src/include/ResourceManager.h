/**
 * @file ResourceManager.h
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager struct and function declarations.
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

typedef struct ResourceManager {
    Texture2D texturaTerreno;
    Texture2D texturaFundo;
    Texture2D texturaFundoNeve;
    Texture2D texturaFundoDeserto;
    Texture2D texturaInicio;
    Texture2D texturaMapaMundo;
    Texture2D texturaInimigoNormal;
    Texture2D texturaInimigoDash;
    Texture2D texturaInimigoVoador;
    Texture2D texturaInimigoEspinho;
    Texture2D texturaInimigoPedra;
    Texture2D texturaInimigoPlanta; 
    Texture2D texturaInimigoPlantaGelo; 
    Texture2D texturaInimigoFantasma;
    Texture2D texturaItens;
    Texture2D texturaTiro;
    Texture2D texturaInimigoMorrendo;
    Texture2D texturaJogadorMapa;
    Texture2D texturaJogador;

    Font fonte;

    Sound somPulo;
    Sound somMorte;
    Sound somChegada;
    Sound somVitoria;
    Sound somMoeda;
    Sound somMatar;
    Sound somTiro;
    Sound somPausa;
    Sound somBotao;
    Sound somGameOver;
    Sound somMapaMover;
    Sound somVida;
    Sound somHurryUp;
    Sound somBatida;

    Music musicaInicio;
    Music musicaFase1;
    Music musicaFase2;
    Music musicaFase3;
    Music musicaMundo;
} ResourceManager;

/**
 * @brief Global ResourceManager instance.
 */
extern ResourceManager rm;

/**
 * @brief Load global game resources, linking them in the global instance of
 * ResourceManager called rm.
 */
void loadResourcesResourceManager( void );

/**
 * @brief Unload global game resources.
 */
void unloadResourcesResourceManager( void );