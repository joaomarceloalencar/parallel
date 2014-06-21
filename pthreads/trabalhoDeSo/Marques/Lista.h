#include "estruturas.h"

NO* NovoNo(int );
Lista* CriarLista();

NO* NoDeInicio(NO*,int);
void InserirNo(NO** ,NO*,NO*);

NO* Inserir(Lista*,int );
void InserirPosicaoNO(int,NO*);
void InserirNoInicio(Lista*,int );
void InserirNoFinal(Lista* ,int );
void ListarValores(Lista*);

void GuardarNO(Lista* ,NO* );

//não usados
int BuscarValor(Lista* , int );
void DevolveNo(NO* );
int RemoverValor(Lista* , int );
void DestroiLista(Lista* );
