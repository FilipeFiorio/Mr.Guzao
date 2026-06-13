#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoAcelerado.h"


ObstaculoAcelerado *criarObstaculoAcelerado(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura) {

    ObstaculoAcelerado *novoObstaculoAcelerado = (ObstaculoAcelerado*) malloc(sizeof(ObstaculoAcelerado));

    novoObstaculoAcelerado->ret = ret;
    novoObstaculoAcelerado->fonte = fonte;
    novoObstaculoAcelerado->cor = cor;
    novoObstaculoAcelerado->textura = textura;
    novoObstaculoAcelerado->fonte = fonte;

    return novoObstaculoAcelerado;

}

void destruirObstaculoAcelerado(ObstaculoAcelerado *obstaculoAcelerado) {
    
    if(obstaculoAcelerado != NULL) {
        free(obstaculoAcelerado);
    }

}

void desenharObstaculoAcelerado(ObstaculoAcelerado *obstaculoAcelerado) {

    if(obstaculoAcelerado->textura == NULL) {
        DrawRectangleRec(obstaculoAcelerado->ret, obstaculoAcelerado->cor);
        return;
    };

    DrawTexturePro(
        *obstaculoAcelerado->textura,
        obstaculoAcelerado->fonte,
        obstaculoAcelerado->ret,
        (Vector2) {0},
        0.0f,
        WHITE
    );

}