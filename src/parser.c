#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "lexer.h"
#include "parser.h"

list_var_t *var_list;
list_arr_t *arr_list;

int main() {
    char file_name[MAX_LEX_SIZE];
    printf("LUTFEN KODLARIN YAZILI OLDUGU DOSYAYI UZANTISIYLA BIRLIKTE GIRIN:\n");
    scanf("%s", &file_name);

    run_lexer(file_name);

    var_list = list_var_t_create(10);
    arr_list = list_arr_t_create(10);

    size_t pos = 0;
    while (pos < lex_list->length) {
        statement(&pos);
    }

    free(lex_list);
    free(var_list);
    free(arr_list);


    return 0;
}

var_t define(size_t *current_ptr) {
    size_t start = *current_ptr;

    if ((lex_list->data + (*current_ptr))->token == TAMSAYI_TIP ||
        (lex_list->data + (*current_ptr))->token == ONDALIK_TIP ||
        (lex_list->data + (*current_ptr))->token == MANTIKSAL_TIP ||
        (lex_list->data + (*current_ptr))->token == KARAKTER_TIP ||
        (lex_list->data + (*current_ptr))->token == YAZI_TIP) {

        (*current_ptr)++;

        if ((lex_list->data + (*current_ptr))->token != ISIM) {
            printf("HATA: DEGISKEN ISMI YAZMADINIZ.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        } else {
            for (size_t i = 0; i < var_list->length; ++i)
                if (strcmp((var_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0) {
                    printf("HATA: AYNI DEGISKEN BIRDEN FAZLA KEZ TANIMLANAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }

            for (size_t i = 0; i < arr_list->length; ++i)
                if (strcmp((arr_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0) {
                    printf("HATA: AYNI DEGISKEN BIRDEN FAZLA KEZ TANIMLANAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
        }

        var_t d;
        switch ((lex_list->data + start)->token) {
            case TAMSAYI_TIP:
                d.type = TAMSAYI_SABIT;
                d.value_int = 0;
                break;
            case ONDALIK_TIP:
                d.type = ONDALIK_SABIT;
                d.value_float = 0;
                break;
            case KARAKTER_TIP:
                d.type = KARAKTER_SABIT;
                d.value_char = 0;
                break;
            case MANTIKSAL_TIP:
                d.type = MANTIKSAL_SABIT;
                strcpy(d.value_bool, "yanlis");
                break;
            case YAZI_TIP:
                d.type = YAZI_SABIT;
                strcpy(d.value_string, "");
                break;
        }
        strcpy(d.name, (lex_list->data + (*current_ptr))->lexeme);
        return d;
    }
}

arr_t *define_arr(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == DIZI_TIP) {

        int local_type;


        (*current_ptr)++;
        switch ((lex_list->data + (*current_ptr))->token) {
            case TAMSAYI_TIP:
                local_type = TAMSAYI_SABIT;
                break;
            case ONDALIK_TIP:
                local_type = ONDALIK_SABIT;
                break;
            case MANTIKSAL_TIP:
                local_type = MANTIKSAL_SABIT;
                break;
            case KARAKTER_TIP:
                local_type = KARAKTER_SABIT;
                break;
            case YAZI_TIP:
                local_type = YAZI_SABIT;
                break;
            default: {
                printf("HATA: DIZININ TIPINI YAZMADINIZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != ISIM) {
            printf("HATA: DEGISKEN ISMI YAZMADINIZ.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        } else {
            for (size_t i = 0; i < var_list->length; ++i)
                if (strcmp((var_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0) {
                    printf("HATA: AYNI DEGISKEN BIRDEN FAZLA KEZ TANIMLANAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }

            for (size_t i = 0; i < arr_list->length; ++i)
                if (strcmp((arr_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0) {
                    printf("HATA: AYNI DIZI BIRDEN FAZLA KEZ TANIMLANAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
        }

        char local_name[MAX_LEX_SIZE];
        strcpy(local_name, (lex_list->data + (*current_ptr))->lexeme);

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_INDEX_OPERATOR) {
            printf("HATA: \"[\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        //
        int local_size;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT) {
            local_size = atoi((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local_var = var_list->data + local_var_index;

            if (local_var->type != TAMSAYI_SABIT) {
                printf("HATA: DIZININ BOYUTU OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            local_size = local_var->value_int;
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local_var = get_arr_element(current_ptr);

            if (local_var->type != TAMSAYI_SABIT) {
                printf("HATA: DIZININ BOYUTU OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            local_size = local_var->value_int;
        } else if (if_arithmetic_operation(current_ptr)) {
            double local_value = arithmetic_operation(current_ptr);

            if (local_value - ((long) local_value) != 0) {
                printf("HATA: DIZININ BOYUTU OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }


            local_size = (int) local_value;
        } else {
            printf("HATA: ELEMANIN BOYUTUNU YAZMADINIZ.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
        //

        if (local_size < 1) {
            printf("HATA: DIZILERIN BOYUTU 1'DEN KUCUK OLAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_INDEX_OPERATOR) {
            printf("HATA: \"]\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        arr_t *local_arr = arr_t_create(local_type, local_name, local_size);

        return local_arr;
    }
}

int do_if_define(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == TAMSAYI_TIP ||
        (lex_list->data + (*current_ptr))->token == ONDALIK_TIP ||
        (lex_list->data + (*current_ptr))->token == MANTIKSAL_TIP ||
        (lex_list->data + (*current_ptr))->token == KARAKTER_TIP ||
        (lex_list->data + (*current_ptr))->token == YAZI_TIP) {
        list_var_t_add(var_list, define(current_ptr));
        return 1;
    }
    return 0;
}

int do_if_define_arr(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == DIZI_TIP) {
        arr_t *local = define_arr(current_ptr);
        list_arr_t_add(arr_list, *local);
        free(local);
        return 1;
    }
    return 0;
}

int get_index_of_var(char *name) {
    int var_idx = -1;
    for (size_t i = 0; i < var_list->length; ++i)
        if (strcmp(name, (var_list->data + i)->name) == 0) {
            var_idx = i;
            break;
        }

    return var_idx;
}

double arithmetic_operation(size_t *current_ptr) {
    double hold = 0;
    size_t start = *(current_ptr);
    (*(current_ptr)) = start;
    double left_operand = 0;
    double right_operand = 0;


    if ((lex_list->data + (*(current_ptr)))->token == TOPLA_OPERATOR ||
        (lex_list->data + (*(current_ptr)))->token == CIKAR_OPERATOR ||
        (lex_list->data + (*(current_ptr)))->token == CARP_OPERATOR ||
        (lex_list->data + (*(current_ptr)))->token == BOL_OPERATOR) {
        (*(current_ptr))++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*(current_ptr))++;
        if ((lex_list->data + (*(current_ptr)))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*(current_ptr)))->token == ONDALIK_SABIT) {
            left_operand = atof((lex_list->data + (*(current_ptr)))->lexeme);
        } else if ((lex_list->data + (*(current_ptr)))->token == ISIM) {
            int var_idx = get_index_of_var((lex_list->data + (*(current_ptr)))->lexeme);

            if (var_idx == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            switch ((var_list->data + var_idx)->type) {
                case TAMSAYI_SABIT:
                    left_operand = (var_list->data + var_idx)->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = (var_list->data + var_idx)->value_float;
                    break;
                case MANTIKSAL_SABIT:
                    printf("HATA: MANTIKSAL DEGISKENLERLE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case KARAKTER_SABIT:
                    printf("HATA: KARAKTERLER ILE ARITMETIK ISLEM YAPILAMAZ. KARAKTERLERI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case YAZI_SABIT:
                    printf("HATA: YAZILAR ILE ARITMETIK ISLEM YAPILAMAZ. YAZILARI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
            }
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local = get_arr_element(current_ptr);

            switch ((*local).type) {
                case TAMSAYI_SABIT:
                    left_operand = (*local).value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = (*local).value_float;
                    break;
                case MANTIKSAL_SABIT:
                    printf("HATA: MANTIKSAL DEGISKENLERLE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case KARAKTER_SABIT:
                    printf("HATA: KARAKTERLER ILE ARITMETIK ISLEM YAPILAMAZ. KARAKTERLERI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case YAZI_SABIT:
                    printf("HATA: YAZILAR ILE ARITMETIK ISLEM YAPILAMAZ. YAZILARI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
            }

        } else if ((lex_list->data + ((*(current_ptr))))->token == TOPLA_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == CIKAR_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == CARP_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == BOL_OPERATOR) {
            left_operand = arithmetic_operation(current_ptr);
        }
        else if ((lex_list->data + (*(current_ptr)))->token != TAMSAYI_SABIT && (lex_list->data + (*(current_ptr)))->token != ONDALIK_SABIT){
            printf("HATA: NUMERIK OLMAYAN TIPLER ILE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*(current_ptr))++;
        if ((lex_list->data + (*(current_ptr)))->token != VIRGUL) {
            printf("HATA: EKSIK \",\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*(current_ptr))++;
        if ((lex_list->data + (*(current_ptr)))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*(current_ptr)))->token == ONDALIK_SABIT) {
            right_operand = atof((lex_list->data + (*(current_ptr)))->lexeme);
        } else if ((lex_list->data + (*(current_ptr)))->token == ISIM) {
            int var_idx = get_index_of_var((lex_list->data + (*(current_ptr)))->lexeme);

            if (var_idx == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            switch ((var_list->data + var_idx)->type) {
                case TAMSAYI_SABIT:
                    right_operand = (var_list->data + var_idx)->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = (var_list->data + var_idx)->value_float;
                    break;
                case MANTIKSAL_SABIT:
                    printf("HATA: MANTIKSAL DEGISKENLERLE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case KARAKTER_SABIT:
                    printf("HATA: KARAKTERLER ILE ARITMETIK ISLEM YAPILAMAZ. KARAKTERLERI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case YAZI_SABIT:
                    printf("HATA: YAZILAR ILE ARITMETIK ISLEM YAPILAMAZ. YAZILARI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
            }
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local = get_arr_element(current_ptr);

            switch ((*local).type) {
                case TAMSAYI_SABIT:
                    right_operand = (*local).value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = (*local).value_float;
                    break;
                case MANTIKSAL_SABIT:
                    printf("HATA: MANTIKSAL DEGISKENLERLE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case KARAKTER_SABIT:
                    printf("HATA: KARAKTERLER ILE ARITMETIK ISLEM YAPILAMAZ. KARAKTERLERI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
                case YAZI_SABIT:
                    printf("HATA: YAZILAR ILE ARITMETIK ISLEM YAPILAMAZ. YAZILARI BIRLESTIRMEK ICIN \"birlestir(karakter *k_ptr1, karakter *k_ptr2)\" FONKSIYONUNU KULLANINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                    break;
            }
        } else if ((lex_list->data + (*(current_ptr)))->token == TOPLA_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == CIKAR_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == CARP_OPERATOR ||
                   (lex_list->data + (*(current_ptr)))->token == BOL_OPERATOR) {
            right_operand = arithmetic_operation(current_ptr);
        }
        else if ((lex_list->data + (*(current_ptr)))->token != TAMSAYI_SABIT && (lex_list->data + (*(current_ptr)))->token != ONDALIK_SABIT){
            printf("HATA: NUMERIK OLMAYAN TIPLER ILE ARITMETIK ISLEM YAPILAMAZ.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*(current_ptr))++;
        if ((lex_list->data + (*(current_ptr)))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        switch ((lex_list->data + start)->token) {
            case TOPLA_OPERATOR:
                hold = left_operand + right_operand;
                break;
            case CIKAR_OPERATOR:
                hold = left_operand - right_operand;
                break;
            case CARP_OPERATOR:
                hold = left_operand * right_operand;
                break;
            case BOL_OPERATOR:
                if (right_operand == 0) {
                    printf("HATA: BOLME ISLEMINDE PAYDA 0 OLAMAZ.\nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
                hold = left_operand / right_operand;
                break;
        }
        return hold;
    }
}

int if_arithmetic_operation(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == TOPLA_OPERATOR ||
        (lex_list->data + (*current_ptr))->token == CIKAR_OPERATOR ||
        (lex_list->data + (*current_ptr))->token == CARP_OPERATOR ||
        (lex_list->data + (*current_ptr))->token == BOL_OPERATOR) {
        return 1;
    }
    return 0;
}


list_var_t *list_var_t_create(size_t s) {
    list_var_t *list = (list_var_t *) malloc(sizeof(*list));

    if (list == NULL)
        return NULL;

    list->data = (var_t *) malloc(s * sizeof(var_t));
    list->size = s;
    list->length = 0;

    return list;
}

void list_var_t_add(list_var_t *list1, var_t item) {
    if (list1->length < list1->size) {
        *(list1->data + list1->length) = item;
        list1->length += 1;
    } else {
        list1 = list_var_t_enlarge(list1);
        list_var_t_add(list1, item);
    }
}

list_var_t *list_var_t_enlarge(list_var_t *list1) {
    size_t new_size = (list1->size) * 2;
    list1->data = (var_t *) realloc(list1->data, new_size);
    list1->size = new_size;
}

int do_if_assignment(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == ESITLE_OPERATOR) {
        do_assignment(current_ptr);
        return 1;
    }
    return 0;
}

void do_assignment(size_t *current_ptr) {
    size_t start = (*current_ptr);

    if ((lex_list->data + (*current_ptr))->token == ESITLE_OPERATOR) {


        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\"\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;

        int first_type;
        var_t *to;
        to = NULL;
        int local_index;

        if ((lex_list->data + (*current_ptr))->token != ISIM) {
            if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
                to = get_arr_element(current_ptr);
            } else {
                printf("HATA: ATAMA YAPILACAK DEGISKEN ISMI EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
        } else {
            local_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);
            to = var_list->data + local_index;
        }


        if (local_index == -1) {
            printf("HATA: BIR DEGISKENI/DIZIYI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        first_type = to->type;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != VIRGUL) {
            printf("HATA: EKSIK \",\"\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if (if_bool_operation(current_ptr) == 1) {
            if (to->type != MANTIKSAL_SABIT) {
                printf("HATA: MANTIKSAL TIPTE OLMAYAN BIR DEGISKENE MANTIKSAL ISLEM YAPAMAZSINIZ.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
            char *s = do_bool_operation(current_ptr);
            strcpy(to->value_bool, s);
            free(s);
        } else if (if_catenation(current_ptr) == 1) {
            if (to->type != YAZI_SABIT) {
                printf("HATA: YAZI SABIT REFERANS TIPI OLMAYAN BIR DEGERE BIRLESTIRME ISLEMININ SONUCUNU ATAYAMAZSINIZ.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
            char *s = do_catenation(current_ptr);
            strcpy(to->value_string, s);
            free(s);
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local = get_arr_element(current_ptr);
            int second_type = (local->type);

            if (first_type == TAMSAYI_SABIT && second_type == ONDALIK_SABIT) {
                double temp = local->value_float;
                if (temp - ((long) temp) == 0)
                    to->value_int = (int) (local->value_float);
                else {
                    printf("HATA: DARALAN TIP DONUSUMUNE (TAMSAYI <- ONDALIK) IZIN VERILMEMEKTEDIR.\nHATA YERI: %d. LEXEME",
                           (*current_ptr));
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
            } else if (first_type == ONDALIK_SABIT && second_type == TAMSAYI_SABIT) {
                to->value_float = local->value_int;
            } else if (first_type == second_type) {
                switch (first_type) {
                    case TAMSAYI_SABIT:
                        to->value_int = local->value_int;
                        break;
                    case ONDALIK_SABIT:
                        to->value_float = local->value_float;
                        break;
                    case MANTIKSAL_SABIT:
                        strcpy(to->value_bool, local->value_bool);
                        break;
                    case KARAKTER_SABIT:
                        to->value_char = local->value_char;
                        break;
                    case YAZI_SABIT:
                        strcpy(to->value_string, local->value_string);
                        break;
                }
            } else {
                if (second_type == TAMSAYI_SABIT
                    || second_type == ONDALIK_SABIT
                    || second_type == MANTIKSAL_SABIT
                    || second_type == KARAKTER_SABIT
                    || second_type == YAZI_SABIT) {
                    printf("HATA: ATANAN IFADEYLE ATANACAK DEGISKENIN TIPLERI AYNI OLMAK ZORUNDADIR.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                } else {
                    printf("HATA: ATAMA ISLEMI ICIN GEREKEN ISLEM YA DA DEGER EKSIK.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
            }
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int var_idx_2 = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (var_idx_2 == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            int second_type = (var_list->data + var_idx_2)->type;

            if (first_type == TAMSAYI_SABIT && second_type == ONDALIK_SABIT) {
                double temp = (var_list->data + var_idx_2)->value_float;
                if ((temp - (long) temp) == 0)
                    to->value_int = (int) (var_list->data + var_idx_2)->value_float;
                else {
                    printf("HATA: DARALAN TIP DONUSUMUNE (TAMSAYI <- ONDALIK) IZIN VERILMEMEKTEDIR.\nHATA YERI: %d. LEXEME",
                           (*current_ptr));
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
            } else if (first_type == ONDALIK_SABIT && second_type == TAMSAYI_SABIT) {
                to->value_float = (var_list->data + var_idx_2)->value_int;
            } else if (first_type == second_type) {
                switch (first_type) {
                    case TAMSAYI_SABIT:
                        to->value_int = (var_list->data + var_idx_2)->value_int;
                        break;
                    case ONDALIK_SABIT:
                        to->value_float = (var_list->data + var_idx_2)->value_float;
                        break;
                    case MANTIKSAL_SABIT:
                        strcpy(to->value_bool, (var_list->data + var_idx_2)->value_bool);
                        break;
                    case KARAKTER_SABIT:
                        to->value_char = (var_list->data + var_idx_2)->value_char;
                        break;
                    case YAZI_SABIT:
                        strcpy(to->value_string, (var_list->data + var_idx_2)->value_string);
                        break;
                }
            }
            else {
                if (second_type == TAMSAYI_SABIT
                    || second_type == ONDALIK_SABIT
                    || second_type == MANTIKSAL_SABIT
                    || second_type == KARAKTER_SABIT
                    || second_type == YAZI_SABIT) {
                    printf("HATA: ATANAN IFADEYLE ATANACAK DEGISKENIN TIPLERI AYNI OLMAK ZORUNDADIR.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                } else {
                    printf("HATA: ATAMA ISLEMI ICIN GEREKEN ISLEM YA DA DEGER EKSIK.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            if (first_type != ONDALIK_SABIT && first_type != TAMSAYI_SABIT) {
                printf("HATA: ATANAN IFADEYLE ATANACAK DEGISKENIN TIPLERI AYNI OLMAK ZORUNDADIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
            double result = arithmetic_operation(current_ptr);
            switch (to->type) {
                case TAMSAYI_SABIT: {
                    if ((result - (long) result) == 0)
                        to->value_int = (int) result;
                    else {
                        printf("HATA: DARALAN TIP DONUSUMUNE (TAMSAYI <- ONDALIK) IZIN VERILMEMEKTEDIR.\nHATA YERI: %d. LEXEME",
                               (*current_ptr));
                        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                        getch();
                        exit(1);
                    }
                    break;
                }
                case ONDALIK_SABIT:
                    to->value_float = result;
                    break;
            }
        } else if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT && to->type == ONDALIK_SABIT)
            to->value_float = atof((lex_list->data + (*current_ptr))->lexeme);
        else if ((lex_list->data + (*current_ptr))->token == ONDALIK_SABIT && to->type == TAMSAYI_SABIT) {
            double temp = atof((lex_list->data + (*current_ptr))->lexeme);
            if ((temp - (long) temp) == 0)
                to->value_int = (int) temp;
            else {
                printf("HATA: DARALAN TIP DONUSUMUNE (TAMSAYI <- ONDALIK) IZIN VERILMEMEKTEDIR.\nHATA YERI: %d. LEXEME",
                       (*current_ptr));
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
        } else if ((lex_list->data + (*current_ptr))->token == to->type) {
            switch (to->type) {
                case TAMSAYI_SABIT:
                    to->value_int = atoi((lex_list->data + (*current_ptr))->lexeme);
                case ONDALIK_SABIT:
                    to->value_float = atof((lex_list->data + (*current_ptr))->lexeme);
                    break;
                case MANTIKSAL_SABIT:
                    strcpy(to->value_bool, (lex_list->data + (*current_ptr))->lexeme);
                    break;
                case KARAKTER_SABIT:
                    to->value_char = *((lex_list->data + (*current_ptr))->lexeme + 0);
                    break;
                case YAZI_SABIT:
                    strcpy(to->value_string, (lex_list->data + (*current_ptr))->lexeme);
                    break;
            }
        } else {
            if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
                (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT ||
                (lex_list->data + (*current_ptr))->token == MANTIKSAL_SABIT ||
                (lex_list->data + (*current_ptr))->token == KARAKTER_SABIT ||
                (lex_list->data + (*current_ptr))->token == YAZI_SABIT) {
                printf("HATA: ATANAN IFADEYLE ATANACAK DEGISKENIN TIPLERI AYNI OLMAK ZORUNDADIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
            printf("HATA: ATAMA ISLEMI ICIN GEREKEN ISLEM YA DA DEGER EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\"\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
    }
}

char *do_bool_operation(size_t *current_ptr) {
    size_t start = *current_ptr;
    if (is_comparison(current_ptr) == 1) {
        char *local = (char *) malloc(sizeof(char) * 7);

        switch ((lex_list->data + (*current_ptr))->token) {
            case KUCUKTUR_OPERATOR:
                strcpy(local, is_less(current_ptr));
                break;
            case ESITTIR_OPERATOR:
                strcpy(local, is_equal(current_ptr));
                break;
            case BUYUKTUR_OPERATOR:
                strcpy(local, is_greater(current_ptr));
                break;
        }

        return local;

    } else if ((lex_list->data + (*current_ptr))->token == MANTIKSAL_DEGIL_OPERATOR) {

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == ISIM) {

            int var_idx = -1; // sentinel
            var_idx = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (var_idx == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            int dogru_mu = 0;
            strcmp((var_list->data + var_idx)->value_bool, "dogru") == 0 ? (dogru_mu = 1) : (dogru_mu = 0);
            switch (dogru_mu) {
                case 0 : {
                    char *s = (char *) malloc(sizeof(char) * 7);
                    strcpy(s, "dogru");
                    (*current_ptr)++;
                    if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                        printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                        getch();
                        exit(1);
                    }
                    return s;
                }
                case 1: {
                    char *s = (char *) malloc(sizeof(char) * 7);
                    strcpy(s, "yanlis");
                    (*current_ptr)++;
                    if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                        printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                        getch();
                        exit(1);
                    }
                    return s;
                }
            }
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local = get_arr_element(current_ptr);

            if (local->type != MANTIKSAL_SABIT) {
                printf("HATA: MANTIKSAL OPERATORLERE, MANTIKSAL SABIT TIPINDEN FARKLI TIPTE DEGERLER GONDERILEMEZ.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            char *s = (char *) malloc(sizeof(char) * 7);
            if (strcmp(local->value_bool, "dogru") == 0) {
                strcpy(s, "yanlis");
                (*current_ptr)++;
                if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                    printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
                return s;
            } else if (strcmp(local->value_bool, "yanlis") == 0) {
                strcpy(s, "dogru");
                (*current_ptr)++;
                if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                    printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
                return s;
            }
        } else if ((lex_list->data + (*current_ptr))->token == MANTIKSAL_SABIT) {
            if (strcmp((lex_list->data + (*current_ptr))->lexeme, "dogru") == 0) {
                char *s = (char *) malloc(sizeof(char) * 7);
                strcpy(s, "yanlis");
                (*current_ptr)++;
                if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                    printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
                return s;
            } else if (strcmp((lex_list->data + (*current_ptr))->lexeme, "yanlis") == 0) {
                char *s = (char *) malloc(sizeof(char) * 7);
                strcpy(s, "dogru");
                (*current_ptr)++;
                if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                    printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
                }
                return s;
            }
        } else if (is_comparison(current_ptr) == 1) {
            char *local = (char *) malloc(sizeof(char) * 7);

            switch ((lex_list->data + (*current_ptr))->token) {
                case KUCUKTUR_OPERATOR:
                    strcpy(local, is_less(current_ptr));
                    break;
                case ESITTIR_OPERATOR:
                    strcpy(local, is_equal(current_ptr));
                    break;
                case BUYUKTUR_OPERATOR:
                    strcpy(local, is_greater(current_ptr));
                    break;
            }

            if (strcmp(local, "dogru") == 0)
                strcpy(local, "yanlis");
            else if (strcmp(local, "yanlis") == 0)
                strcpy(local, "dogru");

            (*current_ptr)++;
            if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
                printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }
            return local;
        }
    }
    return 0;
}

int if_bool_operation(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == MANTIKSAL_DEGIL_OPERATOR || is_comparison(current_ptr) == 1) {
        return 1;
    }
    return 0;
}

int print(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == YAZDIR_OPERATOR) {

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != ISIM) {
            printf("HATA: YAZDIRMAK ISTEDIGINIZ DEGISKEN EKSIK.\nHATA YERI : %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        int var_idx = -1;
        for (size_t i = 0; i < var_list->length; ++i) {
            if (strcmp((var_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0)
                var_idx = i;
        }

        int arr_idx = -1;
        for (size_t i = 0; i < arr_list->length; ++i) {
            if (strcmp((arr_list->data + i)->name, (lex_list->data + (*current_ptr))->lexeme) == 0) {
                arr_idx = i;
            }
        }

        if (var_idx == -1 && arr_idx == -1) {
            printf("HATA: BIR DEGISKENI/DIZIYI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (var_idx != -1) {
            switch ((var_list->data + var_idx)->type) {
                case TAMSAYI_SABIT:
                    printf("DEGISKEN TIPI: TAMSAYI, ISMI: %s, DEGERI: %d\n", (var_list->data + var_idx)->name,
                           (var_list->data + var_idx)->value_int);
                    break;
                case ONDALIK_SABIT:
                    printf("DEGISKEN TIPI: ONDALIK, ISMI: %s, DEGERI: %f\n", (var_list->data + var_idx)->name,
                           (var_list->data + var_idx)->value_float);
                    break;
                case MANTIKSAL_SABIT:
                    printf("DEGISKEN TIPI: MANTIKSAL, ISMI: %s, DEGERI: %s\n", (var_list->data + var_idx)->name,
                           (var_list->data + var_idx)->value_bool);
                    break;
                case KARAKTER_SABIT:
                    printf("DEGISKEN TIPI: KARAKTER, ISMI: %s, DEGERI: %c\n", (var_list->data + var_idx)->name,
                           (var_list->data + var_idx)->value_char);
                    break;
                case YAZI_SABIT:
                    printf("DEGISKEN TIPI: YAZI, ISMI: %s, DEGERI: %s\n", (var_list->data + var_idx)->name,
                           (var_list->data + var_idx)->value_string);
                    break;
            }
        } else if (arr_idx != -1) {
            switch ((arr_list->data + arr_idx)->type) {
                case TAMSAYI_SABIT:
                    printf("DIZI TIPI: TAMSAYI, ISMI: %s ", (arr_list->data + arr_idx)->name);
                    for (size_t i = 0; i < (arr_list->data + arr_idx)->size; ++i)
                        printf("\n%d. ELEMANIN DEGERI: %d", i, ((arr_list->data + arr_idx)->data + i)->value_int);
                    printf("\n");
                    break;
                case ONDALIK_SABIT:
                    printf("DIZI TIPI: ONDALIK, ISMI: %s ", (arr_list->data + arr_idx)->name);
                    for (size_t i = 0; i < (arr_list->data + arr_idx)->size; ++i)
                        printf("\n%d. ELEMANIN DEGERI: %f", i, ((arr_list->data + arr_idx)->data + i)->value_float);
                    printf("\n");
                    break;
                case MANTIKSAL_SABIT:
                    printf("DIZI TIPI: MANTIKSAL, ISMI: %s ", (arr_list->data + arr_idx)->name);
                    for (size_t i = 0; i < (arr_list->data + arr_idx)->size; ++i)
                        printf("\n%d. ELEMANIN DEGERI: %s", i, ((arr_list->data + arr_idx)->data + i)->value_bool);
                    printf("\n");
                    break;
                case KARAKTER_SABIT:
                    printf("DIZI TIPI: KARAKTER, ISMI: %s ", (arr_list->data + arr_idx)->name);
                    for (size_t i = 0; i < (arr_list->data + arr_idx)->size; ++i)
                        printf("\n%d. ELEMANIN DEGERI: %c", i, ((arr_list->data + arr_idx)->data + i)->value_char);
                    printf("\n");
                    break;
                case YAZI_SABIT:
                    printf("DIZI TIPI: YAZI, ISMI: %s ", (arr_list->data + arr_idx)->name);
                    for (size_t i = 0; i < (arr_list->data + arr_idx)->size; ++i)
                        printf("\n%d. ELEMANIN DEGERI: %s", i, ((arr_list->data + arr_idx)->data + i)->value_string);
                    printf("\n");
                    break;
            }
        }


        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
        return 1;
    }
    return 0;
}

arr_t *arr_t_create(enum token_types t, char *name, size_t s) {
    if (strlen(name) > MAX_LEX_SIZE - 1) {
        printf("HATA: DIZI ISMI %d KARAKTERDEN FAZLA OLAMAZ.", MAX_LEX_SIZE);
        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
        getch();
        exit(1);
    }

    arr_t *list = (arr_t *) malloc(sizeof(*list));

    if (list == NULL)
        return NULL;

    list->data = (var_t *) malloc(s * sizeof(var_t));
    list->size = s;
    list->type = t;
    strcpy(list->name, name);
    for (size_t i = 0; i < s; ++i) {
        (list->data + i)->value_float = 0;
        (list->data + i)->value_int = 0;
        strcpy((list->data + i)->value_string, "");
        strcpy((list->data + i)->value_bool, "yanlis");
        (list->data + i)->value_char = 0;
        (list->data + i)->type = t;
    }

    return list;
}

list_arr_t *list_arr_t_create(size_t s) {
    list_arr_t *list = (list_arr_t *) malloc(sizeof(*list));

    if (list == NULL)
        return NULL;

    list->data = (arr_t *) malloc(s * sizeof(arr_t));
    list->size = s;
    list->length = 0;

    return list;
}

void list_arr_t_add(list_arr_t *list1, arr_t item) {
    if (list1->length < list1->size) {
        *(list1->data + list1->length) = item;
        list1->length += 1;
    } else {
        list1 = list_arr_t_enlarge(list1);
        list_arr_t_add(list1, item);
    }
}

list_arr_t *list_arr_t_enlarge(list_arr_t *list1) {
    size_t new_size = (list1->size) * 2;
    list1->data = (arr_t *) realloc(list1->data, new_size);
    list1->size = new_size;
}

int get_index_of_arr(char *name) {
    int index = -1;

    for (size_t i = 0; i < arr_list->length; ++i) {
        if (strcmp(name, (arr_list->data + i)->name) == 0)
            index = i;
    }

    return index;
}

var_t *get_arr_element(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != ISIM) {
            printf("HATA: DIZI ADI EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        char local_name[MAX_LEX_SIZE];
        strcpy(local_name, (lex_list->data + (*current_ptr))->lexeme);

        int local_arr_list_index = get_index_of_arr(local_name);

        if (local_arr_list_index == -1) {
            printf("HATA: BIR DIZIYI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != VIRGUL) {
            printf("HATA: EKSIK \",\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        int local_index;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT) {
            local_index = atoi((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local_var = var_list->data + local_var_index;

            if (local_var->type != TAMSAYI_SABIT) {
                printf("HATA: DIZININ INDEKSI OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            local_index = local_var->value_int;
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local_var = get_arr_element(current_ptr);

            if (local_var->type != TAMSAYI_SABIT) {
                printf("HATA: DIZININ INDEKSI OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            local_index = local_var->value_int;
        } else if (if_arithmetic_operation(current_ptr)) {
            double local_value = arithmetic_operation(current_ptr);

            if (local_value - ((long) local_value) != 0) {
                printf("HATA: DIZININ INDEKSI OLARAK GIRDIGINIZ DEGER, TAMSAYI SABIT REFERANS TIPINDE OLMALIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            local_index = (int) local_value;
        } else {
            printf("HATA: ELEMANIN INDEKSINI YAZMADINIZ.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }


        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        int size = (arr_list->data + local_arr_list_index)->size;

        if (local_index > size - 1 || local_index < 0) {
            printf("HATA: GIRMIS OLDUGUNUZ INDEKS ILGILI DIZININ SINIRLARININ DISINDA.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        return ((arr_list->data + local_arr_list_index)->data + local_index);
    }
}

int if_catenation(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == BIRLESTIR_OPERATOR)
        return 1;
    return 0;
}

char *do_catenation(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == BIRLESTIR_OPERATOR) {

        char *result = (char *) malloc(sizeof(char) * MAX_LEX_SIZE);;
        char *left_operand = (char *) malloc(sizeof(char) * MAX_LEX_SIZE);
        char *right_operand = (char *) malloc(sizeof(char) * MAX_LEX_SIZE);

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == KARAKTER_SABIT) {
            strcpy(left_operand, (lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == YAZI_SABIT) {
            strcpy(left_operand, (lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *src = (var_list->data + local_var_index);

            switch (src->type) {
                case KARAKTER_SABIT:{
                    char temp[2];
                    temp[0] = src->value_char;
                    temp[1] = 0;
                    strcpy(left_operand, temp);
                }
                    break;
                case YAZI_SABIT:
                    strcpy(left_operand, src->value_string);
                    break;
                default:
                    printf("HATA: BIRLESTIR OPERATORU ILE SADECE KARAKTER SABIT VE YAZI SABIT REFERANS TIPLERIYLE ISLEM YAPABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *src = get_arr_element(current_ptr);

            switch (src->type) {
                case KARAKTER_SABIT:{
                    char temp[2];
                    temp[0] = src->value_char;
                    temp[1] = 0;
                    strcpy(left_operand, temp);
                }
                    break;
                case YAZI_SABIT:
                    strcpy(left_operand, src->value_string);
                    break;
                default:
                    printf("HATA: BIRLESTIR OPERATORU ILE SADECE KARAKTER SABIT VE YAZI SABIT REFERANS TIPLERIYLE ISLEM YAPABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if ((lex_list->data + (*current_ptr))->token == BIRLESTIR_OPERATOR) {
            strcpy(left_operand, do_catenation(current_ptr));
        } else {
            printf("HATA: BIRLESTIR OPERATORUNUN ILK OPERANDI EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != VIRGUL) {
            printf("HATA: EKSIK \",\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == KARAKTER_SABIT) {
            strcpy(right_operand, (lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == YAZI_SABIT) {
            strcpy(right_operand, (lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *src = (var_list->data + local_var_index);

            switch (src->type) {
                case KARAKTER_SABIT:{
                    char temp[2];
                    temp[0] = src->value_char;
                    temp[1] = 0;
                    strcpy(right_operand, temp);
                }
                    break;
                case YAZI_SABIT:
                    strcpy(right_operand, src->value_string);
                    break;
                default:
                    printf("HATA: BIRLESTIR OPERATORU ILE SADECE KARAKTER SABIT VE YAZI SABIT REFERANS TIPLERIYLE ISLEM YAPABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *src = get_arr_element(current_ptr);

            switch (src->type) {
                case KARAKTER_SABIT:{
                    char temp[2];
                    temp[0] = src->value_char;
                    temp[1] = 0;
                    strcpy(right_operand, temp);
                }
                    break;
                case YAZI_SABIT:
                    strcpy(right_operand, src->value_string);
                    break;
                default:
                    printf("HATA: BIRLESTIR OPERATORU ILE SADECE KARAKTER SABIT VE YAZI SABIT REFERANS TIPLERIYLE ISLEM YAPABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if ((lex_list->data + (*current_ptr))->token == BIRLESTIR_OPERATOR) {
            strcpy(right_operand, do_catenation(current_ptr));
        } else {
            printf("HATA: BIRLESTIR OPERATORUNUN IKINCI OPERANDI EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if ((strlen(left_operand) + strlen(right_operand)) > MAX_LEX_SIZE) {
            printf("HATA: BIRLESTIRILEN YAZI MAKSIMUM KARAKTER SAYISINI ASIYOR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        strcpy(result, "");
        strcat(result, left_operand);
        strcat(result, right_operand);
        free(left_operand);
        free(right_operand);
        return strdup(result);

    }

}

int statement(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == EOF){
        printf("CALISTIRMA BASARILI. HATAYA RASTLANMADI.\nCIKMAK ICIN BIR TUSA BASIN...\n");
        getch();
        exit(0);
    }
    int i = do_if_define(current_ptr);
    if (i == 1) {
        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != NOKTALI_VIRGUL) {
            printf("HATA: YARGI BELIRTEN IFADELERIN SONUNA \";\" KONULMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
    }
    int j = do_if_define_arr(current_ptr);
    if (j == 1) {
        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != NOKTALI_VIRGUL) {
            printf("HATA: YARGI BELIRTEN IFADELERIN SONUNA \";\" KONULMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
    }
    int k = do_if_assignment(current_ptr);
    if (k == 1) {
        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != NOKTALI_VIRGUL) {
            printf("HATA: YARGI BELIRTEN IFADELERIN SONUNA \";\" KONULMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
    }
    int l = print(current_ptr);
    if (l == 1) {
        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != NOKTALI_VIRGUL) {
            printf("HATA: YARGI BELIRTEN IFADELERIN SONUNA \";\" KONULMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }
    }
    int m = do_if_if(current_ptr);
    int n = do_if_while(current_ptr);
    if (i == 0 && j == 0 && k == 0 && l == 0 && m == 0 && n == 0) {
        printf("HATA: YAZILAN KODLAR BIR YARGI ICERMEMEKTEDIR.\nHATA YERI: %d. LEXEME", *current_ptr);
        printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
        getch();
        exit(1);
    }

    (*current_ptr)++;
};

char *is_greater(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == BUYUKTUR_OPERATOR) {
        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        double left_operand;
        double right_operand;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            left_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            left_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN ILK OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != VIRGUL) {
            printf("HATA: \",\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            right_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            right_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN IKINCI OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (left_operand > right_operand)
            return strdup("dogru");
        else
            return strdup("yanlis");
    }
    printf("HATA: BILINMEYEN CALISMA ZAMANI HATASI:\n");
    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
    getch();
    exit(1);
}

char *is_less(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == KUCUKTUR_OPERATOR) {
        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        double left_operand;
        double right_operand;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            left_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            left_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN ILK OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != VIRGUL) {
            printf("HATA: \",\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            right_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            right_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN IKINCI OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (left_operand < right_operand)
            return strdup("dogru");
        else
            return strdup("yanlis");
    }
    printf("HATA: BILINMEYEN CALISMA ZAMANI HATASI:\n");
    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
    getch();
    exit(1);
}

char *is_equal(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == ESITTIR_OPERATOR) {
        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SOL_PARANTEZ) {
            printf("HATA: EKSIK \"(\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        double left_operand;
        double right_operand;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            left_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    left_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    left_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            left_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN ILK OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != VIRGUL) {
            printf("HATA: \",\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == TAMSAYI_SABIT ||
            (lex_list->data + (*current_ptr))->token == ONDALIK_SABIT) {
            right_operand = atof((lex_list->data + (*current_ptr))->lexeme);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_var_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_var_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local = (var_list->data + local_var_index);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }

        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {

            var_t *local = get_arr_element(current_ptr);

            switch (local->type) {
                case TAMSAYI_SABIT:
                    right_operand = local->value_int;
                    break;
                case ONDALIK_SABIT:
                    right_operand = local->value_float;
                    break;
                default:
                    printf("HATA: KARSILASTIRMA OPERATORLERINDE OPERAND OLARAK YALNIZCA NUMERIK REFERANS TIPLERI VEYA ARITMETIK ISLEM KULLANABILIRSINIZ.\nHATA YERI: %d. LEXEME",
                           *current_ptr);
                    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                    getch();
                    exit(1);
            }
        } else if (if_arithmetic_operation(current_ptr) == 1) {
            right_operand = arithmetic_operation(current_ptr);
        } else {
            printf("HATA: KARSILASTIRMA OPERATORUNUN IKINCI OPERANDI EKSIK/YANLIS.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*(current_ptr)))->token != SAG_PARANTEZ) {
            printf("HATA: EKSIK \")\" \nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (left_operand == right_operand)
            return strdup("dogru");
        else
            return strdup("yanlis");
    }
    printf("HATA: BILINMEYEN CALISMA ZAMANI HATASI:\n");
    printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
    getch();
    exit(1);
}

int is_comparison(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == KUCUKTUR_OPERATOR ||
        (lex_list->data + (*current_ptr))->token == ESITTIR_OPERATOR ||
        (lex_list->data + (*current_ptr))->token == BUYUKTUR_OPERATOR) {
        return 1;
    }
    return 0;
}

int do_if_if(size_t *current_ptr) {
    if ((lex_list->data + (*current_ptr))->token == EGER) {

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_PARANTEZ) {
            printf("HATA: \"(\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        int bool_expr = -1;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == MANTIKSAL_SABIT) {
            (strcmp((lex_list->data + (*current_ptr))->lexeme, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local_var = var_list->data + local_index;

            if (local_var->type != MANTIKSAL_SABIT) {
                printf("HATA: EGER YAPISINDAN SONRAKI ILK PARANTEZIN ICINE YAZDIGINIZ DEGISKENIN REFERANS TIPININ MANTIKSAL SABIT OLMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            (strcmp(local_var->value_bool, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local_var = get_arr_element(current_ptr);

            if (local_var->type != MANTIKSAL_SABIT) {
                printf("HATA: EGER YAPISINDAN SONRAKI ILK PARANTEZIN ICINE YAZDIGINIZ DIZI ELEMANININ REFERANS TIPININ MANTIKSAL SABIT OLMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            (strcmp(local_var->value_bool, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if (if_bool_operation(current_ptr) == 1) {
            (strcmp("dogru", do_bool_operation(current_ptr)) == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else {
            printf("HATA: EGER YAPISINDAN SONRAKI ILK PARANTEZIN ICINE MANTIKSAL REFERANS TIPINDE BIR DEGER YA DA ISLEM YAZMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
            printf("HATA: \")\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != KAPSAM_BASLANGICI) {
            printf("HATA: \"{\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        size_t first_bracket_start = *current_ptr;
        size_t first_bracket_end = 0;
        size_t bracket_counter = 1;

        while ((*current_ptr) < lex_list->length) {
            if ((lex_list->data + (*current_ptr))->token == KAPSAM_BASLANGICI)
                bracket_counter++;
            if ((lex_list->data + (*current_ptr))->token == KAPSAM_SONU) {
                bracket_counter--;
                first_bracket_end = *current_ptr;
            }
            if (bracket_counter == 0)
                break;
            (*current_ptr)++;
        }

        if (bracket_counter != 0) {
            printf("HATA: \"}\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (bool_expr == 1) {
            (*current_ptr) = first_bracket_start;
            while ((*current_ptr) < first_bracket_end)
                statement(current_ptr);
        } else (*current_ptr) = first_bracket_end;

        if ((lex_list->data + ((*current_ptr) + 1))->token == DEGILSE) {
            (*current_ptr)++;
            (*current_ptr)++;

            if ((lex_list->data + (*current_ptr))->token != KAPSAM_BASLANGICI) {
                printf("HATA: \"{\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            (*current_ptr)++;
            size_t first_bracket_start = *current_ptr;
            size_t first_bracket_end = 0;
            size_t bracket_counter = 1;

            while ((*current_ptr) < lex_list->length) {
                if ((lex_list->data + (*current_ptr))->token == KAPSAM_BASLANGICI)
                    bracket_counter++;
                if ((lex_list->data + (*current_ptr))->token == KAPSAM_SONU) {
                    bracket_counter--;
                    first_bracket_end = *current_ptr;
                }
                if (bracket_counter == 0)
                    break;
                (*current_ptr)++;
            }

            if (bool_expr == 0) {
                (*current_ptr) = first_bracket_start;
                while ((*current_ptr) < first_bracket_end)
                    statement(current_ptr);
            } else (*current_ptr) = first_bracket_end;


        }
        return 1;
    }
    return 0;
}

int do_if_while(size_t *current_ptr) {
    size_t start = *current_ptr;
    start:
    *current_ptr = start;
    if ((lex_list->data + (*current_ptr))->token == DONGU) {

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SOL_PARANTEZ) {
            printf("HATA: \"(\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        int bool_expr = -1;

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token == MANTIKSAL_SABIT) {
            (strcmp((lex_list->data + (*current_ptr))->lexeme, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if ((lex_list->data + (*current_ptr))->token == ISIM) {
            int local_index = get_index_of_var((lex_list->data + (*current_ptr))->lexeme);

            if (local_index == -1) {
                printf("HATA: BIR DEGISKENI KULLANMADAN ONCE TANIMLAMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            var_t *local_var = var_list->data + local_index;

            if (local_var->type != MANTIKSAL_SABIT) {
                printf("HATA: DONGU YAPISINDAN SONRAKI ILK PARANTEZIN ICINE YAZDIGINIZ DEGISKENIN REFERANS TIPININ MANTIKSAL SABIT OLMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            (strcmp(local_var->value_bool, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if ((lex_list->data + (*current_ptr))->token == ELEMAN_OPERATOR) {
            var_t *local_var = get_arr_element(current_ptr);

            if (local_var->type != MANTIKSAL_SABIT) {
                printf("HATA: DONGU YAPISINDAN SONRAKI ILK PARANTEZIN ICINE YAZDIGINIZ DIZI ELEMANININ REFERANS TIPININ MANTIKSAL SABIT OLMASI GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                       *current_ptr);
                printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
                getch();
                exit(1);
            }

            (strcmp(local_var->value_bool, "dogru") == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else if (if_bool_operation(current_ptr) == 1) {
            (strcmp("dogru", do_bool_operation(current_ptr)) == 0) ? (bool_expr = 1) : (bool_expr = 0);
        } else {
            printf("HATA: DONGU YAPISINDAN SONRAKI ILK PARANTEZIN ICINE MANTIKSAL REFERANS TIPINDE BIR DEGER YA DA ISLEM YAZMANIZ GEREKLIDIR.\nHATA YERI: %d. LEXEME",
                   *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != SAG_PARANTEZ) {
            printf("HATA: \")\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        if ((lex_list->data + (*current_ptr))->token != KAPSAM_BASLANGICI) {
            printf("HATA: \"{\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        (*current_ptr)++;
        size_t first_bracket_start = *current_ptr;
        size_t first_bracket_end = 0;
        size_t bracket_counter = 1;

        while ((*current_ptr) < lex_list->length) {
            if ((lex_list->data + (*current_ptr))->token == KAPSAM_BASLANGICI)
                bracket_counter++;
            if ((lex_list->data + (*current_ptr))->token == KAPSAM_SONU) {
                bracket_counter--;
                first_bracket_end = *current_ptr;
            }
            if (bracket_counter == 0)
                break;
            (*current_ptr)++;
        }

        if (bracket_counter != 0) {
            printf("HATA: \"}\" EKSIK.\nHATA YERI: %d. LEXEME", *current_ptr);
            printf("\nCIKMAK ICIN BIR TUSA BASIN...\n");
            getch();
            exit(1);
        }

        if (bool_expr == 1) {
            (*current_ptr) = first_bracket_start;
            while ((*current_ptr) < first_bracket_end)
                statement(current_ptr);
            goto start;
        } else (*current_ptr) = first_bracket_end;
        return 1;
    }
    return 0;
}