
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHAS 13
#define MAX_COLUNAS 20

const char *nivel1[] = {
    "#############",
    "#P  #C  #B A#",
    "### # # # # #",
    "#B     #   ##",
    "# #######  ##",
    "#  A    #C  #",
    "### # # ### #",
    "#C   #A    ##",
    "# ####### # #",
    "#   B   A  S#",
    "#############"
};

const char *nivel2[] = {
    "###############",
    "#P   A #C   B #",
    "##### # ##### #",
    "#B       #A   #",
    "# ###### # ####",
    "# #C A       C#",
    "# # ####### # #",
    "#   #B  A   # #",
    "### # #########",
    "#A    #C   A  #",
    "# ###### #####",
    "#   B   #A   S#",
    "###############"
};

void limparTela() {
    printf("\033[2J\033[H");
}

void escolherPersonagem(char nome[], char icone[]) {

    int opcao;

    printf("=== FNAF: ESCAPE DA PIZZARIA ===\n");
    printf("1 - Freddy Fazbear\n");
    printf("2 - Bonnie\n");
    printf("3 - Chica\n");
    printf("4 - Foxy\n");
    printf("5 - Guarda Noturno\n");

    do {
        printf("\nEscolha seu personagem (1-5): ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                strcpy(nome, "Freddy Fazbear");
                strcpy(icone, "F");
                break;
            case 2:
                strcpy(nome, "Bonnie");
                strcpy(icone, "B");
                break;
            case 3:
                strcpy(nome, "Chica");
                strcpy(icone, "C");
                break;
            case 4:
                strcpy(nome, "Foxy");
                strcpy(icone, "X");
                break;
            case 5:
                strcpy(nome, "Guarda Noturno");
                strcpy(icone, "G");
                break;
            default:
                printf("Opcao invalida! Escolha de 1 a 5.\n");
        }

    } while (opcao < 1 || opcao > 5);
}

void mostrarMapa(char mapa[MAX_LINHAS][MAX_COLUNAS],
                 int linhas, int nivel,
                 char nome[], char icone[],
                 int px, int py, int forca,
                 int bateria, int lanterna,
                 int movimentos, char mensagem[]) {

    limparTela();

    int alcance = lanterna ? 2 : 1;

    printf("====================================\n");
    printf("FNAF: LABIRINTO SOMBRIO - NOITE %d\n", nivel);
    printf("====================================\n");

    printf("Personagem: %s [%s]\n", nome, icone);
    printf("Movimentos: %d\n", movimentos);
    printf("Forca: %d%%\n", forca);
    printf("Bateria: %d%%\n", bateria);
    printf("Lanterna: %s\n", lanterna ? "LIGADA" : "DESLIGADA");

    printf("\nLegenda:\n");
    printf("P = Jogador | # = Parede | C = Pizza\n");
    printf("B = Pilha | A = Animatronico | S = Saida\n\n");

    for (int l = 0; l < linhas; l++) {
        int colunas = strlen(mapa[l]);

        for (int c = 0; c < colunas; c++) {

            if (l >= py - alcance && l <= py + alcance &&
                c >= px - alcance && c <= px + alcance) {

                printf("%c ", mapa[l][c]);

            } else {
                printf("? ");
            }
        }

        printf("\n");
    }

    printf("\nW/A/S/D = Mover\n");
    printf("F = Lanterna | Q = Sair\n");

    if (strlen(mensagem) > 0) {
        printf("\nAVISO: %s\n", mensagem);
    }
}

int jogarNivel(int numero, char nome[], char icone[]) {

    const char **original;
    int linhas;

    if (numero == 1) {
        original = nivel1;
        linhas = 11;
    } else {
        original = nivel2;
        linhas = 13;
    }

    char mapa[MAX_LINHAS][MAX_COLUNAS] = {{0}};

    int inicioX = 0;
    int inicioY = 0;

    for (int l = 0; l < linhas; l++) {
        strcpy(mapa[l], original[l]);

        for (int c = 0; c < strlen(mapa[l]); c++) {
            if (mapa[l][c] == 'P') {
                inicioX = c;
                inicioY = l;
            }
        }
    }

    int px = inicioX;
    int py = inicioY;

    int forca = 100;
    int bateria = 100;
    int movimentos = 0;
    int lanterna = 1;

    char mensagem[180] = "Encontre pizzas e pilhas para sobreviver!";

    while (1) {

        if (forca <= 0) {

            mostrarMapa(mapa, linhas, numero, nome, icone,
                        px, py, 0, bateria, lanterna,
                        movimentos, "");

            printf("\nGAME OVER! Voce ficou sem forca!\n");
            return -1;
        }

        mostrarMapa(mapa, linhas, numero, nome, icone,
                    px, py, forca, bateria, lanterna,
                    movimentos, mensagem);

        mensagem[0] = '\0';

        char comando;

        printf("\nSua acao: ");
        scanf(" %c", &comando);

        comando = (char)toupper((unsigned char)comando);

        if (comando == 'Q') {
            return -1;
        }

        if (comando == 'F') {

            if (!lanterna && bateria <= 0) {
                strcpy(mensagem, "Sem bateria! Encontre uma pilha.");
            } else {
                lanterna = !lanterna;

                strcpy(mensagem, lanterna ?
                       "Lanterna ligada!" : "Lanterna desligada!");
            }

            continue;
        }

        int dx = 0;
        int dy = 0;

        switch (comando) {

            case 'W':
                dy = -1;
                break;

            case 'S':
                dy = 1;
                break;

            case 'A':
                dx = -1;
                break;

            case 'D':
                dx = 1;
                break;

            default:
                strcpy(mensagem, "Tecla invalida! Use W, A, S, D, F ou Q.");
                continue;
        }

        int nx = px + dx;
        int ny = py + dy;

        if (ny < 0 || ny >= linhas ||
            nx < 0 || nx >= strlen(mapa[ny])) {

            strcpy(mensagem, "Fora dos limites do mapa!");
            continue;
        }

        char destino = mapa[ny][nx];

        if (destino == '#') {
            strcpy(mensagem, "Parede bloqueando o caminho!");
            continue;
        }

        movimentos++;
        forca -= 4;

        if (lanterna) {
            bateria -= 6;

            if (bateria <= 0) {
                bateria = 0;
                lanterna = 0;
                strcpy(mensagem, "A bateria acabou!");
            }
        }

        if (destino == 'A') {

            mapa[py][px] = ' ';
            mapa[ny][nx] = ' ';

            forca -= 25;

            px = inicioX;
            py = inicioY;

            mapa[py][px] = 'P';

            strcpy(mensagem,
                "JUMPSCARE! Um animatronico atacou! Voltou ao inicio!");

        } else if (destino == 'C') {

            forca += 35;

            if (forca > 100) {
                forca = 100;
            }

            mapa[py][px] = ' ';
            px = nx;
            py = ny;
            mapa[py][px] = 'P';

            strcpy(mensagem, "Pizza encontrada! +35% de forca!");

        } else if (destino == 'B') {

            bateria += 50;

            if (bateria > 100) {
                bateria = 100;
            }

            mapa[py][px] = ' ';
            px = nx;
            py = ny;
            mapa[py][px] = 'P';

            strcpy(mensagem, "Pilha encontrada! +50% de bateria!");

        } else if (destino == 'S') {

            printf("\nVoce sobreviveu a Noite %d!\n", numero);
            printf("Movimentos: %d\n", movimentos);

            return movimentos;

        } else {

            mapa[py][px] = ' ';
            px = nx;
            py = ny;
            mapa[py][px] = 'P';
        }
    }
}

int main() {

    char nome[40];
    char icone[5];

    escolherPersonagem(nome, icone);

    int totalMovimentos = 0;

    getchar();

    printf("\nPressione ENTER para iniciar...");
    getchar();

    for (int nivel = 1; nivel <= 2; nivel++) {

        int resultado = jogarNivel(nivel, nome, icone);

        if (resultado == -1) {
            printf("\nJOGO ENCERRADO!\n");
            return 0;
        }

        totalMovimentos += resultado;

        if (nivel < 2) {

            printf("\nVoce sobreviveu a esta noite!\n");
            printf("Pressione ENTER para proxima noite...");

            getchar();
            getchar();
        }
    }

    printf("\n====================================\n");
    printf("          VITORIA FINAL!\n");
    printf("      VOCE ESCAPOU DA PIZZARIA!\n");
    printf("Movimentos totais: %d\n", totalMovimentos);
    printf("====================================\n");

    return 0;
}
