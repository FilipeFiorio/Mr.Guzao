#include "raylib/raylib.h"
#include "ResourceManager.h"

#include "Utils.h"

void drawTextAlinhado(const char *texto, int y, int tamanhoFonte, Color cor, int posicao) {
    
    Vector2 tamanhoTexto = MeasureTextEx(rm.fonte, texto, tamanhoFonte, 0);
    int tamanhoTela = GetScreenWidth();
    int posicaoTexto = 0;
    
    switch (posicao) {
        case CENTRO:
        
            posicaoTexto = (tamanhoTela - tamanhoTexto.x) / 2;

            DrawTextEx(rm.fonte, texto, (Vector2) {posicaoTexto, y}, tamanhoFonte, 0, cor);

            break;
        
        case DIREITA:

            posicaoTexto = (int) (tamanhoTela - tamanhoTexto.x - tamanhoTela * 0.03f);

            DrawTextEx(rm.fonte, texto, (Vector2) {posicaoTexto, y}, tamanhoFonte, 0, cor);

            break;

        case ESQUERDA:

            posicaoTexto = (int) (tamanhoTela * 0.03f);

            DrawTextEx(rm.fonte, texto, (Vector2) {posicaoTexto, y}, tamanhoFonte, 0, cor);

            break;
    
        default:
            TraceLog(LOG_INFO, "ERRO AO ESCREVER TEXTO ALINHADO");
            break;
    }


}