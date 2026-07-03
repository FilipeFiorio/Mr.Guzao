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

            desenharTextoContornado(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;
        
        case DIREITA:

            posicaoTexto = (int) (tamanhoTela - tamanhoTexto.x - tamanhoTela * 0.03f);

            desenharTextoContornado(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;

        case ESQUERDA:

            posicaoTexto = (int) (tamanhoTela * 0.03f);

            desenharTextoContornado(texto, posicaoTexto, y, tamanhoFonte, cor);

            break;
    
        default:
            TraceLog(LOG_INFO, "ERRO AO ESCREVER TEXTO ALINHADO");
            break;
    }

}

void desenharTextoContornado(const char *texto, int x, int y, int tamanhoFonte, Color cor) {
    DrawTextEx(rm.fonte, texto,(Vector2) {x + 1, y + 1}, tamanhoFonte, 0.0f, BLACK);
    DrawTextEx(rm.fonte,texto, (Vector2){x, y}, tamanhoFonte, 0.0f, cor);
}

void desenharCoracao(Vector2 pos, float tamanho, Color cor) {

    float r = tamanho * 0.28f;

    DrawCircleV((Vector2){ pos.x + r, pos.y + r }, r, cor);
    DrawCircleV((Vector2){ pos.x + tamanho - r, pos.y + r }, r, cor);

    Vector2 p1 = { pos.x, pos.y + r * 0.9f };
    Vector2 p2 = { pos.x + tamanho, pos.y + r * 0.9f };
    Vector2 p3 = { pos.x + tamanho / 2, pos.y + tamanho };
    DrawTriangle(p3, p2, p1, cor);

}

void desenharIconeMoeda(Vector2 pos, float raio) {
    Vector2 centro = { pos.x + raio, pos.y + raio };
    DrawCircleV((Vector2){ centro.x + 1, centro.y + 2 }, raio, (Color){ 0, 0, 0, 90 });
    DrawCircleV(centro, raio, (Color){ 255, 215, 0, 255 });
    DrawCircleLines(centro.x, centro.y, raio, (Color){ 150, 110, 0, 255 });
    DrawCircleV((Vector2){ centro.x - raio * 0.3f, centro.y - raio * 0.3f }, raio * 0.3f, (Color){ 255, 255, 255, 130 });
}
