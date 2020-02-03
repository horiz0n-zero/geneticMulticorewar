//
//  metal.h
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#ifndef metal_h
#define metal_h

//# include <stdint.h>

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

# define REG_NUMBER 16

# define CYCLE_TO_DIE 1536
# define CYCLE_DELTA 50
# define NBR_LIVE 21
# define MAX_CHECKS 10
# define MAX_ARGS_NUMBER 4
# define MAX_PLAYERS 2
# define MEM_SIZE (4 * 1024)
# define IDX_MOD (MEM_SIZE / 8)
# define CHAMP_MAX_SIZE (MEM_SIZE / 6)

# define MAX_PROCS 1000

# define THREADS_W 16
# define THREADS_H 16
# define THREADS_TOTAL (THREADS_W * THREADS_H)
# define GROUPS_W 8
# define GROUPS_H 8
# define GROUPS_TOTAL (GROUPS_W * GROUPS_H)
# define VM_TOTAL (THREADS_TOTAL * GROUPS_TOTAL)

# define POS (gid.x + gid.y * THREADS_H * GROUPS_H)
# define OFFSET (MEM_SIZE / 2)


struct                          vm_opcode_info
{
    const uint16_t              parameters_count;
    const uint16_t              cycles;
    const uint16_t              parameters_encoding;
    const uint16_t              parameters_direct_small;
    const uint16_t              opvalue;
    const uint16_t              carry;
    const uint16_t              parameters_type[MAX_ARGS_NUMBER];
} __attribute__((packed, aligned(2)));
/*
union                           vm_process_value {
    uint64_t                    v;
    struct {
        unsigned int            pc:12;
        unsigned int            pc_tmp:12;
        unsigned int            carry:1;
        
        unsigned int            isalive:1;
        unsigned int            live_id:2;
        unsigned int            champion_id:1;
        
        unsigned int            opcode:4;
        unsigned int            opcode_cycles:10;
        uint8_t                 opcode_parameters;
        # define OPCODE_PARAMETERS_TYPE_1(proc) ((proc->value.bits.opcode_parameters >> 6) & 0b11)
        # define OPCODE_PARAMETERS_TYPE_2(proc) ((proc->value.bits.opcode_parameters >> 4) & 0b11)
        # define OPCODE_PARAMETERS_TYPE_3(proc) ((proc->value.bits.opcode_parameters >> 2) & 0b11)
    } bits;
};*/
struct                          vm_process {
    uint32_t                    registers[REG_NUMBER];
    uint32_t                    args[MAX_ARGS_NUMBER];
    // union vm_process_value      value;
    
} __attribute__((packed, aligned(2)));

struct                          vm_champion {
    uint32_t                    id;
} __attribute__((packed, aligned(2)));

struct                          vm_arena {
   
    uint8_t                     memory[MEM_SIZE];
    struct vm_champion          champion[MAX_PLAYERS];
    
    int32_t                     cycles;
    int32_t                     cycles_to_die;
    int32_t                     checks;
    int32_t                     live_count;
    
    struct vm_process           process[MAX_PROCS];
    uint32_t                    process_count;
} __attribute__((packed, aligned(2)));



#endif /* metal_h */
