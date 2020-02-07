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

// MARK: - memory { get set }
/*static void                      swap(thread char* a, thread char* b)
{
    char tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}
static int32_t                  memory_swap(int32_t data, uint32_t size)
{
    thread char                        *conv = (thread char*)(&data);
    uint32_t                           i, j;

    for (i = 0, j = size - 1; i < j; ++i, --j)
        swap(conv + i, conv + j);
    return (data);
}*/
static int32_t                  get_memory(const device struct vm_arena *const arena, device struct vm_process *const proc,
                                                   uint32_t size) {
    int32_t                            v = 0;
    //const uint32_t                      s = size;
    
    while (size) {
        v |= ((int32_t)*(arena->memory + proc->pc_tmp++)) & 0xFF;
        if (--size)
            v <<= 8;
    }
    return (v);//memory_swap(v, s);
}
static int32_t                  get_memory_indirect(const device struct vm_arena *const arena, device struct vm_process *const proc,
                                                            const int16_t addr, uint32_t size) {
    int32_t                            v = 0;
    //const uint32_t                      s = size;
    
    //if (addr < 0)
      //  proc->pc_tmp -= (uint32_t)((-addr) % IDX_MOD);
    //else
    proc->pc_tmp += (addr % IDX_MOD);
    while (size) {
        v |= ((int32_t)*(arena->memory + proc->pc_tmp++)) & 0xFF;
        if (--size)
            v <<= 8;
    }
    return (v);//memory_swap(v, s);
}
static void                      set_memory_indirect(device struct vm_arena *const arena, device struct vm_process *const proc,
                                                            const int16_t addr, int32_t data, uint32_t size) {
    thread const uint8_t         *ptr = (thread const uint8_t *)&data;

    //data = memory_swap(data, size); DIR_SIZE
    data = ((data << 8) & 0xff00) | ((data >> 8) & 0x00ff);
    //if (addr < 0)
    //    proc->pc_tmp -= (uint32_t)((-addr) % IDX_MOD);
    //else
        proc->pc_tmp += (addr % IDX_MOD);
    while (size--) {
        *(arena->memory + proc->pc_tmp++) = *ptr++;
    }
}
// MARK: process
static void                      process_init_with_champion(device struct vm_arena *const arena,
                                                            device struct vm_champion *const champion,
                                                            device struct vm_process *const proc) {
    arena->last_live_id = champion->number;
    proc->champion_id = champion->id;
    proc->registers[0] = (int32_t)proc->champion_id;
    proc->pc = OFFSETFOR(champion->number);
}
static void                      process_init_with_process(device struct vm_arena *const arena,
                                                           device struct vm_process *const proc,
                                                           device struct vm_process *const newproc) {
    uint16_t                            index = 0;
    
    (void)arena;
    while (index < REG_NUMBER) {
        newproc->registers[index] = proc->registers[index];
        ++index;
    }
    newproc->carry = proc->carry;
    newproc->opcode = 0;
    newproc->opcode_cycles = 0;
    newproc->isalive = proc->isalive;
    newproc->pc = proc->pc;
    newproc->champion_id = proc->champion_id;
}
// return next
static device struct vm_process   *process_delete(device struct vm_process *const target, device struct vm_arena *const arena) {
    device struct vm_process             *ptr = nullptr;

    target->process_isalive = 0;
    if (target->process_next != -1)
    {
        (arena->process + target->process_next)->process_last = target->process_last;
        ptr = arena->process + target->process_next;
    }
    if (target->process_last != -1)
    {
        (arena->process + target->process_last)->process_next = target->process_next;
    }
    if ((target - arena->process) == arena->process_head_index)
    {
        if (target->process_next != -1)
            arena->process_head_index = (uint32_t)target->process_next;
        else if (target->process_last != -1)
            arena->process_head_index = (uint32_t)target->process_last;
        else
            arena->process_head_index = 0;
    }
    target->process_last = -1;
    target->process_next = -1;
    arena->process_count--;
    return (ptr);
}
static device struct vm_process   *process_create(device struct vm_arena *const arena) {
    device struct vm_process             *ptr;
    device struct vm_process *const      end_ptr = arena->process + MAX_PROCS;

    if (arena->process_count >= MAX_PROCS)
        return (nullptr);
    ptr = arena->process;
    while (ptr < end_ptr)
    {
        if (!ptr->process_isalive)
        {
            ptr->process_isalive = 1;
            ptr->process_last = -1;
            ptr->process_next = -1;
            if (arena->process_count)
            {
                ptr->process_next = (int)arena->process_head_index;
                (arena->process_head_index + arena->process)->process_last = (int)(ptr - arena->process);
                arena->process_head_index = (uint32_t)(ptr - arena->process);
            }
            else
                arena->process_head_index = (uint32_t)(ptr - arena->process);
            arena->process_count++;
            return (ptr);
        }
        ++ptr;
    }
    return (nullptr);
}
static void                              process_cleaner(device struct vm_arena *const arena) {
    device struct vm_process             *proc;
    
    if (arena->process_count) {
        proc = arena->process + arena->process_head_index;
        while (proc && proc->process_isalive) { // rm -> (1)
            if (proc->isalive) {
                proc->isalive = 0;
                if (proc->process_next == -1)
                    break ;
                else
                    proc = arena->process + proc->process_next;
            }
            else
                proc = process_delete(proc, arena);
        }
    }
}

// MARK: - read opcode
constant struct vm_opcode_info    _opcodes_info[] = {
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
static uint16_t                        opcode_read_parameters(device struct vm_arena *const arena, device struct vm_process *const proc,
                                                             thread int32_t *const params, thread uint16_t *const params_type,
                                                             thread uint16_t *const params_size) {
    constant struct vm_opcode_info *const      opcode_info = _opcodes_info + proc->opcode;
    uint16_t                                index = 0;
    uint8_t                                 parameters;
    uint8_t                                 parameter;
    uint16_t                                ret = 1;
    
    proc->pc_tmp = proc->pc + 1;
    if (opcode_info->parameters_encoding)
        parameters = *(arena->memory + proc->pc_tmp++);
    else
        parameters = DIR_CODE << 6;
    while (index < opcode_info->parameters_count) {
        parameter = ((parameters << (index << 1)) >> 6) & 0x3;
        if (parameter == REG_CODE && opcode_info->parameters_type[index] & T_REG) {
            params[index] = get_memory(arena, proc, REG_SIZE);
            params_type[index] = REG_CODE;
            *params_size += REG_SIZE;
        }
        else if (parameter == IND_CODE && opcode_info->parameters_type[index] & T_IND) {
            params[index] = (int16_t)get_memory(arena, proc, IND_SIZE);
            params_type[index] = IND_CODE;
            *params_size += IND_SIZE;
        }
        else if (parameter == DIR_CODE && opcode_info->parameters_type[index] & T_DIR) {
            if (opcode_info->parameters_direct_small) {
                params[index] = (int16_t)get_memory(arena, proc, IND_SIZE);
                *params_size += IND_SIZE;
            }
            else {
                params[index] = get_memory(arena, proc, DIR_SIZE);
                *params_size += DIR_SIZE;
            }
            params_type[index] = DIR_CODE;
        }
        else
            ret = 0;
        ++index;
    }
    return (1);
}

// MARK: - run opcode functions
static void                            run_live(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           param;
    
    proc->pc_tmp = proc->pc + 1;
    param = get_memory(arena, proc, DIR_SIZE);
    
    if (!proc->isalive)
        proc->isalive = 1;
    ++arena->live_count;
    
    if (arena->champion[0].id == (uint32_t)param)
        arena->last_live_id = arena->champion[0].number;
    else if (arena->champion[1].id == (uint32_t)param)
        arena->last_live_id = arena->champion[1].number;
    
    proc->pc += 5;
}
// {2, 5,    1, 0, 0x02, 0, {T_DIR | T_IND, T_REG}},
static void                            run_ld(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[1])) {
        if (params_type[0] == DIR_CODE) {
            proc->registers[params[1] - 1] = params[0];
        }
        else {
            proc->pc_tmp = proc->pc;
            proc->registers[params[1] - 1] = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        SETCARRY(proc->registers[params[1] - 1]);
    }
    proc->pc += params_size;
}
// {2, 5,    1, 0, 0x03, 0, {T_REG, T_IND | T_REG}},
static void                            run_st(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                            params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[0])) {
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1])) {
            proc->registers[params[1] - 1] = proc->registers[params[0] - 1];
        }
        else if (params_type[1] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            set_memory_indirect(arena, proc, (int16_t)params[1], proc->registers[params[0] - 1], DIR_SIZE);
        }
    }
    proc->pc += params_size;
}
// {3, 10,   1, 0, 0x04, 0, {T_REG, T_REG, T_REG}}
static void                            run_add(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) &&
        REGISTER_VALID(params[0]) && REGISTER_VALID(params[1]) && REGISTER_VALID(params[2])) {
        v = proc->registers[params[0] - 1] + proc->registers[params[1] - 1];
        proc->registers[params[2] - 1] = v;
        SETCARRY(v);
    }
    proc->pc += params_size;
}
static void                            run_sub(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) &&
        REGISTER_VALID(params[0]) && REGISTER_VALID(params[1]) && REGISTER_VALID(params[2])) {
        v = proc->registers[params[0] - 1] - proc->registers[params[1] - 1];
        proc->registers[params[2] - 1] = v;
        SETCARRY(v);
    }
    proc->pc += params_size;
}
// {3, 6,    1, 0, 0x06, 0, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}}
static void                            run_and(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v1;
    int32_t                           v2;
    int32_t                           v3;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[2])) {
        if (params_type[0] == REG_CODE && REGISTER_VALID(params[0]))
            v1 = proc->registers[params[0] - 1];
        else if (params_type[0] == DIR_CODE)
            v1 = params[0];
        else if (params_type[0] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v2 = proc->registers[params[1] - 1];
        else if (params_type[1] == DIR_CODE)
            v2 = params[1];
        else if (params_type[1] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v2 = get_memory_indirect(arena, proc, (int16_t)params[1], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        v3 = v1 & v2;
        proc->registers[params[2] - 1] = v3;
        SETCARRY(v3);
    }
    proc->pc += params_size;
}
static void                            run_or(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v1;
    int32_t                           v2;
    int32_t                           v3;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[2])) {
        if (params_type[0] == REG_CODE && REGISTER_VALID(params[0]))
            v1 = proc->registers[params[0] - 1];
        else if (params_type[0] == DIR_CODE)
            v1 = params[0];
        else if (params_type[0] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v2 = proc->registers[params[1] - 1];
        else if (params_type[1] == DIR_CODE)
            v2 = params[1];
        else if (params_type[1] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v2 = get_memory_indirect(arena, proc, (int16_t)params[1], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        v3 = v1 | v2;
        proc->registers[params[2] - 1] = v3;
        SETCARRY(v3);
    }
    proc->pc += params_size;
}
static void                            run_xor(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v1;
    int32_t                           v2;
    int32_t                           v3;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[2])) {
        if (params_type[0] == REG_CODE && REGISTER_VALID(params[0]))
            v1 = proc->registers[params[0] - 1];
        else if (params_type[0] == DIR_CODE)
            v1 = params[0];
        else if (params_type[0] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v2 = proc->registers[params[1] - 1];
        else if (params_type[1] == DIR_CODE)
            v2 = params[1];
        else if (params_type[1] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v2 = get_memory_indirect(arena, proc, (int16_t)params[1], DIR_SIZE);
        }
        else {
            proc->pc += params_size;
            return ;
        }
        v3 = v1 ^ v2;
        proc->registers[params[2] - 1] = v3;
        SETCARRY(v3);
    }
    proc->pc += params_size;
}
// {1, 20,   0, 1, 0x09, 0, {T_DIR}
static void                            run_zjmp(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                            params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 1;
    
    if (proc->carry && opcode_read_parameters(arena, proc, params, params_type, &params_size)) {
        proc->pc += ((int16_t)params[0] % IDX_MOD);
    }
    else {
        proc->pc += 3;
    }
}
// {3, 25,   1, 1, 0x0a, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}}
static void                            run_ldi(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v1;
    int32_t                           v2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[2])) {
        if (params_type[0] == REG_CODE && REGISTER_VALID(params[0]))
            v1 = proc->registers[params[0] - 1];
        else if (params_type[0] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        else if (params_type[0] == DIR_CODE)
            v1 = params[0];
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v2 = proc->registers[params[1] - 1];
        else if (params_type[1] == DIR_CODE)
            v2 = params[1];
        else {
            proc->pc += params_size;
            return ;
        }
        proc->registers[params[2] - 1] = get_memory_indirect(arena, proc, (int16_t)(v1 + v2), DIR_SIZE);
    }
    proc->pc += params_size;
}
// {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}
static void                            run_sti(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                           v1;
    int32_t                           v2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[0])) {
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v1 = proc->registers[params[1] - 1];
        else if (params_type[1] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[1], DIR_SIZE);
        }
        else if (params_type[1] == DIR_CODE)
            v1 = params[1];
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[2] == REG_CODE && REGISTER_VALID(params[2]))
            v2 = proc->registers[params[2] - 1];
        else if (params_type[2] == DIR_CODE)
            v2 = params[2];
        else {
            proc->pc += params_size;
            return ;
        }
        proc->pc_tmp = proc->pc;
        set_memory_indirect(arena, proc, (int16_t)(v1 + v2), proc->registers[params[0] - 1], DIR_SIZE);
    }
    proc->pc += params_size;
    // seem p1 + p2 -> p3 not p2 + p3 -> p1
}
// {1, 800,  0, 1, 0x0c, 0, {T_DIR}}
static void                            run_fork(device struct vm_arena *const arena, device struct vm_process *const proc) {
    device struct vm_process *const           newproc = process_create(arena);
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 0;
    
    if (newproc && opcode_read_parameters(arena, proc, params, params_type, &params_size)) {
        process_init_with_process(arena, proc, newproc);
        newproc->pc = proc->pc + (params[0] % IDX_MOD);
    }
    proc->pc += 3;
}
// {2, 10,   1, 0, 0x0d, 0, {T_DIR | T_IND, T_REG}}
static void                            run_lld(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[1])) {
        if (params_type[0] == DIR_CODE)
            proc->registers[params[1] - 1] = params[0];
        else {
            proc->pc_tmp = proc->pc;
            proc->registers[params[1] - 1] = (int16_t)get_memory(arena, proc, IND_SIZE);
        }
        SETCARRY(proc->registers[params[1] - 1]);
    }
    proc->pc += params_size;
}
// {3, 50,   1, 1, 0x0e, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}}
static void                            run_lldi(device struct vm_arena *const arena, device struct vm_process *const proc) {
    int32_t                            params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 2;
    int32_t                            v1;
    int32_t                            v2;
    
    if (opcode_read_parameters(arena, proc, params, params_type, &params_size) && REGISTER_VALID(params[2])) {
        if (params_type[0] == REG_CODE && REGISTER_VALID(params[0]))
            v1 = proc->registers[params[0] - 1];
        else if (params_type[0] == IND_CODE) {
            proc->pc_tmp = proc->pc;
            v1 = get_memory_indirect(arena, proc, (int16_t)params[0], DIR_SIZE);
        }
        else if (params_type[0] == DIR_CODE)
            v1 = params[0];
        else {
            proc->pc += params_size;
            return ;
        }
        if (params_type[1] == REG_CODE && REGISTER_VALID(params[1]))
            v2 = proc->registers[params[1] - 1];
        else if (params_type[1] == DIR_CODE)
            v2 = params[1];
        else {
            proc->pc += params_size;
            return ;
        }
        proc->pc_tmp = proc->pc + (uint32_t)(v1 + v2);
        proc->registers[params[2] - 1] = get_memory(arena, proc, DIR_SIZE);
        SETCARRY(proc->registers[params[2] - 1]);
    }
    proc->pc += params_size;
}
static void                            run_lfork(device struct vm_arena *const arena, device struct vm_process *const proc) {
    device struct vm_process *const           newproc = process_create(arena);
    int32_t                           params[MAX_ARGS_NUMBER];
    uint16_t                           params_type[MAX_ARGS_NUMBER];
    uint16_t                           params_size = 0;
    
    if (newproc && opcode_read_parameters(arena, proc, params, params_type, &params_size)) {
        process_init_with_process(arena, proc, newproc);
        newproc->pc = proc->pc + (uint16_t)params[0];
    }
    proc->pc += 3;
}
static void                            run_aff(device struct vm_arena *const arena, device struct vm_process *const proc) {
    (void)arena;
    proc->pc += 3;
}

// MARK: - vm_run
static void                             vm_stop(device struct vm_arena *const arena, const int32_t cycles) {
    if (arena->cycles > 0)
        arena->cycles += cycles;
    else
        arena->cycles = cycles;
    if (arena->last_live_id == arena->champion[0].id)
        arena->last_live_id = arena->champion[0].number;
    else
        arena->last_live_id = arena->champion[1].number;
}

static void                      vm_start(device struct vm_arena *const arena)
{
    device struct vm_process            *proc;
    
    arena->cycles_to_die = CYCLE_TO_DIE;
    arena->champion[0].number = CHAMPION_ONE;
    arena->champion[1].number = CHAMPION_TWO;
    arena->champion[0].id = (uint32_t)(CHAMPION_ONE * -1);
    arena->champion[1].id = (uint32_t)(CHAMPION_TWO * -1);
    proc = process_create(arena);
    process_init_with_champion(arena, arena->champion + 0, proc);
    proc = process_create(arena);
    process_init_with_champion(arena, arena->champion + 1, proc);
}

kernel void                          vm_run(device struct vm_arena *const arenas [[buffer(0)]], const uint2 gid [[thread_position_in_grid]]) {
    device struct vm_arena *const        arena = arenas + POS(gid.x, gid.y);
    device struct vm_process            *proc;
    int32_t                      cycles = 0;
    uint8_t                      opcode;
    
    vm_start(arena);
    while (1) {
        proc = arena->process + arena->process_head_index;
        while (1) {
            if (!proc->opcode) {
                opcode = *(arena->memory + proc->pc);
                if (opcode > 0 && opcode <= 0x10) {
                    proc->opcode = opcode;
                    proc->opcode_cycles = (_opcodes_info + opcode)->cycles - 1;
                }
                else {
                    ++proc->pc;
                }
            }
            else if (!--proc->opcode_cycles) {
                switch(proc->opcode) {
                  case 0x01:
                        run_live(arena, proc);
                        break;
                  case 0x02:
                        run_ld(arena, proc);
                        break;
                  case 0x03:
                        run_st(arena, proc);
                        break;
                  case 0x04:
                        run_add(arena, proc);
                        break;
                  case 0x05:
                        run_sub(arena, proc);
                        break;
                  case 0x06:
                        run_and(arena, proc);
                        break;
                  case 0x07:
                        run_or(arena, proc);
                        break;
                  case 0x08:
                        run_xor(arena, proc);
                        break;
                  case 0x09:
                        run_zjmp(arena, proc);
                        break;
                  case 0x0a:
                        run_ldi(arena, proc);
                        break;
                  case 0x0b:
                        run_sti(arena, proc);
                        break;
                  case 0x0c:
                        run_fork(arena, proc);
                        break;
                  case 0x0d:
                        run_lld(arena, proc);
                        break;
                  case 0x0e:
                        run_lldi(arena, proc);
                        break;
                  case 0x0f:
                        run_lfork(arena, proc);
                        break;
                  default:
                        run_aff(arena, proc);
                        break ;
                }
                
                proc->opcode = 0;
            }
            
            if (proc->process_next != -1)
                proc = arena->process + proc->process_next;
            else
                break ;
        }
        if (++arena->cycles >= arena->cycles_to_die) {
            process_cleaner(arena);
            if (arena->live_count >= NBR_LIVE || ++arena->checks >= MAX_CHECKS) {
                arena->cycles_to_die -= CYCLE_DELTA;
                if (arena->cycles_to_die < 0)
                    return vm_stop(arena, cycles);
                arena->checks = 0;
            }
            cycles += arena->cycles;
            arena->cycles = 0;
            arena->live_count = 0;
            if (!arena->process_count)
                return vm_stop(arena, cycles);
        }
    }
}

