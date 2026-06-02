#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define JOGADOR 1
#define MONSTRO 2

#define MAX_NOME 50
#define MAX_CLASSE 50
#define MAX_HABILIDADE 50

typedef struct habilidade {
    char nome[MAX_HABILIDADE];
    float modificador;
} Habilidade;

typedef struct personagem {
    char nome[MAX_NOME];
    int tipo; // JOGADOR ou MONSTRO
    char classe[MAX_CLASSE];
    int vida;
    int dano;
    int iniciativa;
    int temHabilidade;
    Habilidade habilidade;
} Personagem;

typedef struct no {
    Personagem personagem;
    struct no *prox;
} No;

typedef No *Celula;

typedef struct lista {
    Celula inicio;
    Celula fim;
} Lista;

typedef Lista *ListaCircular;

/* =========================================================
   FUNCOES PRONTAS
   ========================================================= */

ListaCircular novaListaCircular(void) {
    ListaCircular lista = (ListaCircular) malloc(sizeof(Lista));
    if (lista == NULL) return NULL;
    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

Celula novaCelula(Personagem p) {
    Celula nova = (Celula) malloc(sizeof(No));
    if (nova == NULL) return NULL;
    nova->personagem = p;
    nova->prox = NULL;
    return nova;
}

int listaVazia(ListaCircular lista) {
    return lista == NULL || lista->inicio == NULL;
}

void insereNoFimCircular(ListaCircular lista, Personagem p) {
    Celula nova = novaCelula(p);
    if (lista == NULL || nova == NULL) return;

    if (lista->inicio == NULL) {
        lista->inicio = nova;
        lista->fim = nova;
        nova->prox = nova;
    } else {
        nova->prox = lista->inicio;
        lista->fim->prox = nova;
        lista->fim = nova;
    }
}

/* Ordem crescente de iniciativa, como no enunciado da aula */
void insereOrdenadoPorIniciativa(ListaCircular lista, Personagem p) {
    Celula nova, aux;

    if (lista == NULL) return;

    nova = novaCelula(p);
    if (nova == NULL) return;

    if (lista->inicio == NULL) {
        lista->inicio = nova;
        lista->fim = nova;
        nova->prox = nova;
        return;
    }

    if (p.iniciativa < lista->inicio->personagem.iniciativa) {
        nova->prox = lista->inicio;
        lista->inicio = nova;
        lista->fim->prox = lista->inicio;
        return;
    }

    if (p.iniciativa >= lista->fim->personagem.iniciativa) {
        nova->prox = lista->inicio;
        lista->fim->prox = nova;
        lista->fim = nova;
        return;
    }

    aux = lista->inicio;
    while (aux->prox != lista->inicio &&
           aux->prox->personagem.iniciativa <= p.iniciativa) {
        aux = aux->prox;
    }

    nova->prox = aux->prox;
    aux->prox = nova;
}

void printListaCircular(ListaCircular lista) {
    Celula aux;

    if (listaVazia(lista)) {
        printf("[lista vazia]\n");
        return;
    }

    aux = lista->inicio;
    do {
        printf("%s{%s, Classe=%s, HP=%d, D=%d, Ini=%d}",
               aux->personagem.nome,
               aux->personagem.tipo == JOGADOR ? "Jogador" : "Monstro",
               aux->personagem.classe,
               aux->personagem.vida,
               aux->personagem.dano,
               aux->personagem.iniciativa);
        if (aux->personagem.temHabilidade) {
            printf("[Hab=%s x%.1f]",
                   aux->personagem.habilidade.nome,
                   aux->personagem.habilidade.modificador);
        }
        aux = aux->prox;
        if (aux != lista->inicio) printf(" -> ");
    } while (aux != lista->inicio);
    printf("\n");
}

int contarTipo(ListaCircular lista, int tipo) {
    Celula aux;
    int total = 0;

    if (listaVazia(lista)) return 0;

    aux = lista->inicio;
    do {
        if (aux->personagem.tipo == tipo) total++;
        aux = aux->prox;
    } while (aux != lista->inicio);

    return total;
}

void liberarLista(ListaCircular lista) {
    Celula atual, prox;

    if (lista == NULL) return;
    if (lista->inicio == NULL) {
        free(lista);
        return;
    }

    atual = lista->inicio->prox;
    while (atual != lista->inicio) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(lista->inicio);
    free(lista);
}

/* =========================================================
   FUNCOES PARA IMPLEMENTAR
   ========================================================= */
   
/* 

    utilizei essa função que o exercício pede para buscar o inimigo mais
    próximo, dentro dela fiz um caso da celula atual ser vazia e aí 
    retorna null, aí criei um tipo atual que recebe o personagem.tipo como 
    atual. aí fiz uma celula auxiliar para marcar o proximo depois do atual
    e enquanto o proximo for diferente de atual, o aux recebe o próximo 
    e faz uma verificação se o proximo tipo do personagem é diferente do 
    tipo do personsagem atual e se for retorna ele mesmo.

*/

Celula buscaInimigoMaisProximo(Celula atual) {

    if (atual == NULL) return NULL;

    int tipoAtual = atual->personagem.tipo;

    Celula aux = atual->prox;

    while (aux != atual) {

        if (aux->personagem.tipo != tipoAtual) {
            return aux;
        }

        aux = aux->prox;
    }

    return NULL;
}

/*
TODO 2:
Implemente a remocao de um no especifico de uma lista circular.
Casos importantes:
- lista vazia
- alvo eh o unico elemento
- alvo eh o inicio
- alvo eh o fim
- alvo esta no meio
*/

/* 
    aqui eu utilizei a função removeDaListaCircular com os parâmetros que o 
    exercício passou. 
    Fiz uma verificação para ver se a lista é vazia ou se o alvo que quero 
    remover é null e eu retorno a lista pois não há nada para remover.
    
    faço um celula aux para demarcar o inicio da lista e celula anterior para
    demarcar o fim da lista. 
    
    aí eu fiz um do while para fazer a verificação de o inicio ser igual ao 
    meu alvo e dentro dessa condição eu tenho outras duas condições
    para remover o único elemento ou remover do inicio e retornar a lista 
    
    aí fiz um outro se para caso o número alvo esteja no fim da lista e se 
    não for nenhuma dessas condições eu tiro do meio da lista fazendo o prox
    do anterior ser igual ao proximo do inicio e tirando o inicio.
    
    e eu faço tudo isso enquanto o meu inicio que é o aux for diferente do
    próprio inicio.

*/
void removeDaListaCircular(ListaCircular lista, Celula alvo) {

    if (listaVazia(lista) || alvo == NULL) return;

    Celula aux = lista->inicio;
    Celula anterior = lista->fim;

    do {

        if (aux == alvo) {

            if (lista->inicio == lista->fim) {
                free(alvo);
                lista->inicio = NULL;
                lista->fim = NULL;
                return;
            }

            if (aux == lista->inicio) {
                lista->inicio = aux->prox;
                lista->fim->prox = lista->inicio;
                free(aux);
                return;
            }

            if (aux == lista->fim) {
                anterior->prox = lista->inicio;
                lista->fim = anterior;
                free(aux);
                return;
            }

            anterior->prox = aux->prox;
            free(aux);
            return;
        }

        anterior = aux;
        aux = aux->prox;

    } while (aux != lista->inicio);
}

/*
TODO 3:
Implemente UM turno de combate.
Passos sugeridos:
1. encontrar o inimigo mais proximo
2. guardar quem sera o proximo da rodada
3. calcular o dano base do personagem atual
4. se ele tiver habilidade, gerar um numero aleatorio entre 0 e 1
   e, com 20%% de chance, multiplicar o dano pelo modificador
5. aplicar o dano ao alvo
6. se o alvo morrer, removê-lo da lista
7. retornar o ponteiro do proximo personagem que devera agir
*/


/* 

    Para a função executa um turno eu fiz uma verificação para ver se a lista 
    era vazia ou se a celula atual é = a null igual a outra função mas a 
    diferença é que aqui retorna null e não a lista.
    aí eu fiz celula inimigo e fiz a chamada da primeira função recebendo 
    atual como parâmetro.
    
    aí se inimigo for vazio ou seja se não existir vai retornar o próximo, vai 
    passar para o próximo.
    
    aí eu fiz celula proximo recebe o próximo valor do atual e fiz 
    uma variável int dano que recebe personagem.dano do personagem
    
    e se personagem tem habilidade, eu gero um número aleatório de 0 
    a 99
    aí fiz um if para ele usar a habildade com 20% de chance. 
    pego o nome do personagem e pego o nome da habilidade do personagem e aplico
    o modificador da habilidade.
    
    depois disso pego o nome do personagem e o nome do inimigo e o dano que 
    levou com ou sem o modificador e faço o cálculo do dano do personagem
    
    e para finalizar se a vida do personagem é menor ou igual a 0
    printo na tela que o inimigo foi derrotado e eu o removo da lista circular 
    e aí retorna o próximo personagem do turno.
    
*/
Celula executaUmTurno(ListaCircular lista, Celula atual) {

    if (listaVazia(lista) || atual == NULL) return NULL;

    Celula inimigo = buscaInimigoMaisProximo(atual);

    if (inimigo == NULL) return atual->prox;

    Celula proximo = atual->prox;

    int dano = atual->personagem.dano;

    if (atual->personagem.temHabilidade) {

        int r = rand() % 100;

        if (r < 20) {
            printf("%s usa habilidade %s!\n",
                   atual->personagem.nome,
                   atual->personagem.habilidade.nome);

            dano = dano *
                   atual->personagem.habilidade.modificador;
        }
    }

    printf("%s ataca %s causando %d de dano.\n",
           atual->personagem.nome,
           inimigo->personagem.nome,
           dano);

    inimigo->personagem.vida -= dano;

    if (inimigo->personagem.vida <= 0) {

        printf("%s foi derrotado!\n",
               inimigo->personagem.nome);

        removeDaListaCircular(lista, inimigo);
    }

    return proximo;
}

/* =========================================================
   TESTES / MAINS DE EXEMPLO
   ========================================================= */

void teste1_iniciativa(void) {
    ListaCircular lista = novaListaCircular();

    Personagem a  = {"Jogador A", JOGADOR, "Guerreiro", 30,  8, 20, 1, {"Ataque Heroico", 1.5f}};
    Personagem b  = {"Jogador B", JOGADOR, "Mago",      20, 10, 10, 1, {"Bola de Fogo",   2.0f}};
    Personagem c  = {"Jogador C", JOGADOR, "Ladino",    25,  6,  5, 0, {"",               0.0f}};
    Personagem m1 = {"Monstro 1", MONSTRO, "Orc",       18,  7,  7, 0, {"",               0.0f}};
    Personagem m2 = {"Monstro 2", MONSTRO, "Goblin",    15,  4,  1, 0, {"",               0.0f}};

    insereOrdenadoPorIniciativa(lista, a);
    insereOrdenadoPorIniciativa(lista, b);
    insereOrdenadoPorIniciativa(lista, c);
    insereOrdenadoPorIniciativa(lista, m1);
    insereOrdenadoPorIniciativa(lista, m2);

    printf("=== TESTE 1: ORDEM DE INICIATIVA ===\n");
    printListaCircular(lista);

    liberarLista(lista);
}

void teste2_um_turno_sem_morte(void) {
    ListaCircular lista = novaListaCircular();

    Personagem a = {"Aragorn", JOGADOR, "Guerreiro", 30, 10, 4, 0, {"", 0.0f}};
    Personagem g = {"Gandalf", JOGADOR, "Mago",      20, 12, 8, 0, {"", 0.0f}};
    Personagem o = {"Orc",     MONSTRO, "Orc",       18,  5, 6, 0, {"", 0.0f}};

    insereOrdenadoPorIniciativa(lista, a);
    insereOrdenadoPorIniciativa(lista, g);
    insereOrdenadoPorIniciativa(lista, o);

    printf("=== TESTE 2: UM TURNO SEM MORTE ===\n");
    printf("Antes do turno:\n");
    printListaCircular(lista);

    executaUmTurno(lista, lista->inicio);

    printf("Depois do turno:\n");
    printListaCircular(lista);

    liberarLista(lista);
}

void teste3_remocao_apos_derrota(void) {
    ListaCircular lista = novaListaCircular();
    Celula atual;

    Personagem a = {"Aragorn", JOGADOR, "Guerreiro", 30, 20, 4, 0, {"", 0.0f}};
    Personagem g = {"Gandalf", JOGADOR, "Mago",      20, 12, 8, 0, {"", 0.0f}};
    Personagem o = {"Orc",     MONSTRO, "Orc",       15,  5, 6, 0, {"", 0.0f}};

    insereOrdenadoPorIniciativa(lista, a);
    insereOrdenadoPorIniciativa(lista, g);
    insereOrdenadoPorIniciativa(lista, o);

    printf("=== TESTE 3: REMOCAO APOS DERROTA ===\n");
    printf("Antes do turno:\n");
    printListaCircular(lista);

    atual = lista->inicio;
    executaUmTurno(lista, atual);

    printf("Depois do turno:\n");
    printListaCircular(lista);

    liberarLista(lista);
}

void teste4_varios_turnos_com_habilidade(void) {
    ListaCircular lista = novaListaCircular();
    Celula atual;
    int turno;

    Personagem a  = {"Jogador A", JOGADOR, "Guerreiro", 25,  8, 2, 1, {"Golpe Heroico", 1.5f}};
    Personagem b  = {"Jogador B", JOGADOR, "Mago",      18, 10, 5, 1, {"Raio Arcano",   2.0f}};
    Personagem m1 = {"Monstro 1", MONSTRO, "Orc",       16,  6, 3, 0, {"",              0.0f}};
    Personagem m2 = {"Monstro 2", MONSTRO, "Goblin",    12,  4, 1, 0, {"",              0.0f}};

    insereOrdenadoPorIniciativa(lista, a);
    insereOrdenadoPorIniciativa(lista, b);
    insereOrdenadoPorIniciativa(lista, m1);
    insereOrdenadoPorIniciativa(lista, m2);

    atual = lista->inicio;

    /* Seed fixa para deixar a execucao reproduzivel */
    srand(42);

    printf("=== TESTE 4: VARIOS TURNOS COM HABILIDADE ===\n");
    printListaCircular(lista);

    for (turno = 1; turno <= 6; turno++) {
        if (contarTipo(lista, JOGADOR) == 0 || contarTipo(lista, MONSTRO) == 0) {
            break;
        }

        printf("\nTurno %d\n", turno);
        atual = executaUmTurno(lista, atual);
        printListaCircular(lista);
    }

    liberarLista(lista);
}

int main(void) {
    /*
    Descomente um teste por vez, ou deixe todos ativos apos implementar
    as funcoes pedidas.
    */

    teste1_iniciativa();
    printf("\n");

    teste2_um_turno_sem_morte();
    printf("\n");

    teste3_remocao_apos_derrota();
    printf("\n");

    teste4_varios_turnos_com_habilidade();

    return 0;
}