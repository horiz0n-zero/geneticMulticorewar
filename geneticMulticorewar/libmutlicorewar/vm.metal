//
//  vm.metal
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#include <metal_stdlib>

using namespace metal;
#include "metal.h"

// MARK: - utility
static inline uint32_t                  get_memory(const device struct vm_arena *const arena, device struct vm_process *const proc,
                                                   uint32_t size) {
    uint32_t                            v = 0;
    
    while (size) {
        v |= ((uint32_t)*(arena->memory + proc->value.bits.pc_tmp++)) & 0xFF;
        if (--size)
            v <<= 8;
    }
    return (v);
}
// FIXME: - need to use pc not tmp_pc ??? change this in instructions and below
static inline uint32_t                  get_memory_indirect(const device struct vm_arena *const arena, device struct vm_process *const proc,
                                                            const uint32_t addr, uint32_t size) {
    uint32_t                            v = 0;
    
    proc->value.bits.pc_tmp += (addr % IDX_MOD);
    while (size) {
        v |= ((uint32_t)*(arena->memory + proc->value.bits.pc_tmp++)) & 0xFF;
        if (--size)
            v <<= 8;
    }
    return (v);
}
static inline void                      set_memory(device struct vm_arena *const arena, device struct vm_process *const proc,
                                                   thread const uint32_t data, uint32_t size) {
    thread const uint8_t                *ptr = (thread const uint8_t *)&data;
    
    while (size--) {
        *(arena->memory + proc->value.bits.pc_tmp++) = *ptr++;
    }
}
static inline void                      set_memory_ataddr(device struct vm_arena *const arena, uint32_t addr,
                                                          thread const uint32_t data, uint32_t size) {
    thread const uint8_t                *ptr = (thread const uint8_t *)&data;
    
    while (size--) {
        *(arena->memory + (addr++ % MEM_SIZE)) = *ptr++;
    }
}
static inline void                      set_memory_indirect(device struct vm_arena *const arena, device struct vm_process *const proc,
                                                            const uint32_t addr, thread const uint32_t data, uint32_t size) {
    thread const uint8_t                *ptr = (thread const uint8_t *)&data;
    
    proc->value.bits.pc_tmp += (addr % IDX_MOD);
    while (size--) {
        *(arena->memory + proc->value.bits.pc_tmp++) = *ptr++;
    }
}
// MARK: process functions
static inline void                      process_init_with_champion(device struct vm_champion *const champion,
                                                                   device struct vm_process *const proc) {
    uint16_t                            index;
    
    proc->args[0] = champion->id;
    proc->value.bits.champion_id = champion->id;
    index = 1;
    while (index < MAX_ARGS_NUMBER)
        proc->args[index++] = 0;
    
}
static inline void                      process_init_with_process(device struct vm_arena *const arena,
                                                                  device struct vm_process *const proc,
                                                                  device struct vm_process *const newproc) {
    uint16_t                            index;
    
    newproc->value.bits = proc->value.bits;
    newproc->value.bits.opcode = 0;
    
    newproc->args[0] = arena->champion->id;
    index = 1;
    while (index < MAX_ARGS_NUMBER) {
        newproc->args[index] = proc->args[index];
        ++index;
    }
}

constant const struct vm_opcode_info    _opcodes_info[] = {
    /* live  */ [0x01] = {1, 10,   0, 0, 0x01, 0, {T_DIR}},
    /* ld    */ [0x02] = {2, 5,    1, 0, 0x02, 0, {T_DIR | T_IND, T_REG}},
    /* st    */ [0x03] = {2, 5,    1, 0, 0x03, 0, {T_REG, T_IND | T_REG}},
    /* add   */ [0x04] = {3, 10,   1, 0, 0x04, 0, {T_REG, T_REG, T_REG}},
    /* sub   */ [0x05] = {3, 10,   1, 0, 0x05, 0, {T_REG, T_REG, T_REG}},
    /* and   */ [0x06] = {3, 6,    1, 0, 0x06, 0, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}},
    /* or    */ [0x07] = {3, 6,    1, 0, 0x07, 0, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    /* xor   */ [0x08] = {3, 6,    1, 0, 0x08, 0, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
    /* zjmp  */ [0x09] = {1, 20,   0, 1, 0x09, 0, {T_DIR}},
    /* ldi   */ [0x0a] = {3, 25,   1, 1, 0x0a, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    /* sti   */ [0x0b] = {3, 25,   1, 1, 0x0b, 0, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}},
    /* fork  */ [0x0c] = {1, 800,  0, 1, 0x0c, 0, {T_DIR}},
    /* lld   */ [0x0d] = {2, 10,   1, 0, 0x0d, 0, {T_DIR | T_IND, T_REG}},
    /* lldi  */ [0x0e] = {3, 50,   1, 1, 0x0e, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
    /* lfork */ [0x0f] = {1, 1000, 0, 1, 0x0f, 0, {T_DIR}},
    /* aff   */ [0x10] = {1, 2,    1, 0, 0x10, 0, {T_REG}}
};

// MARK: run opcode functions
static void                            run_live(device struct vm_arena *const arena, device struct vm_process *const proc) {
    if (!proc->value.bits.isalive)
        proc->value.bits.isalive = 1;
    ++arena->live_count;
}
static void                            run_ld(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v;
    
    if (OPCODE_PARAMETERS_TYPE_1(proc) == REG_CODE)
        v = proc->args[1];
    else
        v = get_memory_indirect(arena, proc, proc->args[0], IND_SIZE);
    proc->registers[proc->args[1]] = v;
    proc->value.bits.carry = !v;
}
static void                            run_st(device struct vm_arena *const arena, device struct vm_process *const proc) {
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        set_memory(arena, proc, proc->args[0], REG_SIZE);
    else
        set_memory_indirect(arena, proc, proc->args[1], proc->args[0], REG_SIZE);
}
static void                            run_add(device struct vm_arena *const arena, device struct vm_process *const proc) {
    proc->registers[proc->args[2]] = proc->registers[proc->args[0]] + proc->registers[proc->args[1]];
}
static void                            run_sub(device struct vm_arena *const arena, device struct vm_process *const proc) {
    proc->registers[proc->args[2]] = proc->registers[proc->args[0]] - proc->registers[proc->args[1]];
}
static void                            run_and(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v1;
    uint32_t                           v2;
    
    if (OPCODE_PARAMETERS_TYPE_1(proc) == REG_CODE)
        v1 = proc->args[0];
    else if (OPCODE_PARAMETERS_TYPE_1(proc) == IND_CODE)
        v1 = get_memory_indirect(arena, proc, proc->args[0], REG_SIZE);
    else
        v1 = proc->registers[proc->args[0]];
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        v2 = proc->args[1];
    else if (OPCODE_PARAMETERS_TYPE_2(proc) == IND_CODE)
        v2 = get_memory_indirect(arena, proc, proc->args[1], REG_SIZE);
    else
        v2 = proc->registers[proc->args[1]];
    proc->registers[proc->args[2]] = v1 & v2;
}
static void                            run_or(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v1;
    uint32_t                           v2;
    
    if (OPCODE_PARAMETERS_TYPE_1(proc) == REG_CODE)
        v1 = proc->args[0];
    else if (OPCODE_PARAMETERS_TYPE_1(proc) == IND_CODE)
        v1 = get_memory_indirect(arena, proc, proc->args[0], REG_SIZE);
    else
        v1 = proc->registers[proc->args[0]];
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        v2 = proc->args[1];
    else if (OPCODE_PARAMETERS_TYPE_2(proc) == IND_CODE)
        v2 = get_memory_indirect(arena, proc, proc->args[1], REG_SIZE);
    else
        v2 = proc->registers[proc->args[1]];
    proc->registers[proc->args[2]] = v1 | v2;
}
static void                            run_xor(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v1;
    uint32_t                           v2;
    
    if (OPCODE_PARAMETERS_TYPE_1(proc) == REG_CODE)
        v1 = proc->args[0];
    else if (OPCODE_PARAMETERS_TYPE_1(proc) == IND_CODE)
        v1 = get_memory_indirect(arena, proc, proc->args[0], REG_SIZE);
    else
        v1 = proc->registers[proc->args[0]];
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        v2 = proc->args[1];
    else if (OPCODE_PARAMETERS_TYPE_2(proc) == IND_CODE)
        v2 = get_memory_indirect(arena, proc, proc->args[1], REG_SIZE);
    else
        v2 = proc->registers[proc->args[1]];
    proc->registers[proc->args[2]] = v1 ^ v2;
}
static void                            run_zjmp(device struct vm_arena *const arena, device struct vm_process *const proc) {
    if (proc->value.bits.carry) {
        proc->value.bits.pc = proc->value.bits.pc + (proc->args[0] % IDX_MOD);
    }
}
static void                            run_ldi(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v1;
    uint32_t                           v2;
    
    if (OPCODE_PARAMETERS_TYPE_1(proc) == REG_CODE)
        v1 = proc->registers[proc->args[0]];
    else if (OPCODE_PARAMETERS_TYPE_1(proc) == IND_CODE)
        v1 = get_memory_indirect(arena, proc, proc->args[0], REG_SIZE);
    else
        v1 = proc->args[0];
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        v2 = proc->registers[proc->args[1]];
    else
        v2 = proc->args[1];
    proc->registers[proc->args[2]] = get_memory_indirect(arena, proc, v1 + v2, REG_SIZE);
} // {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}
static void                            run_sti(device struct vm_arena *const arena, device struct vm_process *const proc) {
    uint32_t                           v1;
    uint32_t                           v2;
    
    if (OPCODE_PARAMETERS_TYPE_2(proc) == REG_CODE)
        v1 = proc->registers[proc->args[1]];
    else if (OPCODE_PARAMETERS_TYPE_2(proc) == DIR_CODE)
        v1 = proc->args[1];
    else
        v1 = get_memory_indirect(arena, proc, proc->args[1], REG_SIZE);
    if (OPCODE_PARAMETERS_TYPE_3(proc) == REG_CODE)
        v2 = proc->registers[proc->args[2]];
    else
        v2 = proc->args[2];
    set_memory_ataddr(arena, v1 + v2, proc->registers[proc->args[0]], REG_SIZE);
}
static void                            run_fork(device struct vm_arena *const arena, device struct vm_process *const proc) {
    
}
static void                            run_lld(device struct vm_arena *const arena, device struct vm_process *const proc) {
    
}
static void                            run_lldi(device struct vm_arena *const arena, device struct vm_process *const proc) {
    
}
static void                            run_lfork(device struct vm_arena *const arena, device struct vm_process *const proc) {
    
}
static void                            run_aff(device struct vm_arena *const arena, device struct vm_process *const proc) {
    
}

typedef void (*opcode_run_function)(device struct vm_arena *const arena, device struct vm_process *const proc);
constant const opcode_run_function      _opcodes_functions[] =
{
    [0x01] = run_live,
    [0x02] = run_ld,
    [0x03] = run_st,
    [0x04] = run_add,
    [0x05] = run_sub,
    [0x06] = run_and,
    [0x07] = run_or,
    [0x08] = run_xor,
    [0x09] = run_zjmp,
    [0x0a] = run_ldi,
    [0x0b] = run_sti,
    [0x0c] = run_fork,
    [0x0d] = run_lld,
    [0x0e] = run_lldi,
    [0x0f] = run_lfork,
    [0x10] = run_aff
};

// MARK: - read opcode
constant const uint16_t                 _parameters_size[] = {
    [REG_CODE] = REG_SIZE,
    [IND_CODE] = IND_SIZE,
    [DIR_CODE] = DIR_SIZE
};
static void                             read_opcode(device struct vm_arena *const arena,
                                                    device struct vm_process *const proc,
                                                    const uint8_t opcode) {
    constant const struct vm_opcode_info  *opcode_info;
    uint16_t                              index;
    uint8_t                               parameters;
    uint8_t                               parameter;
    
    if (opcode > 0 && opcode <= 0x10) {
        opcode_info = _opcodes_info + opcode;
        proc->value.bits.pc_tmp = proc->value.bits.pc;
        
        if (opcode_info->parameters_encoding)
            parameters = *(arena->memory + proc->value.bits.pc_tmp++);
        else
            parameters = REG_CODE << 6;
        proc->value.bits.opcode_parameters = parameters;
        index = 0;
        while (index < opcode_info->parameters_count) {
            parameter = ((parameters << (index << 1)) >> 6) & 0x3;
            if (parameter == REG_CODE && opcode_info->parameters_type[index] & T_REG) {
                proc->args[index] = get_memory(arena, proc, 1);
                if (proc->args[index] >= REG_NUMBER) // FIXME: check in opcode_function ???
                    return ;
            }
            else if (parameter == IND_CODE && opcode_info->parameters_type[index] & T_IND) {
                proc->args[index] = get_memory(arena, proc, IND_SIZE);
            }
            else if (parameter == DIR_CODE && opcode_info->parameters_type[index] & T_DIR) {
                if (opcode_info->parameters_direct_small)
                    proc->args[index] = get_memory(arena, proc, IND_SIZE);
                else
                    proc->args[index] = get_memory(arena, proc, DIR_SIZE);
            }
            else
                return ;
            index++;
        }
        
        proc->value.bits.pc = proc->value.bits.pc_tmp;
        proc->value.bits.opcode = opcode;
        proc->value.bits.opcode_cycles = opcode_info->cycles;
    }
}

// MARK: - vm_run
kernel void                             vm_run(device struct vm_arena *const arenas [[buffer(0)]], const uint2 gid [[thread_position_in_grid]]) {
    
    device struct vm_arena *const        arena = arenas + POS;
    device struct vm_process             *proc = nullptr;
    int32_t                              cycles = 0;
    
    while (1) {
        
        
        // FIXME: iterate procs
        if (!proc->value.bits.opcode)
            read_opcode(arena, proc, *(arena->memory + proc->value.bits.pc++));
        else if (!--proc->value.bits.opcode_cycles) {
            _opcodes_functions[proc->value.bits.opcode](arena, proc);
            proc->value.bits.opcode = 0;
        }
        
        if (++arena->cycles >= arena->cycles_to_die) {
            if (arena->live_count >= NBR_LIVE || arena->checks >= MAX_CHECKS) {
                arena->cycles_to_die -= CYCLE_DELTA;
                if (arena->cycles_to_die < 0) {
                    // TODO: stop vm
                }
                arena->checks = 0;
                // FIXME: iterate procs
                if (!proc->value.bits.isalive)
                    ; // TODO: remove proc
                else
                    proc->value.bits.isalive = 0;
            }
            else {
                // FIXME: iterate procs
                proc->value.bits.isalive = 0;
                ++arena->checks;
            }
            cycles += arena->cycles;
            arena->cycles = 0;
            arena->live_count = 0;
        }
    }
    if (arena->cycles > 0)
        cycles += arena->cycles;
    arena->cycles = cycles; // stats ?
}

// MARK: - vm start
kernel void                             vm_start(device struct vm_arena *const arenas [[buffer(0)]], const uint2 gid [[thread_position_in_grid]]) {
    device struct vm_arena *const        arena = arenas + POS;
    
    arena->cycles_to_die = CYCLE_TO_DIE;
    // FIXME: configure champion
    arena->champion[0].id = 0;
    arena->champion[1].id = 1;
    // FIXME: create first proc
    // first is the last
    process_init_with_champion(arena->champion + 0, nullptr);
    process_init_with_champion(arena->champion + 1, nullptr);
}



