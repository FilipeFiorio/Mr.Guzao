# Mr. Guzão

Jogo de plataforma 2D desenvolvido em **C** utilizando a biblioteca **[raylib](https://www.raylib.com/)**.

![Linguagem](https://img.shields.io/badge/linguagem-C-blue)
![Raylib](https://img.shields.io/badge/engine-raylib-orange)

## 📖 Sobre o projeto

**Mr. Guzão** é um jogo de plataforma onde o jogador controla o personagem principal através de fases repletas de desafios, inimigos e obstáculos. O projeto foi construído do zero em C puro, com a raylib cuidando de renderização, input e áudio.

## 🎮 Funcionalidades

- Movimentação e física de plataforma (pulo, colisões, gravidade)
- Sistema de HUD (vidas, moedas, temporizador)
- Fases com obstáculos e inimigos
- Suporte multiplataforma (Linux e Windows)

## 🛠️ Tecnologias

- **Linguagem:** C
- **Engine/Biblioteca gráfica:** [raylib](https://www.raylib.com/)
- **Build:** Makefile (Linux) e scripts `.bat`/`.ps1` (Windows)

## 📂 Estrutura do repositório

```
Mr.Guzao/
├── lib/           # Bibliotecas utilizadas pelo projeto (raylib, etc.)
├── resources/      # Assets do jogo (sprites, sons, fontes, mapas)
├── src/            # Código-fonte do jogo
├── Makefile        # Build no Linux
├── build.sh        # Script de build no Linux
├── build.bat       # Script de build no Windows (cmd)
├── build.ps1       # Script de build no Windows (PowerShell)
└── README.md
```

## 🚀 Como compilar e executar

### Linux

Pré-requisitos: `gcc`, `make` e as dependências de sistema da raylib (X11, OpenGL, ALSA etc.).

```bash
git clone https://github.com/FilipeFiorio/Mr.Guzao.git
cd Mr.Guzao
make
./Mr.Guzao   # ou o binário gerado pelo Makefile
```

Ou, usando o script de build:

```bash
./build.sh
```

### Windows

Pré-requisitos: [MSYS2](https://www.msys2.org/) com o toolchain MinGW64 (`gcc`) instalado e configurado no `PATH`.

Via PowerShell:

```powershell
.\build.ps1
```

Via `cmd`:

```bat
build.bat
```

O executável e os recursos necessários serão gerados/empacotados na pasta de saída definida pelo script.

## 📦 Releases

Você também pode baixar a versão já compilada diretamente na aba [Releases](https://github.com/FilipeFiorio/Mr.Guzao/releases) do repositório. Para rodas apenas execute o o jogo dentro do diretório com os resources.

## 🎯 Controles

| Ação        | Tecla         |
|-------------|---------------|
| Mover       | Setas / A D   |
| Pular       | Espaço        |
| Correr      | Shift esq.    |
| Pausar      | Esc           |
| Voltar      | BackSpace     |
| Selecionar  | Enter         |

## 👤 Autores

**Filipe Fiorio** — [@FilipeFiorio](https://github.com/FilipeFiorio) & **Gustavo Mastelaro** — [@gustavoMastelaro](https://github.com/gustavoMastelaro) 
