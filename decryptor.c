#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_SIZE 1024
#define ALPHABET_SIZE 256 
#define MAX_CODES 10000   // max für vierstellige Codelänge (1000-9999)

// Substitutions Entschlüsselung
void substitute_decrypt(const char *input, int *rev_table, char *output) {
    char buffer[5]; // vierstellige Zahl + Nullterminierung(stoppt das Auslesen des Strings damit keine unnötigen Werte gelesen werden) 
    int code;
    int output_index = 0; // zählt die Position im entschlüsselten Text, um Zeichen korrekt zu speichern (sonst überschreibung)

    // geht durch die verschlüsselte Nachricht
    while (*input) {
        // liest genau 4 Zeichen ab und stoppt
        strncpy(buffer, input, 4);
        buffer[4] = '\0'; // Nullterminierung

        // macht den String in eine Ganzzahl (vierstellig -> integer)
        code = atoi(buffer);

        // nutzt die Umkehr Tabelle um den Code, zurück in ein Zeichen zu wandeln
        output[output_index++] = (char)rev_table[code];

        // geht auf die nächste vierställige Zahl
        input += 5; // 4 Zeichen + 1 Leerzeichen
    }
    output[output_index] = '\0'; // Nullterminierung
}

int delTrace() {
    const char *file = "rev_key.txt";
    const char *file1 = "PayUp.txt";
    const char *file2 = "substitution_table.txt";
    const char *file3 = "Rechnung_backup.txt";

    // Dateien löschen
    if (remove(file) == 0 && remove(file1) == 0 && remove(file2) == 0 && remove(file3) == 0) {
        printf("Dateien wurden erfolgreich gelöscht.\n");
    } else {
        printf("Fehler: Eine oder mehrere Dateien konnten nicht gelöscht werden.\n");
    }

    // Nachricht ausgeben und 3 Sekunden warten
    printf("Vielen Dank für Ihre Zusammenarbeit.\n");
    sleep(3);

    return 0;
}

// Entschlüsselung
int main() {
    int rev_table[MAX_CODES];
    char encrypted[MAX_TEXT_SIZE * 5]; // Erhöhte Größe weil Zeichen durch vierstellige Zahl + Öeerzeichen ersetzt wurde
    char decrypted[MAX_TEXT_SIZE];

    // öffnet die Umkehr Tabelle 'rev_key.txt'
    FILE *rev_key_file = fopen("rev_key.txt", "rb");
    if (rev_key_file == NULL) {
        perror("Fehler beim Öffnen der Umkehr-Schlüsseldatei");
        return 1;
    }
    fread(rev_table, sizeof(int), MAX_CODES, rev_key_file);
    fclose(rev_key_file);

    // öffnet 'Rechnung.txt'
    FILE *enc_file = fopen("Rechnung.txt", "rb");
    if (enc_file == NULL) {
        perror("Fehler beim Öffnen der verschlüsselten Datei");
        return 1;
    }

    // liest verschlüsselte Nachricht
    size_t len = fread(encrypted, sizeof(char), sizeof(encrypted), enc_file);
    fclose(enc_file);
    
    encrypted[len] = '\0'; // Nullterminierung

    // entschlüsselung
    substitute_decrypt(encrypted, rev_table, decrypted);

    // speichert die entschlüsselte Nachricht als neue 'Rechnung.txt'
    FILE *dec_file = fopen("Rechnung.txt", "wb");
    if (dec_file == NULL) {
        perror("Fehler beim Speichern der entschlüsselten Datei");
        return 1;
    }
    fwrite(decrypted, sizeof(char), strlen(decrypted), dec_file);
    fclose(dec_file);

    printf("Entschlüsselung abgeschlossen. Datei wiederhergestellt als 'Rechnung.txt'\n");
    delTrace();
    return 0;
}
