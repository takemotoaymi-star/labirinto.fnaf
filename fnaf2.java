
import java.util.Scanner;

public class Main {

    static Scanner entrada = new Scanner(System.in);

    static String[][] niveis = {
        {
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
        },
        {
            "###############",
            "#P   A #C   B #",
            "##### # ##### #",
            "#B       #A   #",
            "# ###### # ####",
            "# #C A       C#",
            "# # ####### # #",
            "#   #B  A    # #",
            "### # #########",
            "#A    #C   A  ##",
            "# ###### ##### #",
            "#   B   #A    S#",
            "###############"
        }
    };

    static void limparTela() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    static String[] escolherPersonagem() {

        String[][] personagens = {
            {"Freddy Fazbear", "🐻"},
            {"Bonnie", "🐰"},
            {"Chica", "🐥"},
            {"Foxy", "🦊"},
            {"Guarda Noturno", "👮"}
        };

        System.out.println("=== FNAF: ESCAPE DA PIZZARIA ===");

        for (int i = 0; i < personagens.length; i++) {
            System.out.println((i + 1) + " - " +
                personagens[i][1] + " " + personagens[i][0]);
        }

        while (true) {
            System.out.print("Escolha seu personagem (1-5): ");
            int opcao = entrada.nextInt();

            if (opcao >= 1 && opcao <= 5) {
                return personagens[opcao - 1];
            }

            System.out.println("Opcao invalida!");
        }
    }

    static void mostrarMapa(char[][] mapa, int nivel,
                            String nome, String icone,
                            int px, int py, int forca,
                            int bateria, boolean lanterna,
                            int movimentos, String mensagem) {

        limparTela();

        int alcance = lanterna ? 2 : 1;

        System.out.println("======================================");
        System.out.println("FNAF: LABIRINTO SOMBRIO - NOITE " + nivel);
        System.out.println("======================================");

        System.out.println("Personagem: " + icone + " " + nome);
        System.out.println("Movimentos: " + movimentos);
        System.out.println("Forca: " + forca + "%");
        System.out.println("Bateria: " + bateria + "%");
        System.out.println("Lanterna: " +
            (lanterna ? "LIGADA" : "DESLIGADA"));

        System.out.println("\nLegenda:");
        System.out.println("P = Jogador | # = Parede | C = Pizza");
        System.out.println("B = Pilha | A = Animatronico | S = Saida\n");

        for (int l = 0; l < mapa.length; l++) {
            for (int c = 0; c < mapa[l].length; c++) {

                if (Math.abs(l - py) <= alcance &&
                    Math.abs(c - px) <= alcance) {
                    System.out.print(mapa[l][c] + " ");
                } else {
                    System.out.print("? ");
                }
            }
            System.out.println();
        }

        System.out.println("\nW/A/S/D = Mover");
        System.out.println("F = Lanterna | Q = Sair");

        if (!mensagem.isEmpty()) {
            System.out.println("\nAVISO: " + mensagem);
        }
    }

    static int jogarNivel(int numero, String nome, String icone) {

        char[][] mapa = new char[niveis[numero].length][];

        int inicioX = 0;
        int inicioY = 0;

        for (int l = 0; l < mapa.length; l++) {
            mapa[l] = niveis[numero][l].toCharArray();

            for (int c = 0; c < mapa[l].length; c++) {
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

        boolean lanterna = true;

        String mensagem = "Encontre pizzas e pilhas para sobreviver!";

        while (true) {

            if (forca <= 0) {
                mostrarMapa(mapa, numero + 1, nome, icone,
                    px, py, 0, bateria, lanterna, movimentos, "");

                System.out.println("\nGAME OVER! Voce ficou sem forca!");
                return -1;
            }

            mostrarMapa(mapa, numero + 1, nome, icone,
                px, py, forca, bateria, lanterna, movimentos, mensagem);

            mensagem = "";

            System.out.print("\nSua acao: ");
            char comando = entrada.next().toUpperCase().charAt(0);

            if (comando == 'Q') {
                return -1;
            }

            if (comando == 'F') {

                if (!lanterna && bateria <= 0) {
                    mensagem = "Sem bateria! Encontre uma pilha.";
                } else {
                    lanterna = !lanterna;
                    mensagem = lanterna ?
                        "Lanterna ligada!" : "Lanterna desligada!";
                }

                continue;
            }

            int dx = 0;
            int dy = 0;

            switch (comando) {
                case 'W': dy = -1; break;
                case 'S': dy = 1; break;
                case 'A': dx = -1; break;
                case 'D': dx = 1; break;

                default:
                    mensagem = "Tecla invalida!";
                    continue;
            }

            int nx = px + dx;
            int ny = py + dy;

            if (ny < 0 || ny >= mapa.length ||
                nx < 0 || nx >= mapa[ny].length) {
                mensagem = "Fora dos limites do mapa!";
                continue;
            }

            char destino = mapa[ny][nx];

            if (destino == '#') {
                mensagem = "Parede bloqueando o caminho!";
                continue;
            }

            movimentos++;
            forca -= 4;

            if (lanterna) {
                bateria -= 6;

                if (bateria <= 0) {
                    bateria = 0;
                    lanterna = false;
                    mensagem = "A bateria acabou!";
                }
            }

            if (destino == 'A') {

                mapa[py][px] = ' ';
                mapa[ny][nx] = ' ';

                forca -= 25;

                px = inicioX;
                py = inicioY;

                mapa[py][px] = 'P';

                mensagem = "JUMPSCARE! Um animatronico atacou! Voltou ao inicio!";

            } else if (destino == 'C') {

                forca = Math.min(100, forca + 35);

                mapa[py][px] = ' ';
                px = nx;
                py = ny;
                mapa[py][px] = 'P';

                mensagem = "Pizza encontrada! +35% de forca!";

            } else if (destino == 'B') {

                bateria = Math.min(100, bateria + 50);

                mapa[py][px] = ' ';
                px = nx;
                py = ny;
                mapa[py][px] = 'P';

                mensagem = "Pilha encontrada! +50% de bateria!";

            } else if (destino == 'S') {

                System.out.println("\nVoce sobreviveu a Noite " +
                    (numero + 1) + "!");
                System.out.println("Movimentos: " + movimentos);

                return movimentos;

            } else {

                mapa[py][px] = ' ';
                px = nx;
                py = ny;
                mapa[py][px] = 'P';
            }
        }
    }

    public static void main(String[] args) {

        String[] personagem = escolherPersonagem();

        String nome = personagem[0];
        String icone = personagem[1];

        int totalMovimentos = 0;

        System.out.println("\nPressione ENTER para iniciar...");
        entrada.nextLine();
        entrada.nextLine();

        for (int nivel = 0; nivel < niveis.length; nivel++) {

            System.out.println("\nPreparando a Noite " + (nivel + 1));
            System.out.println("Pressione ENTER para continuar...");
            entrada.nextLine();
            entrada.nextLine();

            int resultado = jogarNivel(nivel, nome, icone);

            if (resultado == -1) {
                System.out.println("\nJOGO ENCERRADO!");
                return;
            }

            totalMovimentos += resultado;

            if (nivel < niveis.length - 1) {
                System.out.println("\nVoce sobreviveu a esta noite!");
                System.out.println("Pressione ENTER para proxima noite...");
                entrada.nextLine();
                entrada.nextLine();
            }
        }

        System.out.println("\n======================================");
        System.out.println("             VITORIA FINAL!");
        System.out.println("       VOCE ESCAPOU DA PIZZARIA!");
        System.out.println("Movimentos totais: " + totalMovimentos);
        System.out.println("======================================");
    }
}
