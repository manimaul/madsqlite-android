//
// Created by William Kamp on 11/16/16.
// Copyright (c) 2016 William Kamp. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MadQuery.h"
#import "MadDatabase.h"

@protocol MadContentValues;

@interface MadDatabaseImpl : NSObject<MadDatabase>

- (instancetype)init;

- (instancetype)initWithPath:(NSString *)path;

@end