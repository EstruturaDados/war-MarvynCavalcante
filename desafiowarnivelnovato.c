#include <stdio.h>  // Para printf, scanf, fgets
#include <string.h> // Para strcspn (opcional, mas útil para limpar strings)

// Constante para definir o número fixo de territórios
#define NUM_TERRITORIOS 5

// --- 1. Criação da struct Territorio ---
/* * Define uma estrutura para armazenar as informações de um território.
 */
struct Territorio {
    char nome[40];       // Nome do território (tamanho maior para usar fgets com segurança)
    char cor_exercito[15]; // Cor do exército
    int numero_tropas;   // Número de tropas
};

int main() {
    // --- 2. Vetor Estático de 5 Elementos ---
    // Declara um array (vetor estático) para armazenar 5 estruturas.
    struct Territorio mapa[NUM_TERRITORIOS];
    
    int i; // Variável de controle
    
    printf("==================================================\n");
    printf("  🛡️ Sistema de Cadastro Inicial de Territórios 🛡️\n");
    printf("==================================================\n");
    printf("Você irá cadastrar os dados para %d territórios.\n\n", NUM_TERRITORIOS);
    
    // --- 3. Laço de Cadastro de Dados (Entrada) ---
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Cadastro do Território #%d ---\n", i + 1);
        
        // 1. Leitura do Nome (usando fgets)
        // Usamos fgets para ler strings que podem conter espaços.
        printf("Digite o Nome do Território: ");
        
        // Limpa o buffer de entrada (evita que o \n pendente do scanf anterior atrapalhe o fgets)
        if (i > 0) while (getchar() != '\n'); 
        
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        
        // Remove o caractere de quebra de linha (\n) que o fgets adiciona
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        
        // 2. Leitura da Cor do Exército (usando scanf)
        printf("Digite a Cor do Exército Dominante: ");
        // O "%14s" limita a leitura a 14 caracteres para evitar buffer overflow.
        scanf("%14s", mapa[i].cor_exercito);

        // 3. Leitura do Número de Tropas (usando scanf)
        printf("Digite o Número de Tropas: ");
        scanf("%d", &mapa[i].numero_tropas);
        
        printf("Território #%d cadastrado com sucesso!\n\n", i + 1);
    }

    // --- 4. Laço de Impressão Organizada (Saída) ---
    printf("\n==================================================\n");
    printf("           ESTADO ATUAL DO MAPA DE JOGO\n");
    printf("==================================================\n");

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("  [TERRITÓRIO %d]\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor do Exército: %s\n", mapa[i].cor_exercito);
        printf("  Tropas: %d\n", mapa[i].numero_tropas);
        printf("--------------------------------------------------\n");
    }

    return 0;
}