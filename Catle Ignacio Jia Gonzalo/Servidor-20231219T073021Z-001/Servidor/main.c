/* Protocol: el client envia un número i el servidor retorna el número multiplicat per 2 */
/* SERVIDOR */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>



/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MIDA_PAQUET 600
#define NUM_PALABRAS 20
#define INT_MAX 5
#define NUM_CLI 20

int main(int argc, char **argv){

    if (argc == 2)
    {
        int s;                              /* El socket */
        struct sockaddr_in socket_servidor; /* Socket on escolta el servidor */
        struct sockaddr_in adr_client;      /* Adreça i port des d'on el client envia el paquet */
        socklen_t adr_client_mida = sizeof(adr_client);          /* Longitud de les dades adreça i port */

        char paquet[MIDA_PAQUET]; /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);

        /* Posem les dades del socket del servidor */
        socket_servidor.sin_family = AF_INET;            /* Socket a Internet */
        socket_servidor.sin_addr.s_addr = INADDR_ANY;    /* Qualsevol NIC */
        socket_servidor.sin_port = htons(atoi(argv[1])); /* Port on estarà escoltant el servidor */

        /* Enllacem el socket */
        int resultat_bind;
        resultat_bind = bind(s, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor));

        if (resultat_bind < 0)
        {
            printf("No se ha podido enlazar el socket\n");
        }
        else
        {
            /* Servidor operatiu! */
            printf("Servidor operativo al puerto %d!\n", atoi(argv[1]));
            typedef struct{
            int id;
            int adivinado;
            int intentoActual;
            char resposta[6];
            char numero[6]; /* Per desar el número protocol */
            int indice_aleatorio;
            char palabraSecreta[6];
            }Client;

            int i;
            bool valido;
            int nclients=0;
            int num;
            char par[6];

            Client client[NUM_CLI];

            /*Tabla de palabras de cinco letras*/
            char palabras[20][6] = {    "michi", "gatos", "garra", "miaus", "pelos",
                                        "persa", "maine", "lykoi", "lince", "korat",
                                        "arena", "cazar", "colas", "jugar", "raton",
                                        "patas", "perro", "salto", "catle", "zarpa"};
            srand(time(NULL));
            for(i=0; i<NUM_CLI; i++){
                client[i].adivinado=0;
                client[i].intentoActual=1;
            }

            while (1)
            {
                memset(paquet, 0, sizeof(paquet));
                i=0;
                valido=false;
                printf("Esperando petición de algun cliente...\n");
                /* Quan es rep un paquet, a adr_client s'hi anota la IP i port des d'on s'envia */
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, &adr_client_mida);
                printf("Paquete recibido!\n");
                printf("%s", paquet);

                    /* Tractar la petició... */
                sscanf(paquet, "%s %d", par, &num);
                while(i<nclients && !valido){
                    if(client[i].id==num){
                        valido=true;
                    }
                    else{
                        i++;
                    }

                }
                if(!valido && nclients < NUM_CLI){
                    i=nclients;
                    client[i].id=num;
                    nclients++;
                    valido=true;
                }


                if(client[i].intentoActual <= INT_MAX && client[i].adivinado==0 && valido)
                {
                    if(client[i].intentoActual==1){
                            client[i].indice_aleatorio = rand() % NUM_PALABRAS;
                            strcpy(client[i].palabraSecreta, palabras[client[i].indice_aleatorio]);
                    }

                    /* Tractar la petició... */
                    sscanf(paquet, "%s %d", client[i].numero, &client[i].id);

                    printf(" Verificamos que %s sea la palabra secreta\n", client[i].numero);
                    sprintf(paquet, " %s\n", client[i].numero);
                    client[i].intentoActual++;
                    if (strcmp(client[i].numero, "exit!")==0)
                    {
                        sprintf(paquet, "¡Te has rendido! D: La palabra era: %s\n", client[i].palabraSecreta);
                        client[i].adivinado=2;
                    }
                    else
                    {
                        if (strcmp(client[i].numero, client[i].palabraSecreta) == 0){
                            sprintf(paquet, "\n⠀⠀⠀⡴⠚⢲⡀⠀⠀⠀⠀⠀⠀⢀⡶⠙⢶⡄⠀⠀\n⠀⠀⢸⡇⣰⠈⠁⠀⠑⠒⠒⠒⠒⠚⠘⠤⠸⣧⠀⠀\n⠀⢀⠞⠁⠀⡠⠤⡀⠀⠀⠀⠀⡠⠒⠐⣄⠀⠸⣧⠀\n⠀⡞⠀⠠⠃⠀⠀⠘⠆⠀⣀⠈⠁⠀⠀⠘⠀⠀⢹⡇\n⢸⡇⠀⠈⠉⠐⠂⠀⠀⠣⣤⠃⠀⠀⠘⠤⠄⠀⢸⡇\n⠘⣧⠀⠀⠄⠊⠀⠀⠀⠄⠜⠤⠂⠀⠓⠀⠀⠀⡼⠁\n⠀⠙⠷⠦⠤⠄⣀⣀⣠⡤⠤⠤⠤⠴⠶⠒⠚⠋⠁⠀⠀⠀⠀⠀⠀⠀\n¡Felicidades! Adivinaste la palabra: %s\n", client[i].palabraSecreta);
                            client[i].adivinado = 1;

                        }
                        else{
                            if(client[i].intentoActual <= INT_MAX){
                                int k;
                                int j;
                                char pSec[6];
                                strcpy(pSec, client[i].palabraSecreta);
                                strcpy(client[i].resposta, "XXXXX");
                                for(k=0;k<5;k++){
                                    if(client[i].numero[k]==pSec[k]){
                                        client[i].resposta[k]='B';
                                        pSec[k]='0';
                                    }
                                }
                                for(k=0;k<5;k++){
                                    for(j=0; j<5; j++){
                                        if(client[i].numero[k]==pSec[j]&&client[i].resposta[k]!='B'){
                                            client[i].resposta[k]='J';
                                            pSec[j]='0';
                                        }

                                    }

                                }

                            sprintf(paquet, "¡Mal! >:c %s\n",client[i].resposta);
                            }
                            else
                            {
                                sprintf(paquet, "\n⠀⢰⠲⢄⡀⠀⠀⠀⠀⠀⡏⠒⠤⡀⠀⠀⠀⠀⠀⠀\n⠀⠘⡄⣀⠙⣦⠀⠀⣀⣰⡣⢸⠢⡈⠢⡀⠀⠀⠀⠀\n⠀⠀⠸⡰⡰⠈⠉⠉⠀⠀⠀⠈⠑⠰⡀⠘⡄⠀⠀⠀\n⠀⠀⢸⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡀⠀⠀\n⠀⠀⡎⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⠀⠀\n⢀⣾⡀⢳⢤⣀⠤⠀⠀⠀⢠⣀⣀⡀⠔⠀⠀⣸⠀⠀\n⠘⢦⡀⡎⠀⡼⠁⠀⢻⠃⠀⠸⡄⠀⡅⠀⠈⢽⠀⠀\n⠀⠀⠙⠤⣰⠃⠀⠀⠁⡖⠒⢼⡀⠐⣄⠤⠋⠁⠀⠀\n⠀⠀⠀⠀⢀⡝⠒⡆⠀⠘⡄⠀⠻⣫⡀⠀⠀⠀⠀⠀\n¡Perdiste! La palabra era: %s\n", client[i].palabraSecreta);
                            }

                        }
                    }

                    /* Enviem el paquet a l'adreça i port on està esperant el client */
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
                    printf("Càlcul enviat!\n");

                    memset(paquet, 0, sizeof(paquet));
                    sprintf(paquet, "%d", client[i].adivinado);
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);

                    memset(paquet, 0, sizeof(paquet));
                    sprintf(paquet, "%d", client[i].intentoActual);
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);

                }
                if (!valido){
                    sprintf(paquet, "¡Servidor lleno! vuelva a intentarlo en 4 maullidos.");
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
                    sprintf(paquet, "%d", 3);
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
                }

                if(client[i].adivinado==0 && client[i].intentoActual > INT_MAX){
                    sprintf(paquet, "¡Perdiste! La palabra era: %s\n", client[i].palabraSecreta);
                    sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_client, adr_client_mida);
                }
            }
        }

        /* Tanquem el socket */
        close(s);
    }

    else
    {
        printf("El nombre de parámetros no es el correcto!\n");
    }
    return 0;
}
