#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>


int banner() {
    // ASCII-Art als String-Array
    const char* ascii_art[] = {
        "                                                                    ",
        ",-------.                     ,------.                   ,-------. ",
        "`--.   /  ,---. ,--.--. ,---. |  .-.  \\  ,--,--.,--. ,--.`--.   /  ",
        "  /   /  | .-. :|  .--'| .-. ||  |  \\  :' ,-.  | \\  '  /   /   /   ",
        " /   `--.\\   --.|  |   ' '-' '|  '--'  /\\ '-'  |  \\   '   /   `--. ",
        "`-------' `----'`--'    `---' `-------'  `--`--'.-'  /   `-------' ",
        "                                                `---'              "
    };

    
    int num_lines = sizeof(ascii_art) / sizeof(ascii_art[0]);

    
    for (int i = 0; i < num_lines; i++) {
        printf("%s\n", ascii_art[i]);
    }

    return 0;
}

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file); // Datei existiert, also schließen wir sie.
        return 1;     // Datei existiert
    }
    return 0;         // Datei existiert nicht
}

int charCounter(const char *filename){
    FILE *file;
     file = fopen(filename, "r");
     int overall = 0;
     char c;
    while (c = fgetc(file) != EOF /*EOF heißt end of file*/){
        printf("%c",c);    
        ++overall; 
    }
    printf("\nInsgesamt hat die Datei %d Buchstaben", overall);

    fclose(file);
    return 0;
}
    
int fileReader (const char *filename){
    FILE *file;
     file = fopen(filename, "r");
    char line[1000]; // Buffer für eine Zeile
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line); // Gibt die aktuelle Zeile aus
    }
    fclose(file);
}



int encrypt(const char *filename) {
    FILE *file ;
    file = fopen(filename, "r+"); // "r+" erlaubt Lesen und Schreiben
    fputwc(3 , file); 
    fclose(file); 
    return 0;    
}


  


int main() {
    const char *filename = "Rechnung.txt";
    banner();
    if (fileExists(filename)) {
        printf("Die Datei '%s' existiert. Verschl\x81sselung wird gestartet...", filename);
       
        char datei[1000];
        for (int i = 0 ; i <=2; ++i){
            printf("%c", '.');  
            fflush(stdout); 
            sleep(1);
        }
        charCounter(filename);
        sleep(1);
        fileReader(filename);
        sleep(3);
        encrypt(filename);
       
    } else {
        printf("Die Datei '%s' existiert nicht. Bitte \x81berpr\x81fen.", filename);
    }

    return 0;
}
