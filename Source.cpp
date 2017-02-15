/*
 
 Práctica 2:
 Al ladrillo con excavadora y grúa.
 
 Autores:
 Facundo Schiavoni Barreyro
 Maksym Kharuk
 
 Curso:
 2016/2017
 
 Grupo:
 E
 
 */


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_SIZE = 25;
const int CENTINELA = -1;
typedef int tArray[MAX_SIZE];


int menu(); // Imprime el menú y pide al usuario la opción a ejecutar.
ifstream openFile(); // Pide al usuario el archivo que desea leer.
void readRow(ifstream &file, tArray row, int &size); // Lee la fila y la guarda en el array.
void printRow(tArray row, int &size); // Imprime la fila.
ofstream createFile(); //Pide al usuario el archivo donde guardar la fila.
void writeRowFile(ofstream outFile, const tArray row, int size); // Escribe la fila en el archivo.
bool isValidaPos(int size, int pos);  // Comprueba si una posicion es válida.
bool isEmpty(const tArray row, int pos); // Comprueba si una posicion esta vacía (0) o no.
bool isPossibleCrane1Between(const tArray row, int size, int initialPos, int dropPos); //Comprueba si es posible usar la grúa entre dos posiciones dadas (1 elemento).
bool crane1Element(tArray row, int size, int initialPos, int dropPos); // Si es posible usar la grua, la ejecuta (1 elemento).
int posFreeSpaceRight(const tArray row, int size, int initialPos); // Devuelve la posición del primer hueco libre de la fila hacia la derecha.
bool excavator1Right(tArray row, int size, int initialPos); // Usa la excavadora una posición a la derecha.
int posFreeSpaceDir(const tArray row, int size, int intialPos, int direction); // Devuelve la posición del primer hueco libre de la fila a izquierda o derecha.
bool excavator1Dir(tArray row, int size, int intialPos, int direction); // Usa la excavadora una posición a la izquierda o derecha.
bool excavator(tArray  row, int size, int initialPos, int movesAmount, int direction); // Usar excavadora.
void executeReadFile(tArray row, int &size); // Ejecuta la función para leer la fila del archivo.
void executeSaveFile(const tArray row, int size); // Ejecuta la función para guardar la fila en el archivo.
void executeCrane(tArray row, int size); // Ejecuta la función para usar la grúa.
void executeExcavator(tArray row, int size); // Ejecuta la función para usar la excavadora.
bool isCranePossiblePosition(int size, int initialPosition, int finalPosition, int dropPosition); // Comprueba si las posiciones de los elementos que queremos levantar con la grúa son válidas.
bool isCranePossible(const tArray row, int size, int initialPosition, int finalPosition, int dropPosition); // Comprueba si las posiciones donde queremos dejar los elementos con la grúa son válidas.
bool crane(tArray row, int size, int initialPosition, int finalPosition, int dropPosition); // Usar grúa.
bool orderedRow(const tArray row, int size); // Comprueba si la fila esta ordenada o no.
void checkOrder(const tArray row, int size, int moves); // Escribe en pantalla si la fila esta ordenada o no.

int menu() {
    
    int selectedOption;
    
    do {
        
        cout << " ################################" << endl
        << " # Select option:               #" << endl
        << " #  1 - Load row from file      #" << endl
        << " #  2 - Save row in file        #" << endl
        << " #  3 - Use crane               #" << endl
        << " #  4 - Use excavator           #" << endl
        << " #  0 - Exit                    #" << endl
        << " ################################" << endl;
        
        cin >> selectedOption;
        
    } while (selectedOption < 0 || selectedOption > 4);
    
    return selectedOption;
}

ifstream openFile() {
    
    string fileName;
    
    cout << "-> Enter file name:" << endl;
    cin >> fileName;
    
    ifstream in(fileName); // Input file stream
    
    if (!in.is_open()) {
        cout << "-> That file doesn't exist!" << endl;
    }
    
    return in;
}

void readRow(ifstream &file, tArray row, int &size) {
    
    int fileValue = 0;
    
    for (int i = 0; fileValue != -1; i++) {
        
        file >> fileValue;
        
        if (fileValue != -1) {
            row[i] = fileValue;
        }
        
        size = i;
    }
}

void printRow(tArray row, int &size) {
    
    for (int i = 0; i < size; i++) { // Print row values
        cout << "| " << row[i];
    }
    
    cout << "|" << endl;
    
    for (int i = 0; i < size; i++) { // Print separator line
        cout << "---";
    }
    
    cout << "-" << endl;
    
    for (int i = 0; i < size; i++) { // Print row positions
        
        if (i < 10) {
            cout << "| " << i;
        }
        
        if (i > 9) {
            cout << "|" << i;
        }
    }
    
    cout << "|" << endl;
}

ofstream createFile() {
    
    string fileName;
    
    cout << "-> Enter file name to save the row:" << endl;
    cin >> fileName;
    
    ofstream out(fileName);
    
    return out;
}

void writeRowFile(ofstream outFile, const tArray row, int size) {
    
    for (int i = 0; i < size; i++) {
        
        outFile << row[i] << " ";
    }
    
    outFile << CENTINELA;
    
    outFile.close();
    
    cout << "-> File saved." << endl;
}

bool isValidPos(int size, int pos) {
    
    return pos < size && pos >= 0;
}

bool isEmpty(const tArray row, int pos) {
    
    return row[pos] == 0;
}

bool isPossibleCrane1Between(const tArray row, int size, int initialPos, int dropPos) {
    
    return isValidPos(size, initialPos) && !isEmpty(row, initialPos) // Check initial pos
    && isValidPos(size, dropPos) && isEmpty(row, dropPos); // Check drop pos
}

bool crane1Element(tArray row, int size, int initialPos, int dropPos) {
    
    if (isPossibleCrane1Between(row, size, initialPos, dropPos)) {
        
        row[dropPos] = row[initialPos];
        row[initialPos] = 0;
        
        return true;
    }
    else {
        return false;
    }
}

int posFreeSpaceRight(const tArray row, int size, int initialPos) {
    
    for (int i = initialPos + 1; i <= size; i++) {
        
        if (row[i] == 0) {
            return i;
        }
    }
    
    return size;
}

bool excavator1Right(tArray row, int size, int initialPos) {
    
    int posFreeSpace = posFreeSpaceRight(row, size, initialPos);
    
    if (posFreeSpace == size) { // Comprovamos si no existe hueco libre
        return false;
    }
    
    for (int i = posFreeSpace; i > initialPos; i--) {
        
        row[i] = row[i - 1]; // Movemos los materiales empezando por el final
    }
    
    row[initialPos + 1] = 0; // Dejamos libre la primera posicion
    
    return true;
}

int posFreeSpaceDir(const tArray row, int size, int initialPos, int direction){
    
    for (int i = initialPos + direction; (direction == 1 && i <= size) || (direction == -1 && i >= 0); i = i + direction) {
        
        if (row[i] == 0) {
            
            return i;
        }
    }
    
    return size;
}

bool excavator1Dir(tArray row, int size, int initialPos, int direction) {
    
    int posFreeSpace = posFreeSpaceDir(row, size, initialPos, direction);
    
    if (posFreeSpace == size){
        return false;
    }
    
    for (int i = posFreeSpace; (direction == 1 && i > initialPos) || (direction == -1 && i < initialPos); i = i - direction) {
        
        row[i] = row[i - direction]; // Movemos los materiales empezando por el final
    }
    
    row[initialPos + direction] = 0; // Dejamos libre la primera posicion
    
    return true;
    
}

bool excavator(tArray row, int size, int initialPos, int movesAmount, int direction) {
    
    if (initialPos < -1 || initialPos > size + 1) {
        return false;
    }
    
    for (int k = 0; k < movesAmount; k++) {
        
        int posFreeSpace = posFreeSpaceDir(row, size, initialPos, direction);
        
        if (posFreeSpace == size){
            return false;
        }
        
        for (int i = posFreeSpace; (direction == 1 && i > initialPos) || (direction == -1 && i < initialPos); i = i - direction) {
            
            row[i] = row[i - direction]; // Movemos los materiales empezando por el final
        }
        
        row[initialPos + direction] = 0; // Dejamos libre la primera posicion
        
        initialPos += direction; // Sumamos o restamos 1 para ir empujando los materiales a derecha o izquierda.
    }
    
    return true;
}

void executeReadFile(tArray row, int &size){
    ifstream file = openFile();
    
    if (file.is_open()) {
        readRow(file, row, size);
    }
}
void executeSaveFile(const tArray row, int size){
    writeRowFile(createFile(), row, size);
}

void executeCrane(tArray row, int size, int &moves){
    
    int initialPos, finalPos, dropPos;
    
    cout << "-> Write the initial position: ";
    cin >> initialPos;
    cout << "-> Write the final position: ";
    cin >> finalPos;
    
    
    cout << "-> Write the drop position: ";
    cin >> dropPos;
    
    if (crane(row, size, initialPos, finalPos, dropPos)) {
        moves++;
    }
    else {
        cout << "-> ERROR!" << endl;
    }
}

void executeExcavator(tArray row, int size, int &moves) {
    
    int initialPos, direction, movesAmount;
    
    cout << "-> Write the initial position: ";
    cin >> initialPos;
    
    cout << "-> Write excavator direction(1: right, -1: left): ";
    cin >> direction;
    
    cout << "-> Write the moves amount: ";
    cin >> movesAmount;
    
    if (excavator(row, size, initialPos, movesAmount, direction)){
        moves++;
    }
}

bool isCranePossiblePosition(int size, int initialPosition, int finalPosition, int dropPosition){
    
    return initialPosition >= 0 && initialPosition <= finalPosition && finalPosition <= size // Comprovamos extremos
    && dropPosition + (finalPosition - initialPosition) < size; // Comprovamos posicion de caida.
}

bool isCranePossible(const tArray row, int size, int initialPosition, int finalPosition, int dropPosition){
    
    if (!isCranePossiblePosition(size, initialPosition, finalPosition, dropPosition)){
        return false;
    }
    
    for (int i = initialPosition; i <= finalPosition; i++){
        
        if (row[i] != 0 && row[dropPosition] != 0
            && !(dropPosition >= initialPosition && dropPosition <= finalPosition)){
            
            return false;
        }
        
        dropPosition++;
    }
    
    return true;
}

bool crane(tArray row, int size, int initialPosition, int finalPosition, int dropPosition){
    
    if (!isCranePossible(row, size, initialPosition, finalPosition, dropPosition)){
        return false;
    }
    
    tArray dropElements;
    
    copy(row + initialPosition, row + finalPosition + 1, dropElements);
    
    for (int i = initialPosition; i <= finalPosition; i++) {
        row[i] = 0;
        
    }
    
    for (int i = 0; i <= finalPosition - initialPosition; i++) {
        if (dropElements[i] != 0){
            row[dropPosition + i] = dropElements[i];
        }
    }
    
    return true;
}

bool orderedRow(const tArray row, int size){
    
    int num = 0;
    
    for (int i = 0; i < size; i++){
        
        if (row[i] != 0){
            
            if (num < row[i]){
                num = row[i];
            }
            else {
                return false;
            }
        }
    }
    
    return true;
}

void checkOrder(const tArray row, int size, int moves) {
    
    cout << "-> Number of movements: " << moves << endl;
    
    if (orderedRow(row, size)) {
        cout << "-> ORDERED ROW!" << endl;
    }
    else {
        cout << "-> UNORDERED ROW" << endl;
    }
}

int main() {
    
    int size = 0, moves = 0;
    tArray row;
    bool exit = false;
    
    while (!exit) {
        
        switch (menu())
        {
            case 1:
                executeReadFile(row, size);
                moves = 0;
                break;
            case 2:
                executeSaveFile(row, size);
                break;
            case 3:
                executeCrane(row, size, moves);
                break;
            case 4:
                executeExcavator(row, size, moves);
                break;
            default:
                exit = true;
                break;
        }
        
        // Print row and check order
        if (size > 0) {
            printRow(row, size);
            checkOrder(row, size, moves);
        }
        else {
            cout << "No hay fila!" << endl;
        }
    }
    
    system("pause");
    
    return 0;
}
