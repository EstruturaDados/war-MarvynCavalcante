#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <time.h>   // Para time()
#include <string.h> // Para funções de string

// Constantes
#define NUM_TERRITORIOS 5
#define TAM_NOME 40
#define TAM_COR 15

// Enumeração para facilitar a leitura dos tipos de missão
typedef enum {
    DESTRUIR_VERDE = 1,
    CONQUISTAR_TRES = 2
} TipoMissao;

// --- Estrutura de Dados ---
struct Territorio {
    char nome[TAM_NOME];
    char cor_exercito[TAM_COR];
    int tropas;
    int conquistado; // 0 = Não, 1 = Sim (Usado para a missão "Conquistar 3")
};

// --- Protótipos das Funções ---
void inicializar_jogo(struct Territorio territorios[], int *missao_atual);
void exibir_menu();
void exibir_mapa(const struct Territorio territorios[]);
void realizar_ataque(struct Territorio territorios[]);
void verificar_missao(const struct Territorio territorios[], int missao_atual);

// --- Implementação das Funções ---

/*
 * Inicializa automaticamente os dados dos territórios e atribui a missão.
 * Usa const correctness para os arrays de strings fixos.
 */
void inicializar_jogo(struct Territorio territorios[], int *missao_atual) {
    const char *nomes[] = {"America do Sul", "Europa Ocidental", "Africa", "Asia", "Oceania"};
    const char *cores[] = {"Vermelho", "Azul", "Verde", "Amarelo", "Preto"};
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Configuração dos Territórios
    printf("Inicializando o mapa com dados automáticos...\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(territorios[i].nome, nomes[i]);
        strcpy(territorios[i].cor_exercito, cores[i]);
        // Tropas iniciais entre 5 e 10
        territorios[i].tropas = rand() % 6 + 5; 
        territorios[i].conquistado = 0; // Nenhum conquistado inicialmente
    }
    
    // Atribuição Aleatória da Missão
    *missao_atual = rand() % 2 + 1; // Gera 1 ou 2
    printf("Missão atribuída com sucesso!\n\n");
}

/*
 * Exibe o menu interativo para o jogador.
 */
void exibir_menu() {
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair\n");
    printf("Escolha sua ação: ");
}

/*
 * Exibe o estado atual de todos os territórios.
 * Usa 'const' para garantir que a função não altere o array de structs.
 */
void exibir_mapa(const struct Territorio territorios[]) {
    printf("\n==========================================\n");
    printf("           ESTADO ATUAL DO MAPA\n");
    printf("==========================================\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("[TERRITÓRIO %d] %s\n", i + 1, territorios[i].nome);
        printf("  Cor Exército: %s\n", territorios[i].cor_exercito);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("------------------------------------------\n");
    }
}

/*
 * Simula uma batalha simples, alterando as tropas do território atacado.
 * O array de structs é passado por referência (por ser um array), permitindo alteração.
 */
void realizar_ataque(struct Territorio territorios[]) {
    int id_atacante, id_defensor;
    int perda_tropas;

    exibir_mapa(territorios);
    
    printf("\nEscolha o Território Atacante (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &id_atacante);

    printf("Escolha o Território Defensor (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &id_defensor);

    // Validação básica dos IDs
    if (id_atacante < 1 || id_atacante > NUM_TERRITORIOS || id_defensor < 1 || id_defensor > NUM_TERRITORIOS || id_atacante == id_defensor) {
        printf("\n❌ Seleção inválida. Tente novamente.\n");
        return;
    }
    
    // Usa índices 0-based
    struct Territorio *defensor = &territorios[id_defensor - 1]; 
    struct Territorio *atacante = &territorios[id_atacante - 1]; 

    printf("\nIniciando Batalha: %s (vs) %s\n", atacante->nome, defensor->nome);

    // Simulação simplificada de ataque: perdas são 50% das tropas do defensor
    perda_tropas = defensor->tropas / 2 + 1; // Pelo menos 1 tropa
    
    printf("Perda de tropas do defensor: %d\n", perda_tropas);

    defensor->tropas -= perda_tropas;
    
    if (defensor->tropas <= 0) {
        printf("\n🎉 Vitória! O exército %s conquistou %s!\n", atacante->cor_exercito, defensor->nome);
        
        // Atualiza o defensor para o exército atacante
        strcpy(defensor->cor_exercito, atacante->cor_exercito);
        defensor->tropas = 1; // Deixa 1 tropa de ocupação
        defensor->conquistado = 1; // Marca como conquistado
        
    } else {
        printf("\nO ataque falhou em conquistar o território. Tropas restantes em %s: %d\n", defensor->nome, defensor->tropas);
    }
}

/*
 * Verifica se a condição de vitória da missão atual foi cumprida.
 * Usa 'const' pois apenas lê os dados para verificação.
 */
void verificar_missao(const struct Territorio territorios[], int missao_atual) {
    int vitoria = 0;
    
    printf("\n--- VERIFICAÇÃO DE MISSÃO ---\n");

    if (missao_atual == DESTRUIR_VERDE) {
        printf("Missão Atual: Destruir o Exército Verde.\n");
        int verde_presente = 0;
        
        // Verifica se a cor "Verde" ainda está presente no mapa
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(territorios[i].cor_exercito, "Verde") == 0) {
                verde_presente = 1;
                break;
            }
        }

        if (!verde_presente) {
            printf("✅ Parabéns! O exército Verde foi eliminado!\n");
            vitoria = 1;
        } else {
            printf("❌ O exército Verde ainda possui territórios.\n");
        }
        
    } else if (missao_atual == CONQUISTAR_TRES) {
        printf("Missão Atual: Conquistar 3 Territórios.\n");
        int conquistas = 0;
        
        // Conta quantos territórios foram conquistados (campo 'conquistado' = 1)
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (territorios[i].conquistado == 1) {
                conquistas++;
            }
        }

        if (conquistas >= 3) {
            printf("✅ Parabéns! Você conquistou %d territórios!\n", conquistas);
            vitoria = 1;
        } else {
            printf("❌ Você conquistou %d de 3 territórios necessários.\n", conquistas);
        }
    }
    
    if (vitoria) {
        printf("\n==========================================\n");
        printf("         🏆 JOGADOR VENCEU O JOGO! 🏆\n");
        printf("==========================================\n");
        // O jogador pode sair após esta mensagem no loop principal
    }
}

// --- Função Principal (Main) ---
int main() {
    // Declaração das variáveis principais
    struct Territorio mapa_territorios[NUM_TERRITORIOS];
    int missao_do_jogador;
    int escolha;

    // Inicialização do Jogo e Missão (Modularização)
    inicializar_jogo(mapa_territorios, &missao_do_jogador); 
    // '&' indica passagem por referência para alterar missao_do_jogador

    // Loop Principal do Jogo
    do {
        exibir_mapa(mapa_territorios);
        exibir_menu();
        
        // Limpa o buffer antes do scanf, boa prática
        while (getchar() != '\n'); 
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                realizar_ataque(mapa_territorios);
                break;
            case 2:
                verificar_missao(mapa_territorios, missao_do_jogador);
                break;
            case 0:
                printf("\nSaindo do jogo. Até a próxima!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
        
    } while (escolha != 0);

    return 0;
}