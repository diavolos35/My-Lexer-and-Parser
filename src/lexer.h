#ifndef LEXER_H
#define LEXER_H

#endif

enum char_t_types {
    HARF = 0,
    RAKAM = 1,
    BILINMEYEN = 99
};

enum token_types {
    TAMSAYI_SABIT = 10,
    ONDALIK_SABIT = 11,
    KARAKTER_SABIT = 12,
    MANTIKSAL_SABIT = 13,
    YAZI_SABIT = 15,
    ISIM = 20,
    ESITLE_OPERATOR = 30,
    ELEMAN_OPERATOR = 31,
    TOPLA_OPERATOR = 32,
    CIKAR_OPERATOR = 33,
    CARP_OPERATOR = 34,
    BOL_OPERATOR = 35,
    MANTIKSAL_DEGIL_OPERATOR = 36,
    SOL_PARANTEZ = 37,
    SAG_PARANTEZ = 38,
    BIRLESTIR_OPERATOR = 39,
    SOL_INDEX_OPERATOR = 40,
    SAG_INDEX_OPERATOR = 41,
    YAZDIR_OPERATOR = 42,
    KUCUKTUR_OPERATOR = 43,
    ESITTIR_OPERATOR = 44,
    BUYUKTUR_OPERATOR = 45,
    VIRGUL = 50,
    NOKTALI_VIRGUL = 51,
    KAPSAM_BASLANGICI = 52,
    KAPSAM_SONU = 53,
    EGER = 61,
    DEGILSE = 62,
    DONGU = 63,
    TAMSAYI_TIP = 70,
    ONDALIK_TIP = 71,
    KARAKTER_TIP = 72,
    MANTIKSAL_TIP = 73,
    DIZI_TIP = 74,
    YAZI_TIP = 75
};

#define MAX_LEX_SIZE 100

typedef struct char_t {
    char ch;
    int type;
} char_t;

typedef struct lexeme_t {
    char lexeme[MAX_LEX_SIZE];
    int length;
    int token;
} lexeme_t;

typedef struct list_lexeme_t {
    size_t size;
    size_t length;
    lexeme_t *data;
} list_lexeme_t;

