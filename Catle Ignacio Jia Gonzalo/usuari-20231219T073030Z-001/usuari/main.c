/* Inclusio de fitxers .h habituals */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>



/* Inclusio de fitxers .h per als sockets */

#include <sys/socket.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <stdbool.h>

#define BUFFER_LEN 1024

void convertirMinusculas(char *cadena)
{
    int i;
    for (i = 0; cadena[i] != '\0'; i++)
    {
        if ((cadena[i] >= 'A' && cadena[i] <= 'Z') || cadena[i] == 'Ñ')
        {
            cadena[i] = cadena[i] + ('a' - 'A');
        }
    }
}


int main(int argc, char **argv){



if (argc != 2) {

    	printf("Us: %s IP\n", argv[0]);

    	exit(0);

}



int s;	/* Per treballar amb el socket */
int in=1;
int id;
int adivinado = 0;
struct sockaddr_in adr;

    char buffer[BUFFER_LEN];

    char p[7];

/* Volem socket d'internet i no orientat a la connexio */

    /*AF_INET6 per ipv6, SOCK_STREAM per a TCP*/

s = socket(AF_INET, SOCK_DGRAM, 0);



adr.sin_family = AF_INET;

    /*htons ens converteix el integer a format de xarxa*/

adr.sin_port = htons(44444);



/* L'adreca de comunicacio sera la IP del servidor, es a dir el parametre */

adr.sin_addr.s_addr = inet_addr(argv[1]);
    srand(time(NULL));
    id = rand() % 999999;
    printf("%s\n",
"                                                                                                    \n\
                                                                                                    \n\
              #############%.                                            ##############             \n\
             ####################/                                  %###################            \n\
             #####,,,,,,,,*#########%                            ##########,,,,,,,,######           \n\
             #####....,,,,,,,,,/#######.      *#%####%(.      ########.,,,,,,,,,...#####%           \n\
             #####........,,,,,,,,/################################.,,,,,,,,.......######           \n\
             #####(..........,,,#(#############(/,,,*/###############%.,,..........#####.           \n\
              #####.......,.#########(.                        (#########(......../#####            \n\
              ######.....########(                                  ########,....,#####             \n\
               %#####..#######                                         (#######.######.             \n\
                .###########                                              ###########               \n\
                  #######*                                                  #######.                \n\
                  ######                                                      ######                \n\
                 #####.                                                        %#####               \n\
                #####                                                           ######              \n\
               #####                                                             ######             \n\
              ######                                                              #####             \n\
              #####                                                               ######            \n\
              #####                                                               .#####            \n\
              #####         %%%(                                     ,&%%,        .#####            \n\
              #####             (%%%#%%%                     (%%%#%%%             *#####            \n\
              #####                        ,,,,............                       %####(            \n\
              .####                         ,,,,..........                        #####             \n\
                           %%%#%%%%%%%/       ,,,,.,....      *#%%%%%%#%%%                          \n\                                                                                          \n\
                                                                                                    \n\
                                                                                                    \n\
                                                                                                    \n\
                                                                                                    \n\
                                                                                                    \n\
          (((((((((((            (((((       ((((((((((((((((((   ((((            /((((((((((((((   \n\
      (((((((((((((((((        ((((((((      (((((((((((((((((   ((((             ((((((((((((((    \n\
    ,(((((                   ((((/ ((((            ((((         ((((/            (((((              \n\
   *((((                    ((((   ,((((          ,(((/         ((((             ((((((((((((((     \n\
   (((((                  /((((     ,(((.         ((((         ((((/            ,(((((((((((((      \n\
   (((((                .((((((((((((((((        ,(((/        .((((             ((((                \n\
    (((((((     (((((  (((((         ((((,       ((((         ,((((            ,((((                \n\
      (((((((((((((/ (((((            ((((      ((((/         ((((((((((((((   (((((((((((((((      \n");

printf("Bienvenido a 'Catle, 5 letters, 5 tries, 5 meows'!: X=Incorrecto, J=Posición incorrecta, B=Correcto\nSi quieres salir, escribe 'exit!'\n");

while(in<= 5 && adivinado == 0){
    printf("intento num: %d\n", in);

    /*Llegir dades*/

    printf("Introduce una palabra: ");

    scanf("%s", p);
    convertirMinusculas(p);

    /*Convertir char to array char*/

    sprintf(buffer, "%s %d",p, id);


    /*Enviar dades*/

    sendto(s, buffer, BUFFER_LEN, 0, (struct sockaddr*)&adr, sizeof(adr));

    printf("Paquete enviado, miau! -> %s\n",buffer);


    /*Esperar resposta*/

    printf("Esperando resultado del servidor!\n");

    recvfrom(s, buffer, BUFFER_LEN, 0, NULL, NULL);

    /*Escriure resposta*/

    printf("El servidor responde: %s\n", buffer);

    recvfrom(s, buffer, BUFFER_LEN, 0, NULL, NULL);
    adivinado = atoi(buffer);

    recvfrom(s, buffer, BUFFER_LEN, 0, NULL, NULL);
    in = atoi(buffer);
    }

    /* Tanquem el socket */

    close(s);

    return 0;

}
