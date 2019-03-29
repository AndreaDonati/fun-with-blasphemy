#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <spandsp.h>
#include <unistd.h>

/**
 * Struttura che identifica un nodo. Ogni nodo rappresenta uno stato della macchina
 */
typedef struct node {
    int numberOfPossibilities;              //il numero di archi uscenti dallo stato
    struct node** followings;               //vettore di puntatore agli stati successivi
    FILE* associatedFile;                   //puntatore al file contenente le parole associate allo stato
}node;

/**
 * @param file: puntatore al file contenente le parole associate allo stato.
 *              In generale, il file deve essere formattato con un numero come prima riga
 *              e nelle seguenti righe delle stringhe. Il numero in prima riga
 *              deve essere <= numero di stringhe nel file.
 * @return      una stringa estratta casualmente dal file.
 */
char* extractRandomWordFromFile(FILE* file);

/**
 * @param bottom: il limite minimo della generazione casuale
 * @param top: il limite massimo della generazione casuale
 * @return
 */
int generateRandomNumber(int bottom, int top);

/**
 *
 * Grammatica usata per produrre le bestemmie
 *
 * S,s santo
 * I,i insulto
 * V verbo
 * C
 *
 * S -> s
 * S -> sI
 * I -> iV
 * V -> C
 * C -> cV
 *
 * La grammatica è implementata con la corrispondente macchina a stati finiti non deterministica.
 */

int main() {

    int numeroProduzioni = 0;
    char *input = (char*) malloc(sizeof(int));
    //printf("Inserisci numero produzioni: ");
    scanf("%s",input);
    numeroProduzioni = atoi(input);

    int lunghezza = 0;
    input = (char*) malloc(sizeof(int));
    //printf("Inserisci Lunghezza: ");
    scanf("%s",input);
    lunghezza = atoi(input);

    //inizializzazione del risultato finale
    char *risultato[lunghezza];
    for (int j = 0; j < lunghezza; ++j) {
        risultato[j] = "";
    }


    /**
     * Inizializzazione:
     * inizializzo tutti gli stati con i loro valori
     */
    //definizione stati
    //----------------------------SANTI-------------------------------------
    node* saintM = malloc(sizeof(node));
    saintM->numberOfPossibilities = 1;
    saintM->followings = calloc((size_t) saintM->numberOfPossibilities, sizeof(node*));

    node* saintF = malloc(sizeof(node));
    saintF->numberOfPossibilities = 1;
    saintF->followings = calloc((size_t) saintF->numberOfPossibilities, sizeof(node*));

    //----------------------------INSULTI-------------------------------------
    node* insultM = malloc(sizeof(node));
    insultM->numberOfPossibilities = 4;
    insultM->followings = calloc((size_t) insultM->numberOfPossibilities, sizeof(node*));

    node* insultF = malloc(sizeof(node));
    insultF->numberOfPossibilities = 4;
    insultF->followings = calloc((size_t) insultF->numberOfPossibilities, sizeof(node*));

    //----------------------------VERBI-------------------------------------
    node* verbOgg = malloc(sizeof(node));
    verbOgg->numberOfPossibilities = 1; //s,sX
    verbOgg->followings = calloc((size_t) verbOgg->numberOfPossibilities, sizeof(node*));

    node* verbMezz = malloc(sizeof(node));
    verbMezz->numberOfPossibilities = 1; //s,sX
    verbMezz->followings = calloc((size_t) verbMezz->numberOfPossibilities, sizeof(node*));

    node* verbLuogo = malloc(sizeof(node));
    verbLuogo->numberOfPossibilities = 1; //s,sX
    verbLuogo->followings = calloc((size_t) verbLuogo->numberOfPossibilities, sizeof(node*));

    //----------------------------COMPLEMENTI-------------------------------------
    node* compOgg = malloc(sizeof(node));
    compOgg->numberOfPossibilities = 3; //s,sX
    compOgg->followings = calloc((size_t) compOgg->numberOfPossibilities, sizeof(node*));

    node* compOggPoiLuogo = malloc(sizeof(node));
    compOggPoiLuogo->numberOfPossibilities = 1; //s,sX
    compOggPoiLuogo->followings = calloc((size_t) compOggPoiLuogo->numberOfPossibilities, sizeof(node*));

    node* compMezz = malloc(sizeof(node));
    compMezz->numberOfPossibilities = 3; //s,sX
    compMezz->followings = calloc((size_t) compMezz->numberOfPossibilities, sizeof(node*));

    node* compSpec = malloc(sizeof(node));
    compSpec->numberOfPossibilities = 2; //s,sX
    compSpec->followings = calloc((size_t) compSpec->numberOfPossibilities, sizeof(node*));

    node* compLuogo = malloc(sizeof(node));
    compLuogo->numberOfPossibilities = 3; //s,sX
    compLuogo->followings = calloc((size_t) compLuogo->numberOfPossibilities, sizeof(node*));

    //----------------------------CONGIUNZIONI-------------------------------------
    node* congiunzioni = malloc(sizeof(node));
    congiunzioni->numberOfPossibilities = 2; //s,sX
    congiunzioni->followings = calloc((size_t) congiunzioni->numberOfPossibilities, sizeof(node*));


    //----------------------------STATO FINALE-------------------------------------
    node* finale = malloc(sizeof(node));
    finale->numberOfPossibilities = 0; //s,sX
    finale->followings = NULL;

    //definizione archi
    saintM->followings[0] = insultM;
    saintF->followings[0] = insultF;

    insultM->followings[0] = verbOgg;
    insultM->followings[1] = verbMezz;
    insultM->followings[2] = verbLuogo;
    insultM->followings[3] = finale;

    insultF->followings[0] = verbOgg;
    insultF->followings[1] = verbMezz;
    insultF->followings[2] = verbLuogo;
    insultF->followings[3] = finale;

    verbOgg->followings[0] = compOgg;
    verbMezz->followings[0] = compMezz;
    verbLuogo->followings[0] = compOggPoiLuogo;

    //compOgg->followings[0] = compMezz;
    //compOgg->followings[1] = verbOgg;
    //compOgg->followings[2] = verbMezz;
    compOgg->followings[0] = congiunzioni;
    compOgg->followings[1] = compSpec;
    //compOgg->followings[3] = insultM;
    //compOgg->followings[4] = insultF;
    compOgg->followings[2] = finale;

    compOggPoiLuogo->followings[0] = compLuogo;

    compMezz->followings[0] = congiunzioni;
    //compMezz->followings[0] = verbOgg;
    //compMezz->followings[1] = verbMezz;
    //compMezz->followings[2] = insultM;
    //compMezz->followings[3] = insultF;
    compMezz->followings[1] = finale;

    compSpec->followings[0] = congiunzioni;
    compSpec->followings[1] = finale;

    compLuogo->followings[0] = compSpec;
    compLuogo->followings[1] = congiunzioni;
    compLuogo->followings[2] = finale;

    congiunzioni->followings[0] = verbOgg;
    congiunzioni->followings[1] = verbMezz;


    //Apro tutti i file necessari alla produzione
    saintM->associatedFile = fopen("../santi/santiM.txt","r");
    saintF->associatedFile = fopen("../santi/santiF.txt","r");
    insultM->associatedFile = fopen("../insulti/insultiM.txt","r");
    insultF->associatedFile = fopen("../insulti/insultiF.txt","r");
    verbOgg->associatedFile = fopen("../verbi/verbi_C_oggetto.txt","r");
    verbMezz->associatedFile = fopen("../verbi/verbi_C_mezzo.txt","r");
    verbLuogo->associatedFile = fopen("../verbi/verbi_C_luogo.txt","r");
    compOgg->associatedFile = fopen("../complementi/C_oggetto.txt","r");
    compMezz->associatedFile = fopen("../complementi/C_mezzo.txt","r");
    congiunzioni->associatedFile = fopen("../congiunzioni/congiunzioni.txt","r");
    compSpec->associatedFile = fopen("../complementi/C_specificazione.txt","r");
    compLuogo->associatedFile = fopen("../complementi/C_luogo.txt","r");
    compOggPoiLuogo->associatedFile = compOgg->associatedFile;

    //Generazione
    node* curr = NULL;

    int randomic = generateRandomNumber(1,10);
    if(randomic %2 == 0)
        curr = saintM;
    else
        curr = saintF;

    //ripeto la generazione il numero desiderato di volte
    for (int k = 0; k < numeroProduzioni; ++k) {
        //genero produzione
        for (int i = 0; i < lunghezza && curr != finale && curr != NULL; i++) {

            risultato[i] = extractRandomWordFromFile(curr->associatedFile);

            if(curr == finale) break;
            randomic = generateRandomNumber(0,curr->numberOfPossibilities);
            if(curr != saintM && curr != saintF) {
                curr = curr->followings[randomic];
            }
            else {
                curr = curr->followings[0];
            }


        }

        //stampo generazione
        //printf("\nGenerazione: ");
        for (int l = 0; l < lunghezza; ++l) {
            if (strcmp(risultato[l], "") != 0)
                printf("%s ", risultato[l]);
        }
        printf(".\n");

        //pulisco il vettore generato
        for (int j = 0; j < lunghezza; ++j) {
            risultato[j] = "";
        }
        //vedo da dove ripartire
        randomic = generateRandomNumber(1,10);
        if(randomic %2 == 0){
            curr = saintM;
        }
        else{
            curr = saintF;
        }
    }

    //chiudo tutti i file aperti
    fclose(saintM->associatedFile);
    fclose(saintF->associatedFile);
    fclose(insultM->associatedFile);
    fclose(insultF->associatedFile);
    fclose(verbOgg->associatedFile);
    fclose(verbMezz->associatedFile);
    fclose(compOgg->associatedFile);
    fclose(compMezz->associatedFile);
    
    return 0;
}

int generateRandomNumber(int bottom, int top) {

    //alternativa per generare numeri casuali uniformemente distribuiti tra bottom e top
    //int range = top - bottom + 1;
    // Largest value that when multiplied by "range"
    // is less than or equal to RAND_MAX
    //int chunkSize = (RAND_MAX) / range;
    //int endOfLastChunk = chunkSize * range;
    //int r = rand();
    //while(r >= endOfLastChunk){
    //    r = rand();
    //}
    //return top + r / chunkSize;



    //nel seguente modo dovrei generare numeri casuali uniformemente distribuiti
    //nell'intervallo [bottom;top]
    srand((unsigned) time(NULL) + rand() + rand() + rand() + rand());
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = top - bottom;
    int myRand_scaled = (int) ((myRand * range) + bottom);
    return myRand_scaled;
//
    //un altro metodo di generazione numeri casuali
    //return rand() % top + bottom;
}

char* extractRandomWordFromFile(FILE* file){

    //torno all'inizio del file
    fseek(file,0,0);
    //variabile temporanea di lettura stringhe
    char* read = calloc(50, sizeof(char));
    //leggo il numero di elementi
    read = fgets(read,50,file);
    //lo trasformo in un intero
    int numberOfElements = atoi(read);
    //genero un numero casuale tra 0 e il numero di elementi nel file
    int randomic = generateRandomNumber(1,numberOfElements);
    //leggo randomic volte, così da leggere una parola casuale dal file
    char *extracted = calloc(50, sizeof(char));

    for (int i = 0; i < randomic; ++i)
        fgets(extracted,50,file);

    for (int j = 0; j < 50; ++j) {
        if(extracted[j] == '\n' || extracted[j] == '\r')
            extracted[j]='\0';
    }
    //debug
        //printf("\n%s ",extracted);
    free(read);
    return extracted;
}