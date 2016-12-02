//
//  madsqliteTests.m
//  madsqliteTests
//
//  Created by William Kamp on 11/27/16.
//  Copyright © 2016 William Kamp. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "MadSqliteFactory.hh"
#import "MadDatabase.h"
#import "MadContentValues.h"
#import "MadQuery.h"

@interface MadSqliteRTreeTests : XCTestCase

@end

@implementation MadSqliteRTreeTests

- (void)testRTreeBetween {
    double PRECISION_MAX_DELTA = 0.00002;

    id <MadDatabase> md = [self givenAnRTreeDatabase];
    [self givenValuesInserted:md];
    [self givenContentValuesInserted:md];

    id <MadQuery> query = [md query:@"SELECT * FROM demo_index WHERE id=2;"];
    XCTAssertTrue([query moveToFirst]);
    XCTAssertEqual(2, [query getInt:0].integerValue);
    XCTAssertEqualWithAccuracy(-81.0, [query getReal:1].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(-79.6, [query getReal:2].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(35.0, [query getReal:3].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(36.2, [query getReal:4].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertTrue([query moveToNext]);
    XCTAssertTrue([query isAfterLast]);
    XCTAssertFalse([query moveToNext]);

    query = [md query:@"SELECT * FROM demo_index WHERE id=1;"];
    XCTAssertTrue([query moveToFirst]);
    XCTAssertEqual(1, [query getInt:0].integerValue);
    XCTAssertEqualWithAccuracy(-80.7749582, [query getReal:1].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(-80.7747392, [query getReal:2].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(35.3776136, [query getReal:3].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertEqualWithAccuracy(35.3778356, [query getReal:4].doubleValue, PRECISION_MAX_DELTA);
    XCTAssertTrue([query moveToNext]);
    XCTAssertTrue([query isAfterLast]);
    XCTAssertFalse([query moveToNext]);
}

- (id <MadDatabase>)givenAnRTreeDatabase {
    id <MadDatabase> md = [MadSqliteFactory inMemoryDatabase];
    NSString *sql = @"CREATE VIRTUAL TABLE demo_index USING rtree(id INTEGER, "
            "minX REAL, "
            "maxX REAL, "
            "minY REAL, "
            "maxY REAL);";
    [md exec:sql];
    return md;
}

- (void)givenValuesInserted:(id <MadDatabase>)md {
    [md exec:@"INSERT INTO demo_index VALUES(1,"
            "-80.7749582,"
            "-80.7747392,"
            "35.3776136,"
            "35.3778356);"];
    XCTAssertNil([md getError]);
}

- (void)givenContentValuesInserted:(id <MadDatabase>)md {
    id <MadContentValues> cv = [MadSqliteFactory contentValues];
    [cv putInteger:@"id" withValue:@(2)];
    // NC 12th Congressional District in 2010
    [cv putReal:@"minX" withValue:@(-81.0)];
    [cv putReal:@"maxX" withValue:@(-79.6)];
    [cv putReal:@"minY" withValue:@(35.0)];
    [cv putReal:@"maxY" withValue:@(36.2)];
    XCTAssertTrue([md insert:@"demo_index" withValues:cv]);
}

@end
