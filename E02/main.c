#include <stdio.h>
#include <stdlib.h>
#define TEST_FILE "e2_test_set_PROG.txt"
#define N_PIETRE 4
typedef enum{
    p_zaffiri, p_rubini, p_topazi, p_smeraldi, p_pietre
} e_pietre;

char* gemme[N_PIETRE] = {"Z", "R", "T", "S"}; // vettore parallelo a valori e a disp_pietre
int valori[N_PIETRE]; //variabile globale in cui sono contenuti i valori di ogni pietra

void generaCollane(int *val, int len, int max_rip); // funzione wrapper che va dalla lunghezza massima a 0
int disp_rip(int pos,int prec, int *val, int len, int *sol, int k, int *bestSol, int *bestSolLen, int *bestSolVal,int max_rip);
int verifica(int *sol, int len); //return il valore della collana, se ci sono ripetizioni di zaffiri superiori agli smeraldi return -1;

int main(){
    int pietre[N_PIETRE];
    int i,j,n_test,max_rip;

    FILE* f_test;
    //apertura del file di test
    f_test = fopen(TEST_FILE, "r");
    if(f_test == NULL) {
        printf("Impossibile aprire il file di TEST\n");
        exit(-1);
    }
    ///TEST
    fscanf(f_test, "%d",&n_test);
    for(j = 0; j < n_test; j++){
        printf("\n\n\nTEST #%d\n",j+1);
        for(i = 0; i < N_PIETRE; i++){
            fscanf(f_test,"%d",&pietre[i]);
            printf("%s: %d\n",gemme[i],pietre[i]);
        }
        for(i = 0; i < N_PIETRE; i++)
            fscanf(f_test,"%d",&valori[i]);
        fscanf(f_test, "%d",&max_rip);

        generaCollane(pietre,N_PIETRE,max_rip);
    }
}

void generaCollane(int *val, int len,int max_rip){
    int i = 0;
    int *sol;
    int *bestSol;
    int bestSolLen = 0;
    int bestSolVal = 0;
    int max_len = 0;
    for(i = 0; i < len; i++){
        max_len += val[i];
    }
    sol = malloc(max_len* sizeof(int));
    bestSol = malloc(max_len* sizeof(int));
    for(i = max_len; i > 0; i--){
        disp_rip(0, -1, val, len, sol, i, bestSol, &bestSolLen, &bestSolVal,max_rip);
    }
    for(i = 0; i < bestSolLen; i++){
        printf("%s", gemme[bestSol[i]]);
    }
    printf("\nLUNGHEZZA TROVATA: %d\nVALORE: %d",  bestSolLen, bestSolVal);
    free(sol);
    free(bestSol);
}

int disp_rip(int pos,int prec, int *disp_gemme, int len, int *sol, int k, int *bestSol, int *bestSolLen, int *bestVal,int max_rip){
    int i, j, count, curr_val;

    if(pos>=k){
        if((curr_val = verifica(sol, k)) != -1){
            if(curr_val > *bestVal){
                *bestVal = curr_val;
                *bestSolLen = k;
                for(i = 0; i < k; i++){
                    bestSol[i] = sol[i];
                }
            }
            return 1;
        }

    }

    for(i = 0; i < len; i++){
        if(prec != -1){
            if((sol[prec] == p_zaffiri ||  sol[prec] == p_topazi) && (i != p_zaffiri && i != p_rubini)) //caso incompatibile
                    continue;
            if((sol[prec] == p_smeraldi || sol[prec] == p_rubini) && (i != p_smeraldi && i != p_topazi))//caso incompatibile
                    continue;
            for(j = prec, count = 1; sol[j] == i && count < max_rip+1; j--, count++); //se una pietra è ripetuta più di max_rip volte, uso un'altra pietra
            if(count > max_rip) continue;
        }
        if(disp_gemme[i] > 0){
            disp_gemme[i]--;
            sol[pos] = i;
            if(disp_rip(pos+1,pos, disp_gemme, len, sol, k, bestSol, bestSolLen, bestVal,max_rip))
                return 1;
            disp_gemme[i]++;
        }
    }
    return 0;
}

int verifica(int *sol, int len){
    int *occ;
    int val_tot = 0;
    int i;
    occ = calloc(N_PIETRE, sizeof(int));
    for(i = 0; i < len; i++){
        occ[sol[i]]++;
    }
    for(i = 0; i < N_PIETRE; i++) val_tot += occ[i]*valori[i];
    if(occ[p_zaffiri] > occ[p_smeraldi]) val_tot = -1;
    free(occ);
    return val_tot;
}
