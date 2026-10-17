# CInType

Jogo de defesa por digitação, desenvolvido em **C** com a biblioteca gráfica **raylib**. Digite as palavras associadas às naves inimigas para disparar, sobreviver às ondas e acumular pontos.

## Compilar e executar

Requer um compilador **C99**, `make` e a biblioteca raylib (com cabeçalhos e arquivos de linkagem).

### Linux

Instale raylib e pkg-config para a sua distribuição. Na raiz do projeto:

```sh
make
./cintype
```

### Windows (w64devkit / MinGW)

Instale o kit raylib, preferencialmente no caminho `C:/raylib/raylib`, e use um terminal com GCC e `mingw32-make`:

```sh
mingw32-make
cintype.exe
```

Se estiver instalado em outro diretório, configure `RAYLIB_PATH` ao compilar.

O executável deve ser iniciado na **raiz do repositório** para que encontre os arquivos de `sprites/`, `sounds/`, `txt_files/` e `COUR.TTF`. Arquivos de configuração são gravados em `txt_files/settings.txt`.

## Controles

- **Teclado:** digite as letras das palavras dos inimigos para atirar.
- **Backspace:** cancela a seleção do inimigo atual.
- **Enter:** usa o próximo power-up, se disponível.
- **0:** pausa o jogo.
- **Mouse:** menus e ajuste de volumes.

## Organização

- `main.c`: inicialização, estado compartilhado e ciclo principal
- `game.h`: tipos e interfaces públicas
- `entities/`: nave, projéteis, inimigos e power-ups
- `mecanics/`: pontuação, vidas e digitação
- `screens/`: telas e fluxo de navegação
- `sprites/`, `sounds/`, `txt_files/`: recursos do jogo

## Desenvolvimento

```sh
make BUILD_MODE=DEBUG
make clean
python3 -m unittest discover -s tests -v
```

As verificações automatizadas incluem testes estáticos e uma compilação em CI. Testes de jogabilidade e de áudio também devem ser feitos manualmente.

## Equipe original

- Alisson Vinicius Arruda de Paula
- Diego Juan
- Ian Sancho Monteiro
- Luis Eduardo Lustosa Brito
- Miguel Câmara Raposo Andrade
- Pedro Inácio Alves dos Santos
- Thiago Tenório de Albuquerque

Este repositório continua o trabalho do [ProjetoPI original](https://github.com/Tenorio05/ProjetoPI). Os direitos sobre código, mídias, fontes e sons seguem pertencendo aos respectivos autores; antes de adotar uma licença de distribuição para o conjunto, confirme as permissões de cada recurso.
