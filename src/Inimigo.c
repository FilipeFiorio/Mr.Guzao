#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Inimigo.h"

static void resolverColisaoMapaX(Inimigo *inimigo, Mapa *m);

Inimigo *criarInimigo(float x, float y, float largura, float altura, Color cor) {

    Inimigo *novoInimigo = (Inimigo*) malloc(sizeof(Inimigo));

    novoInimigo->ret.x = x;
    novoInimigo->ret.y = y;
    novoInimigo->ret.width = largura;
    novoInimigo->ret.height = altura;

    novoInimigo->velX = 150;
    
    novoInimigo->cor = cor;

    novoInimigo->estaVivo = true;

    return novoInimigo;

}

void atualizarInimigo(Inimigo *inimigo, GameWorld *gw, float delta) {

    inimigo->ret.x += inimigo->velX * delta;
    resolverColisaoMapaX(inimigo, gw->mapa);

}

void destruirInimigo(Inimigo *inimigo) {

    if(inimigo != NULL) {
        free(inimigo);
        inimigo = NULL;
    }
    
}

void desenharInimigo(Inimigo *inimigo) {

    DrawRectangleRec(inimigo->ret, inimigo->cor);

}

static void resolverColisaoMapaX(Inimigo *i, Mapa *m) {

    ElementoMapa *el = m->elementos;

    while(el != NULL) {

        Obstaculo *o = &el->obstaculo;

        if(CheckCollisionRecs(i->ret, o->ret)) {
            if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                i->ret.x = o->ret.x - i->ret.width;
            } else {
                i->ret.x = o->ret.x + o->ret.width;
            }
                
            i->velX = -i->velX;
            
        }
        

        el = el->proximo;

    }


}
