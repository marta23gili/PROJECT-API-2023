#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct auto_{
  int autonomia;
  struct auto_ *next;
}auto_t;

typedef struct nodo_{
  int num;
  struct nodo_ *next;
} nodo_t;

typedef struct stazione_{
  int distanza;
  int val;
  int autonomia;
  struct stazione_ *next ;
  struct stazione_ *prec ;
}stazione_t;

typedef struct bstnodo_{
  int distanza;
  struct auto_ *parcheggio;
  struct bstnodo_ *padre, *dx, *sx;
}bstnodo_t;



auto_t* creaAuto(int);
stazione_t* creaStazione(int , int);
bstnodo_t* creaNodoTree(int, auto_t*);
int aggiungiAuto(auto_t**, int );
bstnodo_t* treeSearch(bstnodo_t **, int );
bstnodo_t* treeMinimum(bstnodo_t*);
int treeInsert(bstnodo_t**, int , auto_t*);
void transplant(bstnodo_t*, bstnodo_t* , bstnodo_t* );
int treeDelete(bstnodo_t **, int);
stazione_t* aggiungiStazioneInCoda(stazione_t* , int , int );
stazione_t* inorderTreeWalk(bstnodo_t**, stazione_t**);
int rottamaAuto(bstnodo_t**,int , int );
nodo_t* inserisciInTesta(nodo_t*, int );
nodo_t* pianificaPercorso(stazione_t**, nodo_t*, int, int );
nodo_t* pianificaPercorsoInverso(stazione_t**, nodo_t*, int, int );
void stampaPercorso(nodo_t*);
void deallocaPercorso(nodo_t**);
void deallocaLista(stazione_t** );
void deallocaAutostrada(bstnodo_t**);



int main(){
  FILE* fp=stdin;
  char comando[19];
  bstnodo_t  *tree = NULL;
  stazione_t *autostrada=NULL;
  bstnodo_t *tmp=NULL;
  auto_t* nuovoParcheggio=NULL;
  nodo_t *percorso=NULL;
  int a=0;
  int d=0;
  int n=0;
  int ok=0;

  while(fscanf(fp, "%s", comando)!=EOF){

    if(strcmp(comando, "aggiungi-stazione")==0){
      if(fscanf(fp,"%d %d", &d, &n)==2){
        for(int i=0; i<n; i++){
          if(fscanf(fp,"%d", &a)==1);
          ok=aggiungiAuto(&nuovoParcheggio, a);
        }

        ok=treeInsert(&tree, d, nuovoParcheggio);

        if(ok){
          printf("aggiunta\n");
        }else{
          printf("non aggiunta\n");
        }

      }


    }else if (strcmp(comando, "demolisci-stazione")==0){
      if(fscanf(fp,"%d", &d)==1);
      ok= treeDelete(&tree, d);
      if(ok){
        printf("demolita\n");
      }else{
        printf("non demolita\n");
      }


    }else if (strcmp(comando, "aggiungi-auto")==0){
      if(fscanf(fp,"%d %d", &d, &a)==2);

      tmp= treeSearch(&tree, d);


      if(tmp==NULL){
        printf("non aggiunta\n");
      }else{
        nuovoParcheggio=tmp->parcheggio;
        ok = aggiungiAuto(&nuovoParcheggio, a);
        if (ok){
          tmp->parcheggio=nuovoParcheggio;
          printf("aggiunta\n");
        }else {
          printf("non aggiunta\n");
        }
      }


    }else if (strcmp(comando, "rottama-auto")==0){
      if(fscanf(fp,"%d %d", &d, &a)==2)
        ok= rottamaAuto(&tree, d, a);

      if(ok){
        printf("rottamata\n");
      }else{
        printf("non rottamata\n");
      }


    }else if(strcmp(comando, "pianifica-percorso")==0) {
      if (fscanf(fp, "%d %d", &n, &a) == 2);
      if (n == a) {
        printf("%d\n", n);
      } else {
        autostrada = inorderTreeWalk(&tree, &autostrada);
        if (n < a) {
          percorso = pianificaPercorso(&autostrada, percorso, n, a);
        }else {
          percorso = pianificaPercorsoInverso(&autostrada, percorso, n, a);
        }

      }

      if(percorso!=NULL){
        stampaPercorso(percorso);
        deallocaPercorso(&percorso);

      }else{
        printf("nessun percorso\n");
      }
      deallocaLista(&autostrada);

    }else {
      printf("Input non valido. \n");
    }

    tmp=NULL;
    nuovoParcheggio = NULL;
    a=0;
    d=0;
    n=0;
    ok=0;
    percorso=NULL;
    autostrada=NULL;

  }

  deallocaAutostrada(&tree);

  return 0;
}




auto_t* creaAuto(int autonomia){
  auto_t *nuovaAuto = malloc(sizeof(auto_t));
  nuovaAuto->autonomia=autonomia;
  nuovaAuto->next=NULL;
  return nuovaAuto;
}

stazione_t* creaStazione(int distanza, int autonomia){
  stazione_t *nuovaStazione=NULL;
  nuovaStazione = malloc(sizeof(stazione_t));
  nuovaStazione->distanza=distanza;
  nuovaStazione->next=NULL;
  nuovaStazione->prec=NULL;
  nuovaStazione->val=0;
  nuovaStazione->autonomia=autonomia;
  return nuovaStazione;
}


bstnodo_t* creaNodoTree(int distanza, auto_t* automobili){
  bstnodo_t *new=malloc(sizeof(bstnodo_t));
  new->distanza = distanza;
  new->sx=NULL;
  new->dx=NULL;
  new->padre=NULL;
  new->parcheggio=automobili;
  return new;
}

int aggiungiAuto(auto_t** parcheggio, int autonomia){
  auto_t* nuovaAuto;
  auto_t* automobile=*parcheggio;
  auto_t* auto_corrente;
  int lung=0;
  for(; automobile; automobile=automobile->next, lung++);

  if(lung==512){
    return 0;
  }else{
    nuovaAuto = creaAuto(autonomia);
  }

  if (*parcheggio == NULL || autonomia >= (*parcheggio)->autonomia) {
    nuovaAuto->next = *parcheggio;
    *parcheggio = nuovaAuto;
    return 1;

  }else{
    auto_t* prec_auto = NULL;
    auto_corrente = *parcheggio;

    while (auto_corrente != NULL && auto_corrente -> autonomia > autonomia) {
      prec_auto = auto_corrente;
      auto_corrente = auto_corrente->next;
    }
    prec_auto->next = nuovaAuto;
    nuovaAuto ->next = auto_corrente;
  }

  return 1;
}



bstnodo_t* treeSearch(bstnodo_t **albero, int dist){
  bstnodo_t *tmp=*albero;
  while(tmp!=NULL && tmp->distanza != dist){
    if(dist < tmp->distanza){
      tmp=tmp->sx;
    }else{
      tmp=tmp->dx;
    }
  }
  return tmp;
}



bstnodo_t* treeMinimum(bstnodo_t* x){
  while(x->sx!=NULL){
    x=x->sx;
  }
  return x;
}



int treeInsert(bstnodo_t  **albero, int dist, auto_t* automobili){
  int ok=1;
  bstnodo_t *x=NULL, *y=NULL, *z=NULL;
  x=treeSearch(albero, dist);

  if(x==NULL){
    z=creaNodoTree(dist, automobili);
    x=*albero;
    while(x!=NULL){
      y=x;
      if(z->distanza<x->distanza){
        x=x->sx;
      }else{
        x=x->dx;
      }
    }
    z->padre=y;
    if(y==NULL){
      *albero=z;
    }else if(z->distanza  < y->distanza){
      y->sx=z;
    }else{
      y->dx=z;
    }
  }else{
    ok=0;
  }

  return ok;
}

void transplant(bstnodo_t *albero, bstnodo_t* u, bstnodo_t* v){
  if(u->padre==NULL){
    albero=v;
  }else if(u==u->padre->sx){
    u->padre->sx=v;
  }else{
    u->padre->dx=v;
  }
  if(v!=NULL){
    v->padre=u->padre;
  }
}



int treeDelete(bstnodo_t **albero, int distanza){
  int ok=0;
  bstnodo_t *tmp=*albero;
  bstnodo_t *z=NULL, *y=NULL;
  z=treeSearch(&tmp, distanza);

  if(z!=NULL){
    if(z->sx==NULL){
      transplant(*albero, z, z->dx);
    }else if(z->dx==NULL){
      transplant(*albero, z, z->sx);
    }else {
      y = treeMinimum(z->dx);
      if (y->padre != z) {
        transplant(*albero, y, y->dx);
        y->dx = z->dx;
        y->dx->padre = y;
      }
      transplant(*albero, z, y);
      y->sx = z->sx;
      y->sx->padre = y;
    }
    ok=1;
    auto_t *autoDaEliminare = z->parcheggio;
    while (autoDaEliminare != NULL) {
      auto_t *tmp = autoDaEliminare;
      autoDaEliminare = autoDaEliminare->next;
      free(tmp);
    }

    free(z);

  }else{
    ok=0;
  }
  return ok;
}

stazione_t* aggiungiStazioneInCoda(stazione_t* l, int d, int a){
  stazione_t *tmp, *prec;
  tmp = creaStazione(d,a);
  if(tmp != NULL){
    if(l == NULL)
      l = tmp;
    else{
      for(prec=l; prec->next!=NULL; prec=prec->next);
      prec->next = tmp;
      tmp->prec= prec;
    }
  } else
    printf("Mem. esaurita!\n");
  return l;

}

stazione_t* inorderTreeWalk(bstnodo_t **tree, stazione_t **lista){
  bstnodo_t *x=*tree;
  if(x!=NULL){
    inorderTreeWalk(&x->sx, lista);
    if(x->parcheggio==NULL){
      *lista=aggiungiStazioneInCoda(*lista, x->distanza, 0);
    }else{
      *lista=aggiungiStazioneInCoda(*lista, x->distanza, x->parcheggio->autonomia);
    }

    inorderTreeWalk(&x->dx, lista);
  }
  return *lista;
}

int rottamaAuto(bstnodo_t **albero,int d, int a) {
  bstnodo_t *stazione_corrente = *albero;
  auto_t* auto_corrente =NULL;
  auto_t* prec_auto = NULL;

  stazione_corrente= treeSearch(&stazione_corrente, d);

  if(stazione_corrente!=NULL) {
    auto_corrente = stazione_corrente->parcheggio;

    while (auto_corrente  && auto_corrente->autonomia != a) {
      prec_auto = auto_corrente;
      auto_corrente = auto_corrente->next;
    }

    if (auto_corrente != NULL) {
      if (prec_auto == NULL) {
        stazione_corrente->parcheggio = auto_corrente->next;
      } else {
        prec_auto->next = auto_corrente->next;
      }
      free(auto_corrente);
      return 1;
    }
  }

  return 0;
}


nodo_t* inserisciInTesta(nodo_t* l, int num){
  nodo_t *tmp;
  tmp = malloc(sizeof(nodo_t));
  if(tmp != NULL){
    tmp->num = num;
    tmp->next = l;
    l = tmp;
  } else
    printf("Mem. esaurita\n");
  return l;
}



nodo_t* pianificaPercorso(stazione_t** autostrada, nodo_t *percorso, int p, int a){
  int ok=0;
  stazione_t *arrivo=NULL, *partenza=NULL, *tmp=*autostrada;

  while (tmp && arrivo == NULL) {
    if (tmp->distanza == p) {
      partenza = tmp;
      tmp = tmp->next;
    } else if (tmp->distanza == a) {
      arrivo = tmp;
    } else {
      tmp = tmp->next;
    }
  }

  if (partenza != NULL && arrivo != NULL) {
    tmp = partenza;
    percorso = inserisciInTesta(percorso, a);

    while (!ok && (tmp->distanza!=arrivo->distanza)) {
      if ((arrivo->distanza) - (tmp->distanza) <= (tmp->autonomia)) {
        percorso = inserisciInTesta(percorso, tmp->distanza);
        if (tmp->distanza == partenza->distanza) {
          ok = 1;
        }
        arrivo = tmp;
        tmp = partenza;
      } else {
        tmp = tmp->next;
      }
    }
  }


  if(!ok){
    deallocaPercorso(&percorso);
    percorso=NULL;
  }

  return percorso;

}



nodo_t* pianificaPercorsoInverso(stazione_t** autostrada, nodo_t *percorso, int p, int a){
  stazione_t *arrivo=NULL, *partenza=NULL, *tmp=*autostrada, *curr=NULL;
  int i=1, can=0, exit=0;

  while (tmp && partenza == NULL) {
    if (tmp->distanza == p) {
      partenza = tmp;
    } else if (tmp->distanza == a) {
      arrivo = tmp;
      tmp = tmp->next;
    } else {
      tmp = tmp->next;
    }
  }

  tmp=partenza->prec;
  curr=partenza;
  if(curr->autonomia == 0){
    return NULL;
  }
  can=curr->distanza - curr->autonomia;

  if(can<0){
    percorso=inserisciInTesta(percorso, a);
    percorso=inserisciInTesta(percorso, p);
    return percorso;
  }

  while(arrivo->val==0){
    if(curr->distanza == a){
      while(tmp->distanza!=p){
        tmp->val=0;
        tmp=tmp->next;
      }
      return NULL;
    }

    while(!exit && tmp->distanza >= can){
      if(tmp->distanza == a){
        exit=1;
      }
      tmp->val=i;
      tmp=tmp->prec;
    }

    if(curr->prec->val == 0){
      while(tmp->distanza!=p){
        tmp->val=0;
        tmp=tmp->next;
      }
      return NULL;
    }

    curr=curr->prec;
    while(curr->autonomia == 0){
      if(curr->prec->val!=0){
        curr=curr->prec;
      }else{
        return NULL;
      }
    }
    i=(curr->val)+1;
    can=curr->distanza - curr->autonomia;


  }

  curr=arrivo;
  tmp=arrivo->next;
  percorso=inserisciInTesta(percorso, a);

  while(percorso->num!=p){
    if((tmp->val == curr->val - 1) && (curr->distanza >= tmp->distanza - tmp->autonomia)){
      percorso=inserisciInTesta(percorso, tmp->distanza);
      if(tmp->distanza!=p){
        curr=tmp;
        tmp=tmp->next;
      }
    }else{
      tmp=tmp->next;
    }
  }

  tmp=arrivo;
  while(tmp->distanza!=p){
    tmp->val=0;
    tmp=tmp->next;
  }
  return percorso;
}



void stampaPercorso(nodo_t *percorso){
  nodo_t *tmp=percorso;
  while(tmp->next != NULL){
    printf("%d ", tmp->num);
    tmp=tmp->next;
  }
  printf("%d", tmp->num);
  printf("\n");
}


void deallocaPercorso(nodo_t** l){
  nodo_t *tmp=NULL;
  while(*l){
    tmp=*l;
    *l=(*l)->next;
    free(tmp);
  }

}

void deallocaLista(stazione_t **l) {
  stazione_t *tmp;
  while (*l != NULL) {
    tmp = *l;
    *l =(*l)->next;
    free(tmp);
  }
}


void deallocaAutostrada(bstnodo_t** autostrada) {
  if (*autostrada == NULL) {
    return;
  }
  deallocaAutostrada(&((*autostrada)->sx));

  auto_t* auto_corrente = (*autostrada)->parcheggio;
  while (auto_corrente) {
    auto_t* temp_auto = auto_corrente;
    auto_corrente = auto_corrente->next;
    free(temp_auto);
  }
  free(*autostrada);

  deallocaAutostrada(&((*autostrada)->dx));
  *autostrada = NULL;
}

