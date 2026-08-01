#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Colisao.h"
#include "ResourceManager.h"
#include "Tipos.h"

void resolverColisaoJogadorMapaX(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    Mapa *mapa = gw->mapa;

    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    j->ret.x = o->ret.x - j->ret.width;
                } else {
                    j->ret.x = o->ret.x + o->ret.width;
                }
                j->vel.x = 0;

            }
        
        }else if (obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.width <= retSobre.height - 2) {
                    if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                        j->ret.x = o->ret.x - j->ret.width;
                    } else {
                        j->ret.x = o->ret.x + o->ret.width;
                    }
                    j->vel.x = 0;
                }
            }

        } else if (obs->tipo == OBSTACULO_CHEGADA) {

            ObstaculoChegada *o = (ObstaculoChegada*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                PlaySound(rm.somChegada);
                gw->mapa->faseCompleta = true;

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

            }

        } else if (obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    j->ret.x = o->ret.x - j->ret.width;
                } else {
                    j->ret.x = o->ret.x + o->ret.width;
                }
                j->vel.x = 0;

            }
        }  else if (obs->tipo == OBSTACULO_ACELERADO) {

            ObstaculoAcelerado *o = (ObstaculoAcelerado*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    j->ret.x = o->ret.x - j->ret.width;
                } else {
                    j->ret.x = o->ret.x + o->ret.width;
                }
                j->vel.x = 0;

            }
        } 
            
        el = el->proximo;
    }

}

void resolverColisaoJogadorMapaY(GameWorld *gw, float delta) {

    Jogador *j = gw->mapa->jogador;
    Mapa *mapa = gw->mapa;
    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                    j->ret.y = o->ret.y - j->ret.height;
                    j->noChao = true;
                } else {
                    j->ret.y = o->ret.y + o->ret.height;
                }
                j->vel.y = 0;
                
            }
            
        } else if (obs->tipo == OBSTACULO_CHEGADA) {

            ObstaculoChegada *o = (ObstaculoChegada*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                gw->mapa->faseCompleta = true;

            }

        } else if (obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                    j->ret.y = o->ret.y - j->ret.height;
                    j->noChao = true;

                    if(j->congelado) {
                        j->contadorTempoCongelado = 0;
                    } else {
                        j->congelado = true;
                    }
                        
                } else {
                    j->ret.y = o->ret.y + o->ret.height;
                }
                j->vel.y = 0;
            
            }
        } else if (obs->tipo == OBSTACULO_ACELERADO) {

            ObstaculoAcelerado *o = (ObstaculoAcelerado*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                    j->ret.y = o->ret.y - j->ret.height;
                    j->noChao = true;

                    if(j->acelerado) {
                        j->contadorTempoAcelerado = 0;
                    } else {
                        j->acelerado = true;
                    }
                    
                } else {
                    j->ret.y = o->ret.y + o->ret.height;
                }
                j->vel.y = 0;
            
            }
            
        } else if (obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.height < retSobre.width + 2) {
                    if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                        // pousou em cima -> gruda
                        j->ret.y = o->ret.y - j->ret.height;
                        j->noChao = true;
                        j->plataformaAtual = o;
                    } else {
                        // bateu por baixo
                        j->ret.y = o->ret.y + o->ret.height;
                    }
                    j->vel.y = 0;
                }
            }
        } 
        
        el = el->proximo;
    }

}

void verificarColisaoJogadorItem(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->itens;

    while(el != NULL) {

        Item *item = (Item*) el->objeto;

        if(item->tipo == ITEM_MOEDA) {

            ItemMoeda *i = (ItemMoeda*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->moedas += i->valor;
                PlaySound(rm.somMoeda);
            }

        } else if(item->tipo == ITEM_MOEDA_ESPECIAL) {

            ItemMoedaEspecial *i = (ItemMoedaEspecial*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->moedas += i->valor;
                PlaySound(rm.somMoeda);
            }
            
        } else if(item->tipo == ITEM_VIDA) {

            ItemVida *i = (ItemVida*) item->objeto;

            if(!i->ativo || i->estado == ITEM_COLETADO) {
                el = el->proximo;
                continue;
            }

            if(CheckCollisionRecs(j->ret, i->ret) && i->estado == ITEM_GIRANDO) {
                i->estado = ITEM_COLETADO;
                j->vidas++;
                PlaySound(rm.somVida);
            }

        }


        el = el->proximo;
    }

}

void verificarColisaoJogadorInimigo(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->inimigos;

    while (el != NULL) {

        Inimigo *inimigo = (Inimigo*) el->objeto;

        if (inimigo->tipo == INIMIGO_NORMAL) {

            InimigoNormal *i = (InimigoNormal*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_NORMAL_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {

                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                        i->vel = (Vector2) {0};
                        i->estado = INIMIGO_NORMAL_MORRENDO;
                        PlaySound(rm.somMatar);
                        j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                    if(j->vidas > 1 ) {
                        PlaySound(rm.somMorte);
                    }
                }
                
                return;
            }

        } else if (inimigo->tipo == INIMIGO_DASH) {

            InimigoDash *i = (InimigoDash*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_DASH_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {

                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                    i->velXInicial = 0;
                    i->estado = INIMIGO_DASH_MORRENDO;
                    PlaySound(rm.somMatar);
                    j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                    if(j->vidas > 1 ) {
                        PlaySound(rm.somMorte);
                    }
                }

                return;
            }

        } else if (inimigo->tipo == INIMIGO_VOADOR) {

            InimigoVoador *i = (InimigoVoador*) inimigo->objeto;

            if(!i->estaVivo || i->estado == INIMIGO_VOADOR_MORRENDO) {
                el = el->proximo;
                continue;
            }

            if (CheckCollisionRecs(j->ret, i->ret)) {


                if (j->vel.y > 0 && j->ret.y + j->ret.height / 2 < i->ret.y + i->ret.height / 2) {
                    i->vel = (Vector2) {0};
                    i->estado = INIMIGO_VOADOR_MORRENDO;
                    PlaySound(rm.somMatar);
                    j->vel.y = -j->vel.y * 0.75f;
                } else {
                    j->estado = JOGADOR_MORRENDO;
                    if(j->vidas > 1 ) {
                        PlaySound(rm.somMorte);
                    }
                }
               
                return;
            }


        // A partir daqui não é possivel matar inimigos entao verificamos apenas se ha colisao
        // nao importando o estado do inimigo
        } else if (inimigo->tipo == INIMIGO_ESPINHO) {

            InimigoEspinho *i = (InimigoEspinho*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;
                if(j->vidas > 1 ) {
                    PlaySound(rm.somMorte);
                }

                return;
            }

        } else if (inimigo->tipo == INIMIGO_FANTASMA) {

            InimigoFantasma *i = (InimigoFantasma*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;
                if(j->vidas > 1 ) {
                    PlaySound(rm.somMorte);
                }

                return;
            }

        } else if (inimigo->tipo == INIMIGO_PEDRA) {

            InimigoPedra *i = (InimigoPedra*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;
                if(j->vidas > 1 ) {
                   PlaySound(rm.somMorte);
                }

                return;
            }


        } else if (inimigo->tipo == INIMIGO_PLANTA) {

            InimigoPlanta *i = (InimigoPlanta*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {
                
                j->estado = JOGADOR_MORRENDO;
                if(j->vidas > 1 ) {
                    PlaySound(rm.somMorte);
                }

                return;
            }

            if(i->tiro != NULL && i->tiro->ativo && i->tiro->estado == TIRO_VIAJANDO) {
                if(CheckCollisionRecs(j->ret, i->tiro->ret)) {
                    j->estado = JOGADOR_MORRENDO;
                    if(j->vidas > 1 ) {
                        PlaySound(rm.somMorte);
                    }
                }
            }

        } else if (inimigo->tipo == INIMIGO_PLANTA_GELO) {

            InimigoPlantaGelo *i = (InimigoPlantaGelo*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret)) {

                j->estado = JOGADOR_MORRENDO;
                if(j->vidas > 1 ) {
                    PlaySound(rm.somMorte);
                }

                return;
            }

            if(i->tiro != NULL && i->tiro->ativo && i->tiro->estado == TIRO_VIAJANDO) {
                if(CheckCollisionRecs(j->ret, i->tiro->ret)) {
                    j->congelado = true;
                }
            }

        }

        el = el->proximo;
    }

}

void resolverColisaoInimigoMapaX(Rectangle *ret, Vector2 *vel , Mapa *m) {
        
    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if(CheckCollisionRecs(*ret, o->ret)) {
                if(ret->x + ret->width / 2 < o->ret.x + o->ret.width / 2) {
                    ret->x = o->ret.x - ret->width;
                } else {
                    ret->x = o->ret.x + o->ret.width;
                }
                    
                vel->x = -vel->x;
                
            }

        } else if(obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if(CheckCollisionRecs(*ret, o->ret)) {
                if(ret->x + ret->width / 2 < o->ret.x + o->ret.width / 2) {
                    ret->x = o->ret.x - ret->width;
                } else {
                    ret->x = o->ret.x + o->ret.width;
                }
                    
                vel->x = -vel->x;
                
            }
            
        }

        el = el->proximo;

    }


}

void resolverColisaoInimigoMapaY(Rectangle *ret, Vector2 *vel, bool *noChao, Mapa *m) {

    ElementoMapa *el = m->obstaculos;

    while ( el != NULL ) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if ( CheckCollisionRecs( *ret, o->ret ) ) {
                if ( ret->y + ret->height / 2 < o->ret.y + o->ret.height / 2 ) {
                    ret->y= o->ret.y - ret->height;
                    *noChao = true;
                }
                vel->y = 0;
            }

        } else if(obs->tipo == OBSTACULO_MOVEL)  {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;
            
            if ( CheckCollisionRecs( *ret, o->ret ) ) {
                if ( ret->y + ret->height / 2 < o->ret.y + o->ret.height / 2 ) {
                    ret->y = o->ret.y - ret->height;
                    *noChao = true;
                }
                vel->y = 0;
            }
        } else if(obs->tipo == OBSTACULO_GELO)  {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;
            
            if ( CheckCollisionRecs( *ret, o->ret ) ) {
                if ( ret->y + ret->height / 2 < o->ret.y + o->ret.height / 2 ) {
                    ret->y = o->ret.y - ret->height;
                    *noChao = true;
                }
                vel->y = 0;
            }
        } else if(obs->tipo == OBSTACULO_ACELERADO)  {

            ObstaculoAcelerado *o = (ObstaculoAcelerado*) obs->objeto;
            
            if ( CheckCollisionRecs( *ret, o->ret ) ) {
                if ( ret->y + ret->height / 2 < o->ret.y + o->ret.height / 2 ) {
                    ret->y = o->ret.y - ret->height;
                    *noChao = true;
                }
                vel->y = 0;
            }
        }

        el = el->proximo;

    }
}

bool verificarSeTemChao(Rectangle *retInimigo, Vector2 *velInimigo, Mapa *m) {

    Rectangle ret = {
        .x = (velInimigo->x > 0) ? (retInimigo->x + retInimigo->width) : (retInimigo->x) - 1,
        .y = retInimigo->y + retInimigo->height,
        .width = 1,
        .height = 1
    };

    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {
        
        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;
            
            if(CheckCollisionRecs(ret, o->ret)) {
                return true;
            }

        } else if(obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;
            
            if(CheckCollisionRecs(ret, o->ret)) {
                return true;
            }

        } else if(obs->tipo == OBSTACULO_ACELERADO) {

            ObstaculoAcelerado *o = (ObstaculoAcelerado*) obs->objeto;
            
            if(CheckCollisionRecs(ret, o->ret)) {
                return true;
            }

        } 
        
        el = el->proximo;
    }

    return false;

}

void resolverColisaoInimigoComInimigo(Rectangle *ret, Vector2 *vel, Mapa *m) {

    ElementoMapa *el = m->inimigos;

    while( el != NULL) {
        
        Inimigo *inimigo = (Inimigo*) el->objeto;

        if(inimigo->tipo == INIMIGO_NORMAL) {

            InimigoNormal *i =(InimigoNormal*) inimigo->objeto;

            if(CheckCollisionRecs(*ret, i->ret) && ret->x != i->ret.x && i->estaVivo) {
                vel->x = -vel->x;
            }

        } else if(inimigo->tipo == INIMIGO_DASH) {

            InimigoDash *i =(InimigoDash*) inimigo->objeto;

            if(CheckCollisionRecs(*ret, i->ret) && ret->x != i->ret.x && i->estaVivo) {
                vel->x = -vel->x;
            }

        } else if(inimigo->tipo == INIMIGO_ESPINHO) {

            InimigoEspinho *i =(InimigoEspinho*) inimigo->objeto;

            if(CheckCollisionRecs(*ret, i->ret) && ret->x != i->ret.x) {
                vel->x = -vel->x;
            }

        }

        el = el->proximo;


    }
    

}
