#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <modbus.h>

//#define TCP_PORT_MODBUS 1502
#define TCP_PORT_MODBUS 502
#define DEBUG_MODBUS FALSE

// definit le format d'affichage des mots
#define STRMOTS "MW%d=%d\n"
#define MAX_MOTS_LUS 123


#define RIEN -100
#define IBASE 0x100
#define IBASERESOLUTION 0x1800
#define VOIE1 0
#define VOIE2 4
#define VOIE3 8
#define VOIE4 12

uint16_t tab_reg[MAX_MOTS_LUS];
typedef struct mots {
    float fval;
} t_mots ;
t_mots amots[256];
int aiResolution [100];


/*------------------------------------------------------------------
  * printf des mots lus
  * retourne le nombre de mots lus ou -1 si erreur
  * parametres : ip automate, mot de départ , nombre de mots a lire
-----------------------------------------------------------------*/
int lit_mots_automate ( char * szIp, short int start , short int count)
{
  int rc;
  int i;
  modbus_t *ctx;
  float fval;
  struct timeval response_timeout;

  ctx = modbus_new_tcp(szIp, TCP_PORT_MODBUS);
  if (ctx == NULL) {
       fprintf(stderr, "Unable to allocate libmodbus context\n");
       return -1;
  }
  modbus_set_slave(ctx, 255);
  modbus_set_debug(ctx, DEBUG_MODBUS);
  modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

  /* Define a 1 sec timeout */
  response_timeout.tv_sec = 1;
  response_timeout.tv_usec = 0;
  modbus_set_response_timeout(ctx, &response_timeout);

  if (modbus_connect(ctx) == -1) {
       fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
       modbus_free(ctx);
       return -2;
  }
  if (count > MAX_MOTS_LUS) {
      count = MAX_MOTS_LUS;
  }

  /* Read count registers from the address start */
  rc = modbus_read_registers(ctx, start, count, tab_reg);
  if (rc == -1 && errno == EMBXILADD) {
        fprintf(stderr,"FAILED %d %d %s \n",rc,errno,modbus_strerror(errno));
        return -3;
   }

  if (rc == -1 ) {
        fprintf(stderr,"OK %d %d %s \n",rc,errno,modbus_strerror(errno));
        return -4;
   }


  modbus_close(ctx);
  modbus_free(ctx);

  return rc;
}



/*------------------------------------------------------------------
------------------------------------------------------------------*/
int lit_resolution_voie (char * pIp ,int iVoie)
{
int i=0;
int rc=0;
int start = IBASERESOLUTION + (iVoie/4) * 0x80;

rc=lit_mots_automate (pIp,IBASERESOLUTION,10);
if (rc>0) {
   i = tab_reg[1] ;
   /* fprintf(stderr,"%s %d %d \n",pIp,iVoie, i); */
   return i;
}
 return -1;
}



/*------------------------------------------------------------------
------------------------------------------------------------------*/
void lit_mots_nanodac (char * pIp )
{
int i=0;
int rc=0;
  time_t t;
  char szt[20];

    aiResolution[VOIE1] = lit_resolution_voie(pIp,VOIE1);
    aiResolution[VOIE2] = lit_resolution_voie(pIp,VOIE2);
    aiResolution[VOIE3] = lit_resolution_voie(pIp,VOIE3);
    aiResolution[VOIE4] = lit_resolution_voie(pIp,VOIE4);

memset( amots,0,sizeof(amots));
rc=lit_mots_automate (pIp,IBASE,40);
if (rc>0) {
   for (i=0;i < rc;i++) {
           amots[i].fval = tab_reg[i] ;
   }
}
}

/*------------------------------------------------------------------
genere la chaine qui sera importe dans la table meca_datalog
2019-10-09 18:01:00;;M707003;19.60;78.80;
date;;machine;temperature;hygrometrie;
------------------------------------------------------------------*/
void print_TH(char * pMachine,int iT ,int iHg)
{
float f1;
float f2;
float f;
int n;
int i;

time_t t;
char sztime[20];

 t = time (NULL);
 strftime (sztime,20,"%Y-%m-%d %H:%M:00",localtime(&t));

 if (iT == RIEN && iHg == RIEN) {
      return;
 }
 if (iT != RIEN) {
     n = aiResolution[iT];
     f = 1.0;
     for (i=0; i < n ;i++) {
         f = f * 10.0;
     }
     f1 = amots[iT].fval / f;
 }
 if (iHg != RIEN) {
     n = aiResolution[iHg];
     f = 1.0;
     for (i=0; i < n ;i++) {
         f = f * 10.0;
     }
     f2 = amots[iHg].fval / f;
 }
 if (iT != RIEN && iHg != RIEN) {
     printf ("%s;;%s;%5.2f;%5.2f;\n",sztime,pMachine,f1,f2);
 } else if (iHg == RIEN) {
           if (pMachine == "M909018") {
             printf ("%s;;%s;%5.2f;\n",sztime,pMachine, (f1/150)*-1);
         }else {
             printf ("%s;;%s;%5.2f;\n",sztime,pMachine,f1);
              }
     /*printf ("%s;;%s;%5.2f;;\n",sztime,pMachine,f1);*/
 } else {
     printf ("%s;;%s;;%5.2f;\n",sztime,pMachine,f2);
 }
}
/*------------------------------------------------------------------
 Lit les nanodacs et imprime la sortie export vers meca_datalog
------------------------------------------------------------------*/
void print_nanodacs()
{

 // Cabine atelier 155
 lit_mots_nanodac ("10.101.0.11");
 print_TH ( "M155002",  VOIE1, VOIE3);
 print_TH ( "M155009",  VOIE2, RIEN);

 // Etuve PS1
 lit_mots_nanodac ("10.101.0.12");
 print_TH ( "M154011",  VOIE1, RIEN);

 // Desolvatation 254
 lit_mots_nanodac ("10.101.0.13");
 print_TH ( "M254010",  VOIE1, RIEN);

 //  Etuve A&B 254
 lit_mots_nanodac ("10.101.0.14");
 print_TH ( "M254038",  VOIE4, RIEN);

 // ETUVE C&D 254
 lit_mots_nanodac ("10.101.0.15");
 print_TH ( "M254037",  VOIE4, RIEN);

 // Etuve G 254
 lit_mots_nanodac ("10.101.0.16");
 print_TH ( "M254039",  VOIE4, RIEN);

 // Cabine peinture Speedshop 707
 lit_mots_nanodac ("10.101.0.17");
 print_TH ( "M707003",  VOIE1, VOIE3);
 print_TH ( "M707010",  VOIE2, RIEN);

 // Cabine peinture Cabine F
 lit_mots_nanodac ("10.101.0.18");
 print_TH ( "M254306",  VOIE1, VOIE2);
// print_TH ( "M255002",  RIEN, VOIE4);

 // Etuve Grande Site9
 lit_mots_nanodac ("10.9.12.1");
 print_TH ( "M909014", VOIE1, RIEN);
 print_TH ( "M909015", VOIE4, VOIE3);
 print_TH ( "M909018", VOIE2 , RIEN);

 // Etuve Petite Site9
 lit_mots_nanodac ("10.9.12.2");
 print_TH ( "M909016", VOIE1, RIEN);
}

/*------------------------------------------------------------------
  * printf des mots lus
  ------------------------------------------------------------------*/

void main ()
{
int i=0;

print_nanodacs ();

}
