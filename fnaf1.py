
import os

def limpar_tela():
    os.system("cls" if os.name == "nt" else "clear")


def escolher_personagem():
    personagens = {
        "1": ("Freddy Fazbear", "🐻"),
        "2": ("Bonnie", "🐰"),
        "3": ("Chica", "🐥"),
        "4": ("Foxy", "🦊"),
        "5": ("Guarda Noturno", "👮")
    }

    print("=== FNAF: ESCAPE DA PIZZARIA ===")
    for numero, (nome, icone) in personagens.items():
        print(f"{numero} - {icone} {nome}")

    while True:
        op = input("Escolha seu personagem (1-5): ").strip()
        if op in personagens:
            return personagens[op]
        print("Entrada invalida! Escolha de 1 a 5.")


def obter_niveis():
    nivel_1 = [
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
    ]

    nivel_2 = [
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
    ]

    return [nivel_1, nivel_2]


def exibir_mapa(mapa, nivel, nome, icone, jog_l, jog_c,
                forca, bateria, lanterna, movimentos, mensagem):

    limpar_tela()

    alcance = 2 if lanterna else 1

    print("=" * 42)
    print(f"       FNAF: LABIRINTO SOMBRIO - NOITE {nivel}")
    print("=" * 42)

    print(f"Personagem: {icone} {nome}")
    print(f"Movimentos: {movimentos}")
    print(f"Forca: {forca}%")
    print(f"Bateria: {bateria}%")
    print(f"Lanterna: {'LIGADA' if lanterna else 'DESLIGADA'}")

    print("\nLegenda:")
    print("P = Jogador | # = Parede | C = Pizza")
    print("B = Pilha | A = Animatronico | S = Saida")

    print("\nMAPA:\n")

    for l in range(len(mapa)):
        linha = ""

        for c in range(len(mapa[l])):
            if abs(l - jog_l) <= alcance and abs(c - jog_c) <= alcance:
                linha += mapa[l][c] + " "
            else:
                linha += "? "

        print(linha)

    print("\nControles: W/A/S/D = Mover")
    print("F = Lanterna | Q = Sair")

    if mensagem:
        print("\nAVISO:", mensagem)


def jogar_nivel(mapa_original, nivel, nome, icone):
    mapa = [list(linha) for linha in mapa_original]

    inicio_l = 0
    inicio_c = 0

    for l in range(len(mapa)):
        for c in range(len(mapa[l])):
            if mapa[l][c] == "P":
                inicio_l, inicio_c = l, c

    jog_l = inicio_l
    jog_c = inicio_c

    forca = 100
    bateria = 100
    lanterna = True
    movimentos = 0
    mensagem = "Encontre itens e escape dos animatronicos!"

    while True:

        if forca <= 0:
            exibir_mapa(mapa, nivel, nome, icone, jog_l, jog_c,
                        0, bateria, lanterna, movimentos, "")
            print("\nGAME OVER! Voce ficou sem forca!")
            return None

        exibir_mapa(
            mapa, nivel, nome, icone,
            jog_l, jog_c, forca, bateria,
            lanterna, movimentos, mensagem
        )

        mensagem = ""
        comando = input("\nSua acao: ").strip().upper()

        if comando == "Q":
            return None

        if comando == "F":
            if not lanterna and bateria <= 0:
                mensagem = "Sem bateria! Encontre uma pilha."
            else:
                lanterna = not lanterna
                mensagem = "Lanterna ligada!" if lanterna else "Lanterna desligada!"
            continue

        direcoes = {
            "W": (-1, 0),
            "S": (1, 0),
            "A": (0, -1),
            "D": (0, 1)
        }

        if comando not in direcoes:
            mensagem = "Tecla invalida! Use W, A, S, D, F ou Q."
            continue

        dl, dc = direcoes[comando]
        nova_l = jog_l + dl
        nova_c = jog_c + dc

        if not (0 <= nova_l < len(mapa) and
                0 <= nova_c < len(mapa[nova_l])):
            mensagem = "Voce nao pode sair do mapa!"
            continue

        destino = mapa[nova_l][nova_c]

        if destino == "#":
            mensagem = "Parede bloqueando o caminho!"
            continue

        movimentos += 1
        forca -= 4

        if lanterna:
            bateria -= 6

            if bateria <= 0:
                bateria = 0
                lanterna = False
                mensagem = "A bateria acabou!"

        if destino == "A":
            mapa[jog_l][jog_c] = " "
            mapa[nova_l][nova_c] = " "

            forca -= 25
            jog_l, jog_c = inicio_l, inicio_c
            mapa[jog_l][jog_c] = "P"

            mensagem = "JUMPSCARE! Um animatronico atacou! Voce voltou ao inicio!"

        elif destino == "C":
            forca = min(100, forca + 35)

            mapa[jog_l][jog_c] = " "
            jog_l, jog_c = nova_l, nova_c
            mapa[jog_l][jog_c] = "P"

            mensagem = "Pizza encontrada! +35% de forca!"

        elif destino == "B":
            bateria = min(100, bateria + 50)

            mapa[jog_l][jog_c] = " "
            jog_l, jog_c = nova_l, nova_c
            mapa[jog_l][jog_c] = "P"

            mensagem = "Pilha encontrada! +50% de bateria!"

        elif destino == "S":
            print(f"\nVoce sobreviveu a Noite {nivel}!")
            print(f"Movimentos: {movimentos}")
            return movimentos

        else:
            mapa[jog_l][jog_c] = " "
            jog_l, jog_c = nova_l, nova_c
            mapa[jog_l][jog_c] = "P"


def jogar():
    nome, icone = escolher_personagem()
    niveis = obter_niveis()
    total_movimentos = 0

    input("\nPressione ENTER para iniciar...")

    for indice, mapa in enumerate(niveis):
        nivel = indice + 1

        print(f"\nPreparando a Noite {nivel}...")
        input("Pressione ENTER para continuar...")

        resultado = jogar_nivel(mapa, nivel, nome, icone)

        if resultado is None:
            print("\nJOGO ENCERRADO!")
            return

        total_movimentos += resultado

        if nivel < len(niveis):
            print("\nVoce conseguiu escapar desta noite!")
            input("Pressione ENTER para proxima noite...")

    print("\n" + "=" * 42)
    print("       VITORIA FINAL!")
    print(" VOCE ESCAPOU DA PIZZARIA!")
    print(f" Movimentos totais: {total_movimentos}")
    print("=" * 42)


if __name__ == "__main__":
    jogar()
