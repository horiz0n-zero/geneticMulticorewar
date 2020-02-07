//
//  Multicorewar.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Multicorewar.h"
#include <dirent.h>
#include <objc/runtime.h>

static int                           _seed_fd_source = 0;

@interface Opponent : NSObject
@property(nonatomic, strong) NSString *source;
@property(nonatomic, strong) NSString *name;
@property(nonatomic, assign) struct s_libcorewar_cor_file *file;

@end @implementation Opponent

- (instancetype)init:(NSString *const)source
{
    self = [super init];
    char *error = nil;
    
    [self setSource:source];
    [self setName:[source lastPathComponent]];
    [self setFile:libcorewar_get_cor_file([[self source] cStringUsingEncoding:NSUTF8StringEncoding], &error)];
    if (error) {
        [[Utility shared] fatalError:nil msg:[[NSString alloc] initWithFormat:@"%@: %s", self.source, error]];
    }
    return self;
}

@end
@interface Champion : NSObject {
    uint8_t                                 instructions[CHAMP_MAX_SIZE];
    NSUInteger                              instructions_size;
    struct s_libcorewar_cor_adn             *file;
}
- (uint8_t *)instructions;
- (NSUInteger)instructions_size;
- (void)fillInstructions:(const BOOL)fillWithZero;
- (float)winPercent;
@property(nonatomic, assign) NSUInteger winCount;

@end @implementation Champion
- (uint8_t *)instructions {
    return self->instructions;
}
- (NSUInteger)instructions_size {
    return self->instructions_size;
}
- (instancetype)init:(NSString *const)source {
    self = [super init];
    char *error = nil;
    struct s_libcorewar_cor_file *const cor = libcorewar_get_cor_file([source cStringUsingEncoding:NSUTF8StringEncoding], &error);
    
    if (error) {
        [[Utility shared] fatalError:nil msg:[[NSString alloc] initWithFormat:@"%@: %s", source, error]];
    }
    self->file = libcorewar_get_cor_adn_from_cor_file(cor);
    bzero(self->instructions, self->instructions_size);
    memmove(self->instructions, cor->instructions, cor->length);
    self->instructions_size = cor->length;
    libcorewar_unset_cor_file(cor);
    return self;
}
- (instancetype)init {
    self = [super init];
    self->file = libcorewar_get_cor_adn();
    size_t                              champs_size = 0;
    struct s_libcorewar_adn_instruction *ins;
    struct s_libcorewar_adn_instruction *head = NULL;
    char                                buffer[sizeof(uint64_t) * MAX_INSTRUCTIONS];
    uint64_t                            *seed = (uint64_t*)buffer;
    
    read(_seed_fd_source, buffer, sizeof(uint64_t) * MAX_INSTRUCTIONS);
    while (champs_size < CHAMP_MAX_SIZE) {
        ins = malloc(sizeof(struct s_libcorewar_adn_instruction));
        bzero(ins, sizeof(struct s_libcorewar_adn_instruction));
        libcorewar_get_random_adn_instruction(ins, *seed++);
        if (champs_size + ins->content_size >= CHAMP_MAX_SIZE) {
            free(ins);
            break ;
        }
        champs_size += ins->content_size;
        if (!self->file->head) {
            self->file->head = ins;
            head = ins;
        }
        else {
            head->next = ins;
            head = ins;
        }
        self->file->count++;
    }
    [self fillInstructions:NO];
    
    return self;
}
- (instancetype)initWithMaman:(Champion *const)maman andPapa:(Champion *const)papa {
    self = [super init];
    self->file = libcorewar_get_cor_adn();
    size_t                              champs_size = 0;
    struct s_libcorewar_adn_instruction *ins;
    struct s_libcorewar_adn_instruction *head = NULL;
    struct s_libcorewar_adn_instruction *maman_ins;
    struct s_libcorewar_adn_instruction *papa_ins;
    
    maman_ins = maman->file->head;
    papa_ins = papa->file->head;
    while (champs_size < CHAMP_MAX_SIZE) {
        ins = malloc(sizeof(struct s_libcorewar_adn_instruction));
        bzero(ins, sizeof(struct s_libcorewar_adn_instruction));
        
        if (arc4random_uniform(5) && maman_ins) { // maman ins
            memcpy(ins->content, maman_ins->content, maman_ins->content_size);
            ins->content_size = maman_ins->content_size;
            // mutation ??
            maman_ins = maman_ins->next;
        }
        else {
            if (papa_ins) {
                memcpy(ins->content, papa_ins->content, papa_ins->content_size);
                ins->content_size = papa_ins->content_size;
                papa_ins = papa_ins->next;
            }
            else if (!maman_ins)
                break ;
        }
        
        if (champs_size + ins->content_size >= CHAMP_MAX_SIZE) {
            free(ins);
            break ;
        }
        champs_size += ins->content_size;
        if (!self->file->head) {
            self->file->head = ins;
            head = ins;
        }
        else {
            head->next = ins;
            head = ins;
        }
        self->file->count++;
    }
    [self fillInstructions:NO];
    
    return self;
}
- (void)fillInstructions:(const BOOL)fillWithZero {
    struct s_libcorewar_adn_instruction *head;
    uint8_t                             *mem;
    
    if (fillWithZero)
        bzero(self->instructions, CHAMP_MAX_SIZE);
    head = self->file->head;
    mem = self->instructions;
    self->instructions_size = 0;
    while (head) {
        memmove(mem, head->content, head->content_size);
        mem += head->content_size;
        self->instructions_size += head->content_size;
        head = head->next;
    }
}
- (float)winPercent {
    return ((float)self.winCount / (float)(GROUPS_TOTAL * 2)) * 100.0;
}
@end

@interface Multicorewar () {
    NSMutableArray<Opponent *> *_opponents;
    NSMutableArray<Champion *> *_champions;
    id <MTLCommandBuffer>      _commandBuffer;
}
@end

@implementation Multicorewar

- (instancetype)init
{
    self = [super init];
    [self initMetal];
    
    mkdir(g_information.output_directory, CREATE_DIR);
    
    // MARK: - read opponents
    DIR *directory = opendir(g_information.opponents_directory);
    struct dirent *dirent;
    
    self->_opponents = [[NSMutableArray alloc] initWithCapacity:GROUPS_TOTAL];
    if (!directory)
        [[Utility shared] fatalErrno:[[NSString alloc] initWithFormat:@"opendir(%s)", g_information.opponents_directory]];
    while ((dirent = readdir(directory))) {
        if (!(dirent->d_type & DT_REG))
            continue ;
        [self->_opponents addObject:[[Opponent alloc] init:[[NSString alloc] initWithFormat:@"%s/%s", g_information.opponents_directory, dirent->d_name]]];
    }
    if ([self->_opponents count] != GROUPS_TOTAL)
        [[Utility shared] fatalError:nil msg:[[NSString alloc] initWithFormat:@"%s doesn't contain %d champion",
                                              g_information.opponents_directory, GROUPS_TOTAL]];
    [[Utility shared] messageCPU:"ok: %s: %lu/%d opponent loaded\n", g_information.opponents_directory, [self->_opponents count], GROUPS_TOTAL];
    closedir(directory);
    // MARK: - read sources and/or generate random champion
    NSUInteger index = 0;
    NSString   *source;
    _seed_fd_source = open("/dev/urandom", O_RDONLY);
    
    self->_champions = [[NSMutableArray alloc] initWithCapacity:THREADS_TOTAL];
    if (g_information.flags & FLAGS_S) {
        directory = opendir(g_information.sources_directory);
        if (!directory)
            [[Utility shared] fatalErrno:[[NSString alloc] initWithFormat:@"opendir(%s)", g_information.sources_directory]];
        while ((dirent = readdir(directory)) && index < THREADS_TOTAL) {
            if (!(dirent->d_type & DT_REG))
                continue ;
            source = [[NSString alloc] initWithFormat:@"%s/%s", g_information.sources_directory, dirent->d_name];
            [self->_champions addObject:[[Champion alloc] init:source]];
            ++index;
        }
        ft_printf("ok: %s: %lu champion loaded\n", g_information.sources_directory, [self->_champions count]);
        closedir(directory);
    }
    while (index < THREADS_TOTAL) {
        [self->_champions addObject:[[Champion alloc] init]];
        ++index;
    }
    [[Utility shared] messageCPU:"ok: generate %d champions\n", THREADS_TOTAL];
    self->_bounds = [[Bounds alloc] init:self];
    [self objectSee:self->_device];
    [self startMachine];
    return self;
}

- (void)writeChampions:(NSString *const)directory_target {
    int                 index = 0;
    char                *path;
    char                *file;
    int                 fd;
    struct s_asm_header header;
    Champion            *champion;
    
    ft_asprintf(&path, "%s/%s/", g_information.output_directory, [directory_target cStringUsingEncoding:NSUTF8StringEncoding]);
    mkdir(path, CREATE_DIR);
    [[Utility shared] messageCPU:"saving champion in %s\n", path];
    while (index < THREADS_TOTAL) {
        champion = self->_champions[index];
        ft_asprintf(&file, "%schampion_%d.cor", path, index);
        if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, CREATE_FILE)) < 0) {
            ft_dprintf(STDERR_FILENO, "%s: %s\n", file, strerror(errno));
        }
        else {
            bzero(&header, sizeof(struct s_asm_header));
            header.magic = OSSwapInt32(COREWAR_EXEC_MAGIC);
            header.prog_size = OSSwapInt32([champion instructions_size]);
            memmove(header.prog_name, file, strlen(file));
            memmove(header.comment, file, strlen(file));
            write(fd, &header, sizeof(struct s_asm_header));
            write(fd, [champion instructions], [champion instructions_size]);
        }
        close(fd);
        free(file);
        ++index;
    }
    ft_printf("done\n");
    free(path);
}

- (void)printChampionAt:(const NSInteger)champion_index opponentIndex:(const NSInteger)opponent_index {
    Champion *const        champion = [self->_champions objectAtIndex:champion_index];
    Opponent *const        opponent = [self->_opponents objectAtIndex:opponent_index];
    struct vm_arena *const arena = ((struct vm_arena *)self->_bounds.buffer.contents) + POS(champion_index, opponent_index);
    
    ft_printf("result for champion at (%ld, %ld)\n", champion_index, opponent_index);
    [[Utility shared] printArenaMemory:arena->memory];
    ft_printf("cycles: %d, process_count: %d\n", arena->cycles, arena->process_count);
    ft_printf("winner is %d == %d\n", arena->last_live_id, arena->champion_number);
    
}

- (void)printChampionsWin {
    int                           champion_index;
    Champion                      *champion;
    int                           opponent_index;
    Opponent                      *opponent;
    struct vm_arena               *arena;
    int                           winCount = 0;
    
    champion_index = 0;
    while (champion_index < THREADS_TOTAL) {
        champion = [self->_champions objectAtIndex:champion_index];
        opponent_index = 0;
        while (opponent_index < GROUPS_TOTAL) {
            opponent = [self->_opponents objectAtIndex:opponent_index];
            arena = ((struct vm_arena*)self->_bounds.buffer.contents) + POS(champion_index, opponent_index);
            if (arena->last_live_id == arena->champion_number) {
                winCount++;
            }
            ++opponent_index;
        }
        ++champion_index;
    }
    ft_printf("total win: %d\n", winCount);
}

static int _generation = 0;
- (void)startMachine {
    id <MTLCommandBuffer> commandBuffer = [self encodeCommandBuffer:YES];

    return [self objectSee:commandBuffer];
    [self->_champions makeObjectsPerformSelector:@selector(setWinCount:) withObject:NULL];
    [commandBuffer addScheduledHandler:^(id <MTLCommandBuffer> buffer){
        [[Utility shared] messageGPU:"commandBuffer scheduled\n"];
    }];
    [commandBuffer addCompletedHandler:^(id <MTLCommandBuffer> buffer){
        [[Utility shared] messageGPU:"commandBuffer completed\n"];
    }];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    if (commandBuffer.error) {
        [[Utility shared] fatalError:commandBuffer.error msg:@"Command Buffer"];
    }
    [self setWinsCount];
    commandBuffer = [self encodeCommandBuffer:NO];
    [commandBuffer addScheduledHandler:^(id <MTLCommandBuffer> buffer){
        [[Utility shared] messageGPU:"commandBuffer scheduled\n"];
    }];
    [commandBuffer addCompletedHandler:^(id <MTLCommandBuffer> buffer){
        [[Utility shared] messageGPU:"commandBuffer completed %f\n"];
    }];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    if (commandBuffer.error) {
        [[Utility shared] fatalError:commandBuffer.error msg:@"Command Buffer"];
    }
    [self setWinsCount];
    [self->_champions sortUsingComparator:^(Champion *const c1, Champion *const c2){
        if (c1.winCount < c2.winCount) {
            return (NSComparisonResult)NSOrderedDescending;
        }
        return (NSComparisonResult)NSOrderedAscending;
    }];
    printf("best champion: %lu %f\n", [self->_champions objectAtIndex:0].winCount, [self->_champions objectAtIndex:0].winPercent);
    [self writeChampions:[[NSString alloc] initWithFormat:@"%d", _generation++]];
    [self geneticsSelection];
    [self startMachine];
}
- (void)geneticsSelection {
    const int      killIndex = 100;
    int            index = killIndex;
    Champion       *maman;
    Champion       *papa;
    
    [[Utility shared] messageCPU:"starting genetic selection at %d\n", killIndex];
    index = 0;
    while (index < THREADS_TOTAL) {
        maman = [self->_champions objectAtIndex:(NSUInteger)arc4random_uniform((uint32_t)killIndex)];
        papa = [self->_champions objectAtIndex:(NSUInteger)arc4random_uniform((uint32_t)killIndex)];
        [self->_champions setObject:[[Champion alloc] initWithMaman:maman andPapa:papa] atIndexedSubscript:index];
        ++index;
    }
    [[Utility shared] messageCPU:"done: genetic selection"];
}
- (void)setWinsCount {
    int                           champion_index;
    Champion                      *champion;
    int                           opponent_index;
    Opponent                      *opponent;
    struct vm_arena               *arena;
    
    champion_index = 0;
    while (champion_index < THREADS_TOTAL) {
        champion = [self->_champions objectAtIndex:champion_index];
        opponent_index = 0;
        while (opponent_index < GROUPS_TOTAL) {
            opponent = [self->_opponents objectAtIndex:opponent_index];
            arena = ((struct vm_arena*)self->_bounds.buffer.contents) + POS(champion_index, opponent_index);
            if (arena->last_live_id == arena->champion_number) {
                champion.winCount++;
            }
            ++opponent_index;
        }
        ++champion_index;
    }
}

- (void)objectSee:(const id)object {
    Ivar                  *ptr_ivar = nil;
    unsigned int          count_ivar = 0;
    objc_property_t       *ptr_property = nil;
    unsigned int          count_property = 0;
    Method                *ptr_method = nil;
    unsigned int          count_method = 0;
    
    unsigned int index;
    ft_printf("objectSee: %s\n", object_getClassName(object));
    ptr_ivar = class_copyIvarList([object class], &count_ivar);
    index = 0;
    while (index < count_ivar) {
        ft_printf("ivar: %s\n", ivar_getName(*ptr_ivar++));
        ++index;
    }
    ptr_property = class_copyPropertyList([object class], &count_property);
    index = 0;
    while (index < count_property) {
        ft_printf("property: %s\n", property_getName(*ptr_property++));
        ++index;
    }
    ptr_method = class_copyMethodList([object class], &count_method);
    index = 0;
    while (index < count_method) {
        ft_printf("method: %s\n", sel_getName(method_getName(*ptr_method++)));
        ++index;
    }
}







- (void)initMetal {
    NSError *error = nil;
    
    self->_device = MTLCreateSystemDefaultDevice();
    self->_commandQueue = [self->_device newCommandQueue];
    
    self->_library = [self->_device newDefaultLibrary];
    if (!self->_library)
        [[Utility shared] fatalError:nil msg:@"lib"];
    //self->_pipelineStateStart = [self->_device newComputePipelineStateWithFunction:[self->_library newFunctionWithName:@"vm_start"]
    //                                                                       error:&error];
    //if (error)
    //    [[Utility shared] fatalError:error msg:@"vm_start"];
    self->_pipelineStateRun = [self->_device newComputePipelineStateWithFunction:[self->_library newFunctionWithName:@"vm_run"]
                                                                           error:&error];
    if (error)
        [[Utility shared] fatalError:error msg:@"vm_run"];
    [[Utility shared] messageCPU:"ok: metal device\nok: metal library\nok: metal compute pipeline state\n"];
}
- (id<MTLDevice>)device {
    return self->_device;
}

- (id <MTLCommandBuffer>)encodeCommandBuffer:(const BOOL)champion_first {
    const id <MTLCommandBuffer>   commandBuffer = [self->_commandQueue commandBuffer];
    id <MTLComputeCommandEncoder> computeEncoder;
    int                           champion_index;
    Champion                      *champion;
    int                           opponent_index;
    Opponent                      *opponent;
    size_t                        offset;
    uint8_t                       *ptr;
    
    
    [[Utility shared] messageCPU:"prepare arenas memory\n"];
    bzero(self->_bounds.buffer.contents, g_information.arenas_memory_size);
    champion_index = 0;
    while (champion_index < THREADS_TOTAL) {
        champion = [self->_champions objectAtIndex:champion_index];
        opponent_index = 0;
        while (opponent_index < GROUPS_TOTAL) {
            opponent = [self->_opponents objectAtIndex:opponent_index];
            offset = sizeof(struct vm_arena) * POS(champion_index, opponent_index); // MARK: - POS(champion_index, opponent_index)
            ptr = ((uint8_t*)self->_bounds.buffer.contents) + offset;
            if (champion_first) {
                memmove(ptr, champion.instructions, champion.instructions_size);
                memmove(ptr + OFFSETFOR(CHAMPION_TWO), opponent.file->instructions, opponent.file->length);
                ((struct vm_arena*)ptr)->champion_number = CHAMPION_ONE;
            }
            else {
                memmove(ptr, opponent.file->instructions, opponent.file->length);
                memmove(ptr + OFFSETFOR(CHAMPION_TWO), champion.instructions, champion.instructions_size);
                ((struct vm_arena*)ptr)->champion_number = CHAMPION_TWO;
            }
            ++opponent_index;
        }
        ++champion_index;
    }
    ft_printf("ok: prepare arenas memory\n");
    ft_printf("encoding command buffer\n");
    computeEncoder = [commandBuffer computeCommandEncoderWithDispatchType:MTLDispatchTypeConcurrent];
    if (computeEncoder) {
        [computeEncoder setComputePipelineState:self->_pipelineStateRun];
        [computeEncoder setBuffer:self->_bounds.buffer offset:0 atIndex:0];
        [computeEncoder dispatchThreadgroups:MTLSizeMake(THREADS_W, THREADS_H, 1)
                       threadsPerThreadgroup:MTLSizeMake(GROUPS_W, GROUPS_H, 1)];
        [computeEncoder endEncoding];
    }
    else
        ft_dprintf(STDERR_FILENO, "cannot create computeEncoder %s %s\n", __FILE__, __FUNCTION__);
    return (commandBuffer);
}

@end
