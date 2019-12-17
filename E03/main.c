#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define N_STAT 7
#define FILE_PERS "pg.txt"
#define FILE_INV "inventario.txt"
#define MAX_RIGA 250
#define MAX_EQUIP 8
#define LEN_CODICE 7
#define MAX_STR 50
typedef enum{hp,mp,atk,def,mag,spr}stats;
typedef struct nodoPg_s *linkPg, nodoPg;
typedef struct{
    char* nome;
    char* tipo;
    int* stat;
}inv_t;
typedef struct{
    inv_t* vettInv;
    int nInv;
    int maxInv;
}tabInv_t;
typedef struct{
    int inUso;
    inv_t **vettEq;
}tabEquip_t;
typedef struct{
    char* codice;
    char* nome;
    char* classe;
    tabEquip_t *equip;
    int *stat;

}pg_t;
struct nodoPg_s{
    pg_t *personaggio;
    linkPg next;
};
typedef struct{
    linkPg headPg;
    linkPg tailPg;
    int nPg;
}tabPg_t;

/***PERSONAGGIO */
tabPg_t* loadListPers(char* filename);
tabPg_t* eliminaPers(tabPg_t* tab,char *codice);
pg_t* ricercaPg(tabPg_t* tabPg,char* codice);
pg_t *allocaPersonaggio(char* riga);
tabPg_t* allocaListaPers();
void printTabPg(tabPg_t* tabPg);
void printPersonaggio(pg_t* p);
int aggiungiEquipaggiamento(inv_t* equip,pg_t* p);
tabPg_t* aggiungiPersonaggio(tabPg_t* t,char* attributi);
int rimuoviEquipaggiamento(char* nome, pg_t *p);
void calcolaStats(pg_t *pg);
/***-------------------*/
/***INVENTARIO*/
tabInv_t* allocaTabInv(int nmax);
inv_t* cercaOggetto(tabInv_t *tabInv,char* nome);
tabInv_t* caricaTabInv(char* filename);
void printTabInv(tabInv_t* tabella);
void printOggetto(inv_t oggetto);
int main()
{

    inv_t* e,*e1,*e2;
    pg_t *pg;
    tabPg_t* tabellaPg;
    tabInv_t* tabellaInv;
    char riga[MAX_RIGA],codice[LEN_CODICE];
    printf("Loading [Tabella personaggi] from file %s...\n",FILE_PERS);
    printf("\n---PERSONAGGI---\n");
    tabellaPg = loadListPers(FILE_PERS);
    printTabPg(tabellaPg);
    printf("Loading [Tabella inventario] from file %s...\n",FILE_INV);
    printf("\n---INVENTARIO---\n");
    tabellaInv = caricaTabInv(FILE_INV);
    printTabInv(tabellaInv);
    /*printf("Inserisci attributi personaggio:\n");
    fgets(riga,MAX_RIGA,stdin);
    tabellaPg = aggiungiPersonaggio(tabellaPg,riga);
    printTabPg(tabellaPg);
    printf("Inserisci il codice del PG da eliminare [PGXXXX]\n");
    scanf("%s",codice);
    tabellaPg = eliminaPers(tabellaPg,codice);
    printTabPg(tabellaPg);*/
    e = cercaOggetto(tabellaInv,"Excalibur");
    pg = ricercaPg(tabellaPg,"PG0002");
    printf("\nRicerca PG0002...\n");
    printPersonaggio(pg);
    printOggetto(*e);
    e1 = cercaOggetto(tabellaInv,"dsadad");
    e2 = cercaOggetto(tabellaInv,"Fiocco");
    aggiungiEquipaggiamento(e2,pg);
    aggiungiEquipaggiamento(e1,pg);
    aggiungiEquipaggiamento(e,pg);
    printPersonaggio(pg);
    rimuoviEquipaggiamento("Excalibur",pg);
    printPersonaggio(pg);

    calcolaStats(pg);
    return 0;
}
/**Personaggi*/

void calcolaStats(pg_t *pg){
    int stat[N_STAT];
    int N = pg->equip->inUso;
    int i,j;
    if(N == 0) return;
    printf("PERSONAGGIO: %s \t",pg->nome);
    for(i = 0; i < N_STAT;i++)
        stat[i] = pg->stat[i];
    for(i = 0; i < N; i++)
        for(j = 0; j < N_STAT; j++)
            stat[j] += pg->equip->vettEq[i]->stat[j];
    printf("OK\n");
    for(i = 0; i < N_STAT;i++)
        printf("%d ",(stat[i] >= 1)?stat[i]:1);
    return;
}
int rimuoviEquipaggiamento(char* nome, pg_t *p){
    int i,last;
    int trovato = 0;
    for(i = 0; i < last;i++){
        if(strcmp(p->equip->vettEq[i]->nome,nome) == 0){
            p->equip->vettEq[i] = NULL;
            p->equip->inUso--;
            trovato = 1;
            break;
        }
    }
    //SHIFT LEFT
    last = p->equip->inUso;
    if(i < last-1 && trovato)
        for(;i < last;i++){
            p->equip->vettEq[i] = p->equip->vettEq[i+1];
        }
    return trovato;
}
pg_t* ricercaPg(tabPg_t* tabPg, char* codice){
    linkPg p;
    for(p = tabPg->headPg; p != NULL; p = p->next){
        if(strcmp(p->personaggio->codice,codice) == 0)
            return p->personaggio;
    }
    return NULL;
}
void liberaPersonaggio(pg_t* p){
    free(p->classe);
    free(p->codice);
    free(p->nome);
    free(p->stat);

    free(p);
}
int aggiungiEquipaggiamento(inv_t* equip, pg_t* p){
    int pos = p->equip->inUso;
    if(p->equip->inUso < 8 && equip != NULL){
        p->equip->vettEq[pos] = equip;
        p->equip->inUso++;
        return 1;
    }
    return -1;
}
void printPersonaggio(pg_t* p){
    int N,i;
    N = p->equip->inUso;
    printf("CODICE:%s\tNOME:%s\tCLASSE:%s\t",p->codice,p->nome,p->classe);
    printf("STATISTICHE:");
    for(int i = 0; i < N_STAT;i++)
        printf("%d ",p->stat[i]);
    printf("\nEQUIPAGGIAMENTO: \n");
    for(i = 0; i < N;i++)
        printOggetto(*(p->equip->vettEq[i]));


}
tabPg_t* allocaListaPers(){
    tabPg_t* tabP;
    tabP = malloc(sizeof(tabPg_t));
    tabP->headPg = NULL;
    tabP->nPg = 0;
    tabP->tailPg = NULL;
    //printf("OK allocazione\n");
    return tabP;
}
tabPg_t* loadListPers(char* filename){
    FILE* f_in;
    char riga[MAX_RIGA];
    tabPg_t* tabellaPg;
    linkPg node;
    f_in = fopen(filename, "r");
    tabellaPg = allocaListaPers();

    while(( fgets(riga,MAX_RIGA,f_in))!= NULL){
        //printf("%s\n", riga);
        node = malloc(sizeof *node);
        node->personaggio = allocaPersonaggio(riga);
        node->next = NULL;

        if(tabellaPg->headPg == NULL){
            tabellaPg->headPg = tabellaPg->tailPg = node;
        }else{
            tabellaPg->tailPg->next = node;
            tabellaPg->tailPg = node;
        }
        tabellaPg->nPg++;
    }
    fclose(f_in);
    return tabellaPg;
}
void printTabPg(tabPg_t *tabPg){
    linkPg p;
    for(p = tabPg->headPg;p != NULL;p = p->next)
        printPersonaggio(p->personaggio);
}
pg_t* allocaPersonaggio(char* riga){

    char codice[LEN_CODICE];
    char nome[MAX_STR], classe[MAX_STR];
    pg_t* p;

    p = malloc(sizeof(*p));
    p->stat = malloc(sizeof(int) * N_STAT);
    sscanf(riga,"%s %s %s %d %d %d %d %d %d",
           codice,nome,classe,&p->stat[0],&p->stat[1],&p->stat[2],&p->stat[3],&p->stat[4],&p->stat[5]);

    p->codice = strdup(codice);
    p->nome = strdup(nome);
    p->classe = strdup(classe);
    p->equip = malloc(sizeof(tabEquip_t*));
    p->equip->vettEq = malloc(sizeof(inv_t*) * 8);
    p->equip->inUso = 0;
    return p;

}
tabPg_t* aggiungiPersonaggio(tabPg_t* t,char* attributi){
    linkPg node;
    node = malloc(sizeof(*node));
    node->personaggio = allocaPersonaggio(attributi);
    printf("%s \n",attributi);
    node->next = NULL;

    if(t->nPg == 0)
        t->headPg = t->tailPg = node;
    else{
        t->tailPg->next = node;
        t->tailPg = node;
    }
    t->nPg++;
    return t;
}
tabPg_t* eliminaPers(tabPg_t* tab,char *codice){
    linkPg p,x;
    if(tab->nPg == 0) return NULL;
    for(x = tab->headPg,p = NULL; x !=  NULL; p = x, x = x->next){
        if(strcmp(codice,x->personaggio->codice) == 0){
            if(x == tab->headPg)
                tab->headPg = x->next;
            else
                p->next = x->next;
            liberaPersonaggio(x->personaggio);
            break;
        }
    }
    return tab;
}
/**inventario*/
inv_t* cercaOggetto(tabInv_t *tabInv,char* nome){
    int N,i;
    N = tabInv->nInv;
    if(N == 0) return NULL;
    for(i = 0; i < N; i++){
        if(strcmp(nome,tabInv->vettInv[i].nome) == 0)
            return &(tabInv->vettInv[i]);
    }
    return NULL;
}
void printOggetto(inv_t oggetto){
    printf("NOME: %s\t TIPO: %s \t",oggetto.nome,oggetto.tipo);
    printf("STATISTICHE: ");
    for(int i = 0; i < N_STAT; i++)
        printf("%d ",oggetto.stat[i]);
    printf("\n");
}
void printTabInv(tabInv_t* tabella){
    int i;
    for(i = 0; i < tabella->nInv; i++)
        printOggetto(tabella->vettInv[i]);
}
tabInv_t* allocaTabInv(int nmax){
    tabInv_t* tabInv;
    tabInv = malloc(sizeof(*tabInv));
    tabInv->vettInv = malloc(sizeof(inv_t) * nmax);
    tabInv->maxInv = nmax;
    tabInv->nInv = 0;
    return tabInv;
}
tabInv_t* caricaTabInv(char* filename){
    tabInv_t* tabInv;
    int i, O,s;
    char nome[MAX_STR], tipo[MAX_STR];
    FILE* f_in;
    f_in = fopen(filename, "r");

    if(f_in == NULL) return NULL;
    fscanf(f_in, "%d",&O);
    tabInv = allocaTabInv(O);
    for(i = 0; i < O; i++){
        fscanf(f_in,"%s %s",nome,tipo);
        tabInv->vettInv[i].stat = malloc(sizeof(int) * N_STAT);
        for(int j = 0; j < N_STAT; j++){
            fscanf(f_in,"%d",&s);
            tabInv->vettInv[i].stat[j] = s;
        }

        tabInv->vettInv[i].nome = strdup(nome);
        tabInv->vettInv[i].tipo = strdup(tipo);
    }
    tabInv->nInv = O;
    fclose(f_in);
    return tabInv;
}


