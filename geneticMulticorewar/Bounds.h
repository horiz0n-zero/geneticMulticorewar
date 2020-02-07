//
//  Bounds.h
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class Multicorewar;

@interface Bounds : NSObject

- (instancetype)init:(Multicorewar *const)multicorewar;

@property(nonatomic, assign) NSUInteger index;

@property(nonatomic, strong) id <MTLBuffer> buffer;


@end

NS_ASSUME_NONNULL_END
