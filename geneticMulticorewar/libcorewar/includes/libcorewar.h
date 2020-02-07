/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 10:05:14 by afeuerst          #+#    #+#             */
/*   Updated: 2020/01/28 08:40:23 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_H
# define LIBCOREWAR_H

# include "libcorewar_shared.h"

# define IND_SIZE 2
# define REG_SIZE 1
# define DIR_SIZE 4

# define REG_CODE 1
# define DIR_CODE 2
# define IND_CODE 3

# define T_REG 1
# define T_DIR 2
# define T_IND 4
# define T_LAB 8

# define COMMENT_CHAR '#'
# define LABEL_CHAR ':'
# define DIRECT_CHAR '%'
# define REG_CHAR 'r'
# define SEPARATOR_CHAR ','

# define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"

# define NAME_CMD_STRING ".name"
# define PROG_NAME_LENGTH (128)
# define COMMENT_CMD_STRING ".comment"
# define COMMENT_LENGTH (2048)

# define REG_NUMBER 16

# define CYCLE_TO_DIE 1536
# define CYCLE_DELTA 50
# define NBR_LIVE 21
# define MAX_CHECKS 10
# define MAX_ARGS_NUMBER 4
# define MAX_PLAYERS 4
# define MEM_SIZE 4096
# define BYTES_LINE 32
# define LINES (MEM_SIZE / BYTES_LINE)
# define IDX_MOD (MEM_SIZE / 8)
# define CHAMP_MAX_SIZE (MEM_SIZE / 6)

# define COREWAR_EXEC_MAGIC 0xea83f3
# define MAX_OPCODE_SIZE (2 + MAX_ARGS_NUMBER * DIR_SIZE)

typedef struct								s_asm_header
{
	unsigned int							magic;
	char									prog_name[PROG_NAME_LENGTH + 1];
	unsigned int							prog_size;
	char									comment[COMMENT_LENGTH + 1];
}											t_asm_header;

/* ************************************************************************** */
/* *** OBJECTS STRUCT *** */

typedef struct s_libcorewar_opcode_info		t_libcorewar_opcode_info;
typedef struct s_libcorewar_opcode_data		t_libcorewar_opcode_data;

struct										s_libcorewar_opcode_info
{
	const char								*name;
	const int								parameters;
	const int								cycles;
	const int								parameters_encoding;
	const int								parameters_direct_small;
	const int								opvalue;
	const int								carry;
	const int								parameters_type[MAX_ARGS_NUMBER];
};

struct										s_libcorewar_opcode_data
{
	const struct s_libcorewar_opcode_info	*info;
	int										types[MAX_ARGS_NUMBER];
	int										params[MAX_ARGS_NUMBER];
};

/* ************************************************************************** */
/* *** MAIN STRUCT *** */

typedef struct s_libcorewar_asm_file		t_libcorewar_asm_file;
typedef struct s_libcorewar_src_file		t_libcorewar_src_file;
typedef struct s_libcorewar_opcode_asm		t_libcorewar_opcode_asm;
typedef struct s_libcorewar_opcode_src		t_libcorewar_opcode_src;

struct										s_libcorewar_opcode_asm
{
	const t_libcorewar_opcode_info			*info;
	char									*start;
	char									*label;
	int										parameters[MAX_ARGS_NUMBER];
	int										parameters_type[MAX_ARGS_NUMBER];
	const void								*parameters_labels[MAX_ARGS_NUMBER];
	struct s_libcorewar_opcode_asm			*next;
};

struct										s_libcorewar_asm_file // source is .cor
{
	char									*content;
	char									*content_end;
	size_t									content_size;
	const char								*labels_prefix;
	int										labels_count;
	int										pad;
	struct s_asm_header						*header;
	struct s_libcorewar_opcode_asm			*opcodes;
};

struct										s_libcorewar_cor_file // source is a valid .cor + padding 4
{
	struct s_asm_header						*header;
	void									*instructions;
	size_t									length;
};

struct										s_libcorewar_opcode_src
{
	char									*label;
	struct s_libcorewar_opcode_src			*oplabel;
	const t_libcorewar_opcode_info			*info;
	int										parameters[MAX_ARGS_NUMBER];
	int										parameters_type[MAX_ARGS_NUMBER];
	char									*parameters_labels[MAX_ARGS_NUMBER];
	struct s_libcorewar_opcode_src			*parameters_oplabels[MAX_ARGS_NUMBER];
	char									*parameters_addrlabels[MAX_ARGS_NUMBER]; // when outputing
	char									*ins; // when output start of opcode
	struct s_libcorewar_opcode_src			*next;
};

struct										s_libcorewar_src_file
{
	char									*content;
	char									*content_end;
	size_t									content_size;
	struct s_asm_header						header;
	struct s_libcorewar_opcode_src			*opcodes;
	size_t									opcodes_count;
};

/* ************************************************************************** */
struct s_libcorewar_asm_file	*libcorewar_get_asm_file(const char *const named, char **const error, const char *const prefix);
void							libcorewar_unset_asm_file(struct s_libcorewar_asm_file *const file);

struct s_libcorewar_cor_file	*libcorewar_get_cor_file(const char *const named, char **const error);
void							libcorewar_unset_cor_file(struct s_libcorewar_cor_file *const file);

struct s_libcorewar_src_file	*libcorewar_get_src_file(const char *const named, char **const error);
void							libcorewar_unset_src_file(struct s_libcorewar_src_file *const file);

// print out
void							libcorewar_out_asm_file(const int fd, struct s_libcorewar_asm_file *const file);
void							libcorewar_out_asm_file_hexcolors(const int fd, struct s_libcorewar_asm_file *const file);
void							libcorewar_out_src_file(const int fd, struct s_libcorewar_src_file *const file, char **const error);

// utility
char							libcorewar_opcode_src_encoded_parameters(struct s_libcorewar_opcode_src *const op) __attribute__((pure));
char							libcorewar_opcode_get_encoded_parameters(struct s_libcorewar_opcode_asm *const op) __attribute__((pure));
void							libcorewar_bswap_asm_file(struct s_libcorewar_asm_file *const file);
void							libcorewar_bswap_src_file(struct s_libcorewar_src_file *const file);

int								libcorewar_get_opcode_data(const char *const start, struct s_libcorewar_opcode_data *const data); // todo

// special

# define MIN_INSTRUCTIONS_SIZE 3
# define MAX_INSTRUCTIONS_SIZE 20
# define MIN_INSTRUCTIONS (CHAMP_MAX_SIZE / MIN_INSTRUCTIONS_SIZE)
# define MAX_INSTRUCTIONS (CHAMP_MAX_SIZE / MAX_INSTRUCTIONS_SIZE)
struct									s_libcorewar_instruction
{
	uint8_t									content[MAX_INSTRUCTIONS_SIZE];
	size_t									content_size;
	const struct s_libcorewar_opcode_info	*info;
};

struct									s_libcorewar_cor_instructions
{
	struct s_libcorewar_instruction		**instructions;
	size_t								instructions_count;
};
struct s_libcorewar_cor_instructions	*libcorewar_get_cor_instructions(struct s_libcorewar_cor_file *const file);
struct s_libcorewar_cor_instructions    *libcorewar_get_cor_instructions_empty_sets(void);
void									libcorewar_unset_cor_instructions(struct s_libcorewar_cor_instructions *const file);



void							*libcorewar_error(char *const ptr, char **const error_ptr, ...);

const char						*libcorewar_color(const unsigned char id);
unsigned char					libcorewar_colorid(const char *const src);
unsigned char					libcorewar_colorid_next(void);
unsigned char					libcorewar_colorid_nextid(const unsigned char id);

/* ************************************************************************** */

typedef struct s_libcorewar_champion		t_libcorewar_champion;
typedef struct s_libcorewar_process			t_libcorewar_process;
typedef struct s_libcorewar_arena			t_libcorewar_arena;
typedef void								(*t_libcorewar_opcode_function)(struct s_libcorewar_arena *const arena,
		struct s_libcorewar_process *const process);

struct										s_libcorewar_champion
{
	int32_t									id;
	int										state;
	const char								*name;
	struct s_libcorewar_asm_file			*file;
	unsigned char							colorid;
};

struct										s_libcorewar_process
{
	int32_t									id;
	int32_t									isalive;
	uint32_t								r[REG_NUMBER];
	uint32_t								pc:12;
	uint32_t								tmpc:12;
	uint32_t								cf;

	int32_t									opcode_cycles;
	struct s_libcorewar_opcode_data			opcode_data;

	struct s_libcorewar_process				*next;
};

struct										s_libcorewar_arena
{
	unsigned char							colorsid[MEM_SIZE];

	char									memory[MEM_SIZE];

	uint32_t								cycles_all; // total number of cycle
	uint32_t								cycles; // current number of cycle 0 ... cycleToDie
	uint32_t								cycle_to_die; // start at CYCLE_TO_DIE, decrement by CYCLE_DELTA

	uint32_t								lives_count;
	uint32_t								checks_count;

	int32_t									liveid; // last live id

	int										champions_count;
	struct s_libcorewar_champion			champions[MAX_PLAYERS];

	struct s_libcorewar_process				*procs; // if input are 0 1 2 3 -> procs are 3 2 1 0

	const t_libcorewar_opcode_function		*functions;
};

struct s_libcorewar_arena					*libcorewar_get_arena(
		const t_libcorewar_opcode_function *const functions,
		char **const error, ...); // [[void*, int32_t] ...]
void										libcorewar_unset_arena(struct s_libcorewar_arena *const arena);

int											libcorewar_arena_cycle(struct s_libcorewar_arena *const arena);
int											libcorewar_arena_cycle_check(struct s_libcorewar_arena *const arena);
// true if continue
// false if finish

struct s_libcorewar_process					*libcorewar_get_process(struct s_libcorewar_champion *const champion);
struct s_libcorewar_process					*libcorewar_unset_process(struct s_libcorewar_process *const process); // return next;

void										libcorewar_dump(const int fd, struct s_libcorewar_arena *const arena);
void										libcorewar_dump_colors(const int fd, struct s_libcorewar_arena *const arena);
void										libcorewar_dump_binary(const int fd, struct s_libcorewar_arena *const arena);
typedef void								(*t_libcorewar_dump_function)(const int fd, struct s_libcorewar_arena *const arena);

// generate & adn instructions

typedef struct s_libcorewar_adn_instruction t_libcorewar_adn_instruction;
struct                                      s_libcorewar_adn_instruction
{
    uint8_t                                 content[MAX_INSTRUCTIONS_SIZE];
    size_t                                  content_size;
    struct s_libcorewar_adn_instruction     *next;
};

struct                                      s_libcorewar_cor_adn
{
    uint8_t                                 memory[CHAMP_MAX_SIZE]; // don't need that ???
    size_t                                  memory_size;
    struct s_libcorewar_adn_instruction     *head;
    size_t                                  count;
};

struct s_libcorewar_cor_adn                 *libcorewar_get_cor_adn_from_cor_file(struct s_libcorewar_cor_file *const cor);
struct s_libcorewar_cor_adn                 *libcorewar_get_cor_adn(void);

struct                                        s_libcorewar_adn_info
{
    const int                                parameters;
    const int                                parameters_encoding;
    const int                                parameters_direct_small;
    const int                                opvalue;
    const int                                parameters_type_isunic[MAX_ARGS_NUMBER];
    const int                                parameters_type[MAX_ARGS_NUMBER];
};

void                                        libcorewar_get_random_adn_instruction(struct s_libcorewar_adn_instruction *const ins, uint64_t seed);


#endif
