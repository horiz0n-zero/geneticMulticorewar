//
//  libcorewar_instructions_generator.c
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 05/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#include "libcorewar_instructions_generator.h"

static const struct s_libcorewar_opcode_info        g_opcodes_info[256] =
{
    [0x01] = {"live",  1, 10,   0, 0, 0x01, 0, {T_DIR}},
    [0x02] = {"ld",    2, 5,    1, 0, 0x02, 1, {T_DIR | T_IND, T_REG}},
    [0x03] = {"st",    2, 5,    1, 0, 0x03, 0, {T_REG, T_IND | T_REG}},
    [0x04] = {"add",   3, 10,   1, 0, 0x04, 1, {T_REG, T_REG, T_REG}},
    [0x05] = {"sub",   3, 10,   1, 0, 0x05, 1, {T_REG, T_REG, T_REG}},
    [0x06] = {"and",   3, 6,    1, 0, 0x06, 1, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}},
    [0x07] = {"or",    3, 6,    1, 0, 0x07, 1, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    [0x08] = {"xor",   3, 6,    1, 0, 0x08, 1, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    [0x09] = {"zjmp",  1, 20,   0, 1, 0x09, 0, {T_DIR}},
    [0x0a] = {"ldi",   3, 25,   1, 1, 0x0a, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    [0x0b] = {"sti",   3, 25,   1, 1, 0x0b, 0, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}},
    [0x0c] = {"fork",  1, 800,  0, 1, 0x0c, 0, {T_DIR}},
    [0x0d] = {"lld",   2, 10,   1, 0, 0x0d, 1, {T_DIR | T_IND, T_REG}},
    [0x0e] = {"lldi",  3, 50,   1, 1, 0x0e, 1, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    [0x0f] = {"lfork", 1, 1000, 0, 1, 0x0f, 0, {T_DIR}},
    [0x10] = {"aff",   1, 2,    1, 0, 0x10, 0, {T_REG}}
};

static size_t                                          get_cor_instruction_size(const struct s_libcorewar_opcode_info *const info, uint8_t *content)
{
    int                                                index;
    int                                                r;
    uint8_t                                            encode;// 10(DIR,4) 11(IND,2) 01(REG,1)
    size_t                                             size;

    index = 0;
    if (info->parameters_encoding)
    {
        encode = *++content;
        size = 2;
    }
    else
    {
        encode = DIR_CODE << 6;
        size = 1;
    }
    while (index < info->parameters)
    {
        r = (((int)encode << (index << 1)) >> 6) & 0b00000011;
        if (r == REG_CODE)
            size += REG_SIZE;
        else if (r == IND_CODE)
            size += IND_SIZE;
        else {
            if (info->parameters_direct_small)
                size += IND_SIZE;
            else
                size += DIR_SIZE;
        }
        ++index;
    }
    return (size);
}

struct s_libcorewar_cor_adn                 *libcorewar_get_cor_adn_from_cor_file(struct s_libcorewar_cor_file *const cor)
{
    struct s_libcorewar_cor_adn *const      file = malloc(sizeof(struct s_libcorewar_cor_adn));
    const struct s_libcorewar_opcode_info   *info;
    struct s_libcorewar_adn_instruction     *ins = NULL;
    int                                     index;
    uint8_t                                 *source;
    
    if (file)
    {
        bzero(file, sizeof(struct s_libcorewar_cor_adn));
        source = cor->instructions;
        index = 0;
        while (index < cor->length)
        {
            info = g_opcodes_info + ((int)*source & 0xFF);
            if (info->name)
            {
                if (!ins)
                {
                    ins = malloc(sizeof(struct s_libcorewar_adn_instruction));
                    file->head = ins;
                }
                else
                {
                    ins->next = malloc(sizeof(struct s_libcorewar_adn_instruction));
                    ins = ins->next;
                }
                ins->content_size = get_cor_instruction_size(info, source);
                memmove(ins->content, source, ins->content_size);
                source += ins->content_size;
                index += ins->content_size;
                ++file->count;
            }
            else
            {
                ft_dprintf(STDERR_FILENO, "unknow opcode: %s (%02hhx) index(%lu) length(%lu)\n", cor->header->prog_name, *source, index, cor->length);
                break ;
            }
        }
    }
    return (file);
}

struct s_libcorewar_cor_adn                 *libcorewar_get_cor_adn(void)
{
    struct s_libcorewar_cor_adn *const      file = malloc(sizeof(struct s_libcorewar_cor_adn));
    
    bzero(file, sizeof(struct s_libcorewar_cor_adn));
    return (file);
}

static const struct s_libcorewar_adn_info  g_adn_info[256] =
{
    [0x01] = {1, 0, 0, 0x01, {1},       {T_DIR}},
    [0x02] = {2, 1, 0, 0x02, {0, 1},    {T_DIR | T_IND, T_REG}},
    [0x03] = {2, 1, 0, 0x03, {1, 0},    {T_REG, T_IND | T_REG}},
    [0x04] = {3, 1, 0, 0x04, {1, 1, 1}, {T_REG, T_REG, T_REG}},
    [0x05] = {3, 1, 0, 0x05, {1, 1, 1}, {T_REG, T_REG, T_REG}},
    [0x06] = {3, 1, 0, 0x06, {0, 0, 1}, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}},
    [0x07] = {3, 1, 0, 0x07, {0, 0, 1}, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    [0x08] = {3, 1, 0, 0x08, {0, 0, 1}, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    [0x09] = {1, 0, 1, 0x09, {1},       {T_DIR}},
    [0x0a] = {3, 1, 1, 0x0a, {0, 0, 1}, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    [0x0b] = {3, 1, 1, 0x0b, {1, 0, 0}, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}},
    [0x0c] = {1, 0, 1, 0x0c, {1},       {T_DIR}},
    [0x0d] = {2, 1, 0, 0x0d, {0, 1},    {T_DIR | T_IND, T_REG}},
    [0x0e] = {3, 1, 1, 0x0e, {0, 0, 1}, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    [0x0f] = {1, 0, 1, 0x0f, {1},       {T_DIR}},
    [0x10] = {1, 1, 0, 0x10, {1},       {T_REG}}
};

void                                        libcorewar_get_random_adn_instruction(struct s_libcorewar_adn_instruction *const ins, uint64_t seed)
{
    const struct s_libcorewar_adn_info *const    info = g_adn_info + ((seed % 0x10) + 1);
    uint8_t                                      *dest;
    int                                          index;
    uint8_t                                      encoding;
    uint8_t                                      r;
    uint64_t                                     paramseed;
    static const uint8_t                         type_to_code[] =
    {
        [T_REG] = REG_CODE,
        [T_IND] = IND_CODE,
        [T_DIR] = DIR_CODE
    };
    
    if (!seed)
        seed = 0x4545678987652345;
    dest = ins->content;
    *dest++ = (uint8_t)info->opvalue;
    
    encoding = 0;
    index = 0;
    while (index < info->parameters) {
        encoding <<= 2;
        if (info->parameters_type_isunic[index])
            encoding |= type_to_code[info->parameters_type[index]];
        else {
            paramseed = (seed >> (index << 3));
            r = 0;
            
            while (!(paramseed & info->parameters_type[index]) && r++ < 6)
                paramseed >>= 1;
            if (r >= 6)
                paramseed = T_REG | T_IND | T_DIR;
            if (paramseed & T_REG && info->parameters_type[index] & T_REG)
                encoding |= REG_CODE;
            else if (paramseed & T_IND && info->parameters_type[index] & T_IND)
                encoding |= IND_CODE;
            else
                encoding |= DIR_CODE;
        }
        ++index;
    }
    encoding = encoding << ((MAX_ARGS_NUMBER - info->parameters) << 1);
    
    if (info->parameters_encoding) {
        *dest++ = encoding;
        ins->content_size = 2;
    }
    else
        ins->content_size = 1;
    index = 0;
    while (index < info->parameters) {
        r = (((int)encoding << (index << 1)) >> 6) & 3;
        paramseed = (seed >> (index << 5));
        if (!paramseed)
            paramseed = seed;
        if (r == REG_CODE) {
            *dest++ = paramseed % REG_NUMBER;
            ins->content_size++;
        }
        else if (r == IND_CODE) {
            *(uint16_t*)dest = paramseed % UINT16_MAX;
            dest += IND_SIZE;
            ins->content_size += IND_SIZE;
        }
        else if (r == DIR_CODE) {
            if (info->parameters_direct_small) {
                *(uint16_t*)dest = paramseed % UINT16_MAX;
                dest += IND_SIZE;
                ins->content_size += IND_SIZE;
            }
            else {
                *(uint64_t*)dest = seed % paramseed;
                dest += DIR_SIZE;
                ins->content_size += DIR_SIZE;
            }
        }
        ++index;
    }
    
}
