#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoGelo.h"


ObstaculoGelo *criarObstaculoGelo(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura) {

    ObstaculoGelo *novoObstaculoGelo = (ObstaculoGelo*) malloc(sizeof(ObstaculoGelo));

    novoObstaculoGelo->ret = ret;
    novoObstaculoGelo->fonte = fonte;
    novoObstaculoGelo->cor = cor;
    novoObstaculoGelo->textura = textura;
    novoObstaculoGelo->fonte = fonte;

    return novoObstaculoGelo;

}

void destruirObstaculoGelo(ObstaculoGelo *obstaculoGelo) {
    
    if(obstaculoGelo != NULL) {
        free(obstaculoGelo);
    }

}

void desenharObstaculoGelo(ObstaculoGelo *obstaculoGelo) {

    if(obstaculoGelo->textura == NULL) {
        DrawRectangleRec(obstaculoGelo->ret, obstaculoGelo->cor);
        return;
    };

    DrawTexturePro(
        *obstaculoGelo->textura,
        obstaculoGelo->fonte,
        obstaculoGelo->ret,
        (Vector2) {0},
        0.0f,
        WHITE
    );

}