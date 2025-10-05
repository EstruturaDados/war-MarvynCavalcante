#include <stdio.h>
#include <stdlib.h> // Para calloc, free, rand, srand
#include <time.h>   // Para time (usado no srand)
#include <string.h> // Para manipulação de strings

// Constantes
#define NUM_TERRITORIOS 5
#define TAM_NOME 40
#define TAM_COR 15

// --- Estrutura de Dados ---
struct Territorio {
    char nome[TAM_NOME];
    char cor_exercito[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
void cadastrar_territorios(struct Territorio *territorios, int num_territorios);
void exibir_mapa(const struct Territorio *territorios, int num_territorios);
void simular_batalha(struct Territorio *territorios, int num_territorios);
int lancar_dado();

// --- Implementação das Funções ---

/*
 * Gera um número aleatório entre 1 e 6, simulando um dado.
 */
int lancar_dado() {
    return (rand() % 6) + 1; // rand() % 6 gera 0 a 5. + 1 resulta em 1 a 6.
}

/*
 * Preenche os dados dos territórios com entrada do usuário.
 * Recebe o ponteiro para o bloco de memória alocado.
 */
void cadastrar_territorios(struct Territorio *territorios, int num_territorios) {
    printf("==========================================\n");
    printf("     Cadastrando %d Territórios (Alocação Dinâmica)\n", num_territorios);
    printf("==========================================\n");

    for (int i = 0; i < num_territorios; i++) {
        printf("\n--- Território #%d ---\n", i + 1);
        
        // Limpa o buffer de entrada, essencial ao misturar scanf e fgets
        if (i > 0) while (getchar() != '\n'); 
        
        // Leitura do Nome (usando fgets)
        printf("Nome do Território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;
        
        // Leitura da Cor do Exército (usando scanf simples)
        printf("Cor do Exército Dominante: ");
        scanf("%s", territorios[i].cor_exercito);
        
        // Leitura do Número de Tropas
        printf("Número de Tropas (Mínimo 2): ");
        scanf("%d", &territorios[i].tropas);
        
        // Garantir no mínimo 2 tropas para poder atacar
        if (territorios[i].tropas < 2) {
            territorios[i].tropas = 2;
            printf("(Ajustado para 2 tropas para permitir ataque)\n");
        }
    }
}

/*
 * Exibe o estado atual de todos os territórios.
 * Usa 'const' para garantir que a função não altere a estrutura.
 */
void exibir_mapa(const struct Territorio *territorios, int num_territorios) {
    printf("\n==========================================\n");
    printf("           ESTADO ATUAL DO MAPA\n");
    printf("==========================================\n");
    
    for (int i = 0; i < num_territorios; i++) {
        printf("  [TERRITÓRIO %d] %s\n", i + 1, territorios[i].nome);
        printf("  Cor Exército: %s\n", territorios[i].cor_exercito);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("------------------------------------------\n");
    }
}

/*
 * Simula a batalha entre o território atacante e o defensor.
 * A lógica dos dados é crucial aqui.
 */
void simular_batalha(struct Territorio *territorios, int num_territorios) {
    int id_atacante, id_defensor;
    int dado_ataque, dado_defesa;
    
    exibir_mapa(territorios, num_territorios);
    
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o Território Atacante (1 a %d): ", num_territorios);
    scanf("%d", &id_atacante);

    printf("Escolha o Território Defensor (1 a %d): ", num_territorios);
    scanf("%d", &id_defensor);

    // Validações
    if (id_atacante < 1 || id_atacante > num_territorios || id_defensor < 1 || id_defensor > num_territorios || id_atacante == id_defensor) {
        printf("\n❌ Seleção inválida. Tente novamente.\n");
        return;
    }
    
    // Obtém ponteiros para as structs (Permite acesso mais idiomático)
    struct Territorio *atacante = &territorios[id_atacante - 1];
    struct Territorio *defensor = &territorios[id_defensor - 1];

    if (atacante->tropas < 2) {
        printf("\n❌ Ataque cancelado: O atacante precisa de pelo menos 2 tropas (1 para o ataque, 1 para defesa).\n");
        return;
    }

    // Lançamento dos dados
    dado_ataque = lancar_dado();
    dado_defesa = lancar_dado();

    printf("\n--- RESULTADO DOS DADOS ---\n");
    printf("Atacante (%s) tirou: %d\n", atacante->nome, dado_ataque);
    printf("Defensor (%s) tirou: %d\n", defensor->nome, dado_defesa);
    
    // Lógica da Batalha: Empates favorecem o atacante!
    if (dado_ataque >= dado_defesa) {
        // Atacante vence (ou empata) -> Defensor perde 1 tropa
        defensor->tropas--;
        printf("\n✅ Vitória do Atacante! O defensor (%s) perde 1 tropa.\n", defensor->nome);
        
        // Verifica Conquista
        if (defensor->tropas <= 0) {
            printf("\n🎉 TERRITÓRIO CONQUISTADO! 🎉\n");
            printf("%s foi anexado ao exército %s!\n", defensor->nome, atacante->cor_exercito);
            
            // Defensor perde o território e ganha a cor/tropas do atacante
            