#include <stdio.h>
#include <stdlib.h>
#define N 4
#define FILENAME "e2_test_set_PROG.txt"

int disp_ripet(int pos, int *dispGemme, int *sol, int n, int k, int prec);
int verifica(int *sol, int k);
char gemme[N] = {'Z', 'R', 'T', 'S'};
int main(void){
  int  i, s, k, numGemme[N], dispGemme[N], maxlun, bestlun, numtestset, min, max;
  //char gemme[N] = {'Z', 'R', 'T', 'S'};
  int *sol;
  int rip = 0;
  int *val_pietre,*npietre;
  int val_c, val_max, max_rip;
  val_max = 0;
  val_c = 0;
  FILE *in = fopen(FILENAME, "r");
  if (in == NULL)
    return 0;

  fscanf(in, "%d", &numtestset);
  /* INIZIO TEST*/
  for (s=0; s<numtestset; s++) {

    printf("Test set #%d\n", s+1);
    min = 0;
    maxlun = 0;

    val_pietre = malloc(N * sizeof(int));
    npietre = calloc(N,sizeof(int));
    for (i=0; i<N; i++) {// maxlun = z + s + t + r;
      fscanf(in, "%d", &numGemme[i]);
      maxlun += numGemme[i];
    }
    for(i = 0; i < N; i++)
        fscanf(in, "%d", &val_pietre[i]);
    fscanf(in,"%d",&max_rip);
    max = maxlun;
    sol = malloc((maxlun) * sizeof(int));
    printf("Prima del while...\n");
    printf("MAX_RIP : %d\n",max_rip);

    while (min <= max) {
        int rip = 0;
      k = min + (max-min)/2;
      for (i=0; i<N; i++)
        dispGemme[i]=numGemme[i];

      if (disp_ripet(0, dispGemme, sol, N, k, -1)) {

        if(val_c > val_max)
            val_c = val_max;
        bestlun = k;
        min = k+1;
    } else
        max = k-1;

    }
    printf("Una soluzione a lunghezza massima %d e' ",bestlun);
    for (i=0; i<bestlun; i++)
      printf("%c", gemme[sol[i]]);
    printf("\n");
    //exit(1);
    free(sol);

  }

  /*FINE DEL TEST*/

  fclose(in);
  return 0;
}

int controlla(int *sol, int n){
    int i;
    int n_z = 0, n_s = 0;
    int rip = 0;
    printf("SOL PARZIALE: \n");

    for(i = 1; i < n; i++){
        if(sol[i] == 0) n_z++;
        else if(sol[i] == 3) n_s++;
    }
    if(n_z <= n_s) return 1;
    return 0;
}
int disp_ripet(int pos, int *dispGemme, int *sol, int n, int k, int prec) {
  int i;
  if (pos >= k) {
    return 1;
  }

  for (i = 0; i < n; i++){ // i rappresenta la pietra che voglio posare

    if (dispGemme[i] > 0  ) { // se ci sono ancora gemme disponibili di quel tipo
      if (prec != -1) { // se ho già posato una pietra
        if (((sol[prec]==0) || (sol[prec]==2)) && (i!=0) && (i!=1)) //se in sol[prec] = Z o T e i != Z e R
          continue; // NON assegno la pietra i
        if (((sol[prec]==3) || (sol[prec]==1)) && (i!=2) && (i!=3))//se in sol[prec] = S o R e i != T e S
          continue; // NON assegno la pietra i
        //printf("PIETRA : %d PREC: %d rip: %d\n",i,sol[prec],rip);
      }
        /*POSO LA PIETRA se non supero gli if*/

          //printf("%d %d\n",rip,max_rip);
    dispGemme[i]--; //riduco la quantita della i-esima pietra
    sol[pos] = i; //assegno la pietra

    if (disp_ripet(pos+1, dispGemme, sol, n, k, pos)){ //ricorro
        return 1;
     }
     dispGemme[i]++; //backtrack


    }
  }
  return 0;
}
