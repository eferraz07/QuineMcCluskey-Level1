#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Implicants{
    int position;
    char binary[4];
    bool dontcare;
    bool tick;
    struct Implicants *Next;
};

struct Implicants *First;
struct Implicants *Last;

struct Box{
    int boxNumber;
    struct Implicants *pImplicant;
    struct Box *Next;
};

struct Box *bFirst;
struct Box *bLast;

void listStart(){  //Start the values in the list.
    struct Implicants *aux;
    aux = (struct Implicants*)malloc(sizeof(struct Implicants));
    First = aux;
    Last = First;
    struct Box *auxBox;
    auxBox = (struct Box*)malloc(sizeof(struct Box));
    bFirst = auxBox;
    bLast   = bFirst;
}

int listInsert(int iposition, bool idontcare){  //Insert values in the list.
   if (First->Next == NULL ){
       listStart();
    }
    struct Implicants *aux;
    aux = (struct Implicants*)malloc(sizeof(struct Implicants));
    aux->position = iposition;
    itoa(aux->position,aux->binary,2);
    aux->dontcare = idontcare;
    Last->Next = aux;
    Last = Last->Next;
    aux->Next = NULL;
 }

 void listPrint(int amountVariables, int amountMinterms, int amountDontcares){
    struct Box *aux;
    aux = bFirst->Next;
    string outputPath;

    system("cls");

    cout << "Type a name for the output file: " << endl;
    cout << "(The extension .txt is automaticaly added) " << endl;
    cin >> outputPath;
    outputPath = outputPath + ".txt";
    ofstream outputFile(outputPath.c_str());

    system("cls");

    cout << "Amounts: " << endl;
    cout << "Variables: " << amountVariables << endl;
    cout << "Minterms: " << amountMinterms << endl;
    cout << "DontCares: " << amountDontcares << endl;
    cout << endl;
    cout << "Generated Implicants in Level 1: " << endl;

    outputFile << "Amounts: " << "\n";
    outputFile << "Variables: " << amountVariables << "\n";
    outputFile << "Minterms: " << amountMinterms << "\n";
    outputFile << "Dontcares: " << amountDontcares << "\n";
    outputFile << "\n";
    outputFile << "generated Implicants in Level 1: " << "\n";

    while(aux!=NULL){
        cout <<" Binary: " << aux->pImplicant->binary << " Box: " <<aux->boxNumber <<endl;
        outputFile <<" Binary: " << aux->pImplicant->binary << " Box: " <<aux->boxNumber << "\n";
        aux = aux ->Next;
    }
    cout << endl;
    cout << "The Implicants were also written in the specified file, which is created in the same path where the project is." <<endl;
    outputFile.close();
    free(outputFile);
 }

 void standardizesBinary(int amountVariables){  //Transforms the position number of an implicant in a binary number.

    struct Implicants *aux;
    aux = First->Next;

    while(aux!=NULL){
        for(int i = 0; i < amountVariables-1; i++){
            while(aux->binary[i+1] != '1' && aux->binary[i+1] != '0'){
               for (int j=i; j>=0; j--){
                    aux->binary[j+1] = aux->binary[j];
                    aux->binary[j] = '0';
                }
            }
        }
        aux = aux ->Next;
    }
 }

 void defineBox(int amountVariables){  // Defines the box for each implicant.

    struct Implicants *aux;
    aux = (struct Implicants*)malloc(sizeof(struct Implicants));

    aux = First->Next;

    struct Box *auxBox;

    bFirst = auxBox;
    bLast   = bFirst;

    int amountOnes = 0;

    while(aux!=NULL){
        if(aux->tick==false){
            for (int i = 0; i < amountVariables; i++){
                if (aux->binary[i] == '1'){
                    amountOnes++;
                }
            }
            auxBox = (struct Box*)malloc(sizeof(struct Box));

            auxBox->boxNumber = amountOnes;
            auxBox->pImplicant = aux;
            bLast->Next = auxBox;
            bLast = bLast->Next;
            auxBox->Next = NULL;
            auxBox = auxBox->Next;
        }

        amountOnes = 0;
        aux = aux->Next;
    }
 }

 void orderBox(){  //Orders the boxes for their amount of 1's.
    struct Box *aux;
    struct Box *auxNext;
    aux = bFirst->Next;
    auxNext = aux->Next;
    struct Implicants *auxImp;
    int auxBoxNumber = 0;

    while(aux!=NULL){
        auxNext = aux->Next;
        while(auxNext!=NULL){
            if (auxNext->boxNumber < aux->boxNumber){
                auxBoxNumber = aux->boxNumber;
                auxImp = aux->pImplicant;
                aux->boxNumber = auxNext->boxNumber;
                aux->pImplicant = auxNext->pImplicant;
                auxNext->boxNumber = auxBoxNumber;
                auxNext->pImplicant = auxImp;
            }
            auxNext = auxNext->Next;
        }
    aux = aux->Next;
    }
}

void insertNewImplicant(struct Implicants *p){  //Inserts a implicant in the list and checks if there is a repetition of that implicant.
    bool repetition = false;
    struct Implicants *aux;
    struct Implicants *auxValid;
    aux = (struct Implicants*)malloc(sizeof(struct Implicants));
    *aux = *p;
    auxValid = (struct Implicants*)malloc(sizeof(struct Implicants));
    auxValid = First->Next;
    while(auxValid!=NULL){
        if(strcmp(aux->binary,auxValid->binary) == 0){
            repetition = true;
            break;
        }
        auxValid = auxValid->Next;
    }
    if (repetition == false){
        aux->Next = NULL;
        Last->Next = aux;
        Last = Last->Next;
    }
}

void defineInicialGroup(int amountVariables){  //First group resolution of the Quine Method
    struct Box *aux;
    struct Box *auxNext;
    aux = (struct Box*)malloc(sizeof(struct Box));
    auxNext = (struct Box*)malloc(sizeof(struct Box));
    char vetorBinary[amountVariables];
    char vetorBinaryNext[amountVariables];
    char binaryUpdated[amountVariables];
    int amountDiference = 0;
    aux = bFirst->Next;

    while(aux!=NULL && aux->pImplicant->binary!=NULL){
        strcpy(vetorBinary,aux->pImplicant->binary);
        auxNext = aux->Next;

        while(auxNext!=NULL && auxNext->pImplicant->binary!=NULL){
            strcpy(vetorBinaryNext,auxNext->pImplicant->binary);
            amountDiference = 0;

            for(int i=0;i<amountVariables;i++){
                if(vetorBinary[i] == vetorBinaryNext[i] ){
                    binaryUpdated[i] = vetorBinary[i];
                }
                    else{
                        amountDiference++;
                        binaryUpdated[i] = 'X';
                    }
                    if(amountDiference>1){
                        break;
                    }
            }
            if (amountDiference == 1){
                aux->pImplicant->tick = true;
                auxNext->pImplicant->tick = true;
                struct Implicants *NewImplicant;
                NewImplicant = (struct Implicants*)malloc(sizeof(struct Implicants));
                strcpy(NewImplicant->binary,binaryUpdated);
                NewImplicant->tick = false;
                NewImplicant->dontcare = false;
                NewImplicant->Next = NULL;
                insertNewImplicant(NewImplicant);
            }
        auxNext = auxNext->Next;
        }
    aux = aux->Next;
    }

    defineBox(amountVariables);
    orderBox();
}

void defineFollowingGroups(int amountVariables){ //Method for the conclusion of the Quine Level 1.
    struct Box *aux;
    struct Box *auxNext;
    auxNext = (struct Box*)malloc(sizeof(struct Box));
    char vetorBinary[amountVariables];
    char vetorBinaryNext[amountVariables];
    char binaryUpdated[amountVariables];
    int amountDiference = 0;
    bool NewIteration = true;
    bool auxNewIteration;

    while(NewIteration == true){
        aux = (struct Box*)malloc(sizeof(struct Box));
        aux = bFirst->Next;
        auxNewIteration = false;

        while(aux!=NULL && aux->pImplicant->binary!=NULL){
            strcpy(vetorBinary,aux->pImplicant->binary);
            auxNext = aux->Next;

            while(auxNext!=NULL && auxNext->pImplicant->binary!=NULL){
                strcpy(vetorBinaryNext,auxNext->pImplicant->binary);
                amountDiference = 0;

                for(int i=0; i<amountVariables; i++){
                    if(vetorBinary[i] == vetorBinaryNext[i]){
                        binaryUpdated[i] = vetorBinary[i];
                    }
                    else{
                        amountDiference++;
                        binaryUpdated[i] = 'X';
                        }
                    if(amountDiference>1){
                        break;
                    }
                }

                if (amountDiference == 1){
                    aux->pImplicant->tick = true;
                    auxNext->pImplicant->tick = true;
                    struct Implicants *NewImplicant;
                    NewImplicant = (struct Implicants*)malloc(sizeof(struct Implicants));
                    strcpy(NewImplicant->binary,binaryUpdated);
                    NewImplicant->tick = false;
                    NewImplicant->dontcare = false;
                    NewImplicant->Next = NULL;
                    insertNewImplicant(NewImplicant);
                    auxNewIteration = true;
                }
                auxNext = auxNext->Next;
            }
        aux = aux->Next;
        }
        if(auxNewIteration == false){
            NewIteration = false;
        }else{
            defineBox(amountVariables);
            orderBox();
        }
    }
}

int main(){

    listStart();

    string inputPath;

    cout << "Type the input file path: " << endl;
    cout << "Exemple:C:\\Desktop\\information.txt" << endl;
    cin >> inputPath;
    ifstream inputFile(inputPath.c_str());

   while ( ! inputFile.is_open() ) {
      system("cls");
      cout <<"File not found, try to type it again: " << endl;
      cout << "Exemple:C:\\Desktop\\information.txt" << endl;
      cin >> inputPath;
      inputFile.open(inputPath.c_str());
   }

   int i = 0;
   int amountVariables = 0;
   int amountMinterms = 0;
   int amountDontcare = 0;
   int amountImplicants  = 0;

    if ((i == 0) && (!inputFile.eof())){
        inputFile >> amountVariables >> amountMinterms >> amountDontcare;

        cout << "Amounts: " << endl;
        cout << "Variables: " << amountVariables << endl;
        cout << "Minterms: " << amountMinterms << endl;
        cout << "DontCares: " << amountDontcare << endl;
        cout << endl;
        cout << "Generated Implicants in Level1: " << endl;
    }

    amountImplicants = amountMinterms + amountDontcare;
    int vetorImplicants[amountImplicants];

    while (!inputFile.eof()){

        string aux = "";

        getline(inputFile, aux);

        if(i != 0 && aux != ""){
           vetorImplicants[i] = atoi(aux.c_str());
        }

        i++;
    }

    int vetorAux = 0;

    for(int j=0; j<=amountImplicants; j++){
        for(int k=j+1; k<=amountImplicants; k++){
            if( vetorImplicants[j] > vetorImplicants[k] ){
                vetorAux = vetorImplicants[j];
                vetorImplicants[j] = vetorImplicants[k];
                vetorImplicants[k] = vetorAux;
            }
        }
    }

    for(int e = 0; e < amountImplicants; e++){

        if(vetorImplicants[e] <= amountMinterms){
            listInsert(vetorImplicants[e],false);
        }
        if(vetorImplicants[e] > amountMinterms){
           listInsert(vetorImplicants[e],true);
        }
    }

    standardizesBinary(amountVariables);
    defineBox(amountVariables);
    orderBox();
    defineInicialGroup(amountVariables);
    defineFollowingGroups(amountVariables);
    listPrint(amountVariables, amountMinterms, amountDontcare);
    inputFile.close();
    free(inputFile);
}
