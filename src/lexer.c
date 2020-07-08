#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include "lexer.h"

FILE *fp;
lexeme_t next_lex;
char_t next_ch;
list_lexeme_t *lex_list;

void read();
void skip_space();
void add();
int lex();
int lookup(char ch);
list_lexeme_t *list_lexeme_t_create(size_t s);
void list_lexeme_t_add(list_lexeme_t *list, lexeme_t item);
void list_lexeme_t_enlarge(list_lexeme_t *list);
void run_lexer(char *file_name);

void run_lexer(char *file_name) {
    fp = fopen(file_name, "r");
    lex_list = list_lexeme_t_create(50);
    if (fp != NULL) {
        read();
        do {
            lex();
        } while (next_lex.token != EOF);
    }
    else printf("HATA: %s DOSYASI ACILAMADI!\n", file_name);
    fclose(fp);
}

void read() {
    next_ch.ch = fgetc(fp);

    if (next_ch.ch != EOF)
        if (isalpha(next_ch.ch))
            next_ch.type = HARF;
        else if (isdigit(next_ch.ch))
            next_ch.type = RAKAM;
        else
            next_ch.type = BILINMEYEN;
    else
        next_ch.type = EOF;
}

void skip_space() {
    while (isspace(next_ch.ch))
        read();
}

void add() {
    if (next_lex.length < MAX_LEX_SIZE - 1) {
        *(next_lex.lexeme + next_lex.length) = next_ch.ch;
        next_lex.length += 1;
        *(next_lex.lexeme + next_lex.length) = '\0';
    } else {
        printf("HATA: MAKSIMUM KARAKTER SAYISI ASILDI!");
        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
        getch();
        exit(1);
    }
}

int lex() {
    next_lex.length = 0;
    skip_space();
    switch (next_ch.type) {
        case HARF:
            if (next_ch.ch == 'b' ||
                next_ch.ch == 'c' ||
                next_ch.ch == 'd' ||
                next_ch.ch == 'e' ||
                next_ch.ch == 'i' ||
                next_ch.ch == 'k' ||
                next_ch.ch == 'm' ||
                next_ch.ch == 'o' ||
                next_ch.ch == 't' ||
                next_ch.ch == 'y') {
                lookup(next_ch.ch);
                break;
            }
            add();
            read();
            while (next_ch.type == HARF || next_ch.type == RAKAM) {
                add();
                read();
            }
            next_lex.token = ISIM;
            break;
        case RAKAM:
            add();
            read();
            while (next_ch.type == RAKAM) {
                add();
                read();
            }
            if (next_ch.ch == '.') {
                add();
                read();
                size_t count = 0;
                while (next_ch.type == RAKAM) {
                    add();
                    read();
                    ++count;
                }
                if (count == 0) {
                    printf("HATA: ONDALIK SAYININ ONDALIK KISMINI YAZMADINIZ.\n");
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                } else next_lex.token = ONDALIK_SABIT;
                break;
            } else
                next_lex.token = TAMSAYI_SABIT;
            break;
        case BILINMEYEN:
            lookup(next_ch.ch);
            break;
        case EOF:
            next_lex.token = EOF;
            strcpy(next_lex.lexeme, "DOSYA SONU");
            break;
    }
    list_lexeme_t_add(lex_list, next_lex);
    return next_lex.token;
}

int lookup(char ch) {
    switch (ch) {
        case '"':
            read();
            while (isprint(next_ch.ch) && next_ch.ch != '"') {
                add();
                read();
            }
            read();
            next_lex.token = YAZI_SABIT;

            if (next_lex.length == 0)
                strcpy(next_lex.lexeme, "");

            break;
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'i':
        case 'k':
        case 'm':
        case 'o':
        case 't':
        case 'y':
            add();
            read();
            while (next_ch.type == HARF || next_ch.type == RAKAM) {
                add();
                read();
            }
            if (strcmp(next_lex.lexeme, "dogru") == 0)
                next_lex.token = MANTIKSAL_SABIT;
            else if (strcmp(next_lex.lexeme, "yanlis") == 0)
                next_lex.token = MANTIKSAL_SABIT;
            else if (strcmp(next_lex.lexeme, "esitle") == 0)
                next_lex.token = ESITLE_OPERATOR;
            else if (strcmp(next_lex.lexeme, "eleman") == 0)
                next_lex.token = ELEMAN_OPERATOR;
            else if (strcmp(next_lex.lexeme, "topla") == 0)
                next_lex.token = TOPLA_OPERATOR;
            else if (strcmp(next_lex.lexeme, "cikar") == 0)
                next_lex.token = CIKAR_OPERATOR;
            else if (strcmp(next_lex.lexeme, "carp") == 0)
                next_lex.token = CARP_OPERATOR;
            else if (strcmp(next_lex.lexeme, "bol") == 0)
                next_lex.token = BOL_OPERATOR;
            else if (strcmp(next_lex.lexeme, "eger") == 0)
                next_lex.token = EGER;
            else if (strcmp(next_lex.lexeme, "degilse") == 0)
                next_lex.token = DEGILSE;
            else if (strcmp(next_lex.lexeme, "degil") == 0)
                next_lex.token = MANTIKSAL_DEGIL_OPERATOR;
            else if (strcmp(next_lex.lexeme, "birlestir") == 0)
                next_lex.token = BIRLESTIR_OPERATOR;
            else if (strcmp(next_lex.lexeme, "dongu") == 0)
                next_lex.token = DONGU;
            else if (strcmp(next_lex.lexeme, "eleman") == 0)
                next_lex.token = ELEMAN_OPERATOR;
            else if (strcmp(next_lex.lexeme, "tamsayi") == 0)
                next_lex.token = TAMSAYI_TIP;
            else if (strcmp(next_lex.lexeme, "ondalik") == 0)
                next_lex.token = ONDALIK_TIP;
            else if (strcmp(next_lex.lexeme, "karakter") == 0)
                next_lex.token = KARAKTER_TIP;
            else if (strcmp(next_lex.lexeme, "mantiksal") == 0)
                next_lex.token = MANTIKSAL_TIP;
            else if (strcmp(next_lex.lexeme, "dizi") == 0)
                next_lex.token = DIZI_TIP;
            else if (strcmp(next_lex.lexeme, "yazi") == 0)
                next_lex.token = YAZI_TIP;
            else if (strcmp(next_lex.lexeme, "yazdir") == 0)
                next_lex.token = YAZDIR_OPERATOR;
            else if (strcmp(next_lex.lexeme, "buyuktur") == 0)
                next_lex.token = BUYUKTUR_OPERATOR;
            else if (strcmp(next_lex.lexeme, "kucuktur") == 0)
                next_lex.token = KUCUKTUR_OPERATOR;
            else if (strcmp(next_lex.lexeme, "esittir") == 0)
                next_lex.token = ESITTIR_OPERATOR;
            else
                next_lex.token = ISIM;
            break;
        case '-':
            add();
            read();
            while (next_ch.type == RAKAM) {
                add();
                read();
            }
            if (next_ch.ch == '.') {
                add();
                read();
                size_t count = 0;
                while (next_ch.type == RAKAM) {
                    add();
                    read();
                    ++count;
                }
                if (count == 0) {
                    printf("HATA: ONDALIK SAYININ ONDALIK KISMINI YAZMADINIZ.\n");
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                } else next_lex.token = ONDALIK_SABIT;

                break;
            } else next_lex.token = TAMSAYI_SABIT;
            break;
        case '(':
            add();
            read();
            next_lex.token = SOL_PARANTEZ;
            break;
        case ')':
            add();
            read();
            next_lex.token = SAG_PARANTEZ;
            break;
        case '[':
            add();
            read();
            next_lex.token = SOL_INDEX_OPERATOR;
            break;
        case ']':
            add();
            read();
            next_lex.token = SAG_INDEX_OPERATOR;
            break;
        case '{':
            add();
            read();
            next_lex.token = KAPSAM_BASLANGICI;
            break;
        case '}':
            add();
            read();
            next_lex.token = KAPSAM_SONU;
            break;
        case ',':
            add();
            read();
            next_lex.token = VIRGUL;
            break;
        case ';':
            add();
            read();
            next_lex.token = NOKTALI_VIRGUL;
            break;
        case '\'':
            read();
            if (isprint(next_ch.ch)) {
                add();
                read();
            } else {
                add();
                next_lex.token = EOF;
            }
            if (next_ch.ch == '\'')
                next_lex.token = KARAKTER_SABIT;
            else {
                add();
                next_lex.token = EOF;
            }
            read();
            break;

        default:
            add();
            next_lex.token = EOF;
            break;
    }
    return next_lex.token;
}

list_lexeme_t *list_lexeme_t_create(size_t s) {
    list_lexeme_t *list = malloc(sizeof *list);

    if (list == NULL)
        return NULL;

    list->data = (lexeme_t *) malloc(s * sizeof(lexeme_t));

    if (list->data == NULL) {
        free(list);
        return NULL;
    }

    list->size = s;
    list->length = 0;

    return list;
}

void list_lexeme_t_add(list_lexeme_t *list, lexeme_t item) {
    if (list->length < list->size) {
        *(list->data + list->length) = item;
        list->length += 1;
    } else {
        list_lexeme_t_enlarge(list);
        list_lexeme_t_add(list, item);
    }
}

void list_lexeme_t_enlarge(list_lexeme_t *list) {
    size_t newSize = 2 * list->size;
    list->data = (lexeme_t *) realloc(list->data, 2 * newSize * sizeof(lexeme_t));
    list->size = newSize;
}

