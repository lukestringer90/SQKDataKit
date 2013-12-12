//
//  NaiveImportOperation.m
//  SQKDataKit
//
//  Created by Luke Stringer on 12/12/2013.
//  Copyright (c) 2013 3Squared. All rights reserved.
//

#import "NaiveImportOperation.h"
#import "Commit.h"
#import "NSManagedObject+SQKAdditions.h"

@implementation NaiveImportOperation

- (void)updatePrivateContext:(NSManagedObjectContext *)context usingJSON:(id)json {
    NSDate *beforeDate = [NSDate date];
    
    [json enumerateObjectsUsingBlock:^(NSDictionary *dictionary, NSUInteger idx, BOOL *stop) {
        NSFetchRequest *fetchRequest = [Commit SQK_fetchRequest];
        fetchRequest.predicate = [NSPredicate predicateWithFormat:@"sha == %@", dictionary[@"sha"]];
        fetchRequest.fetchLimit = 1;
        NSArray *objects = [context executeFetchRequest:fetchRequest error:nil];
        Commit *commit = [objects lastObject];
        if (!commit) {
            commit = [Commit SQK_insertInContext:context];
            commit.sha = dictionary[@"sha"];
            commit.authorName = dictionary[@"commit"][@"committer"][@"name"];
            commit.authorEmail = dictionary[@"commit"][@"committer"][@"email"];
            commit.date = [self dateFromJSONString:dictionary[@"commit"][@"committer"][@"date"]];
            commit.message = dictionary[@"commit"][@"message"];
            commit.url = dictionary[@"html_url"];
        }
    }];
    
    NSLog(@"Naive took: %f", [[NSDate date] timeIntervalSinceDate:beforeDate]);
    
}

- (NSDate *)dateFromJSONString:(NSString *)jsonString {
    NSDate *date = [[self dateFormatter] dateFromString:jsonString];
    return date;
}

- (NSDateFormatter *)dateFormatter {
    static NSDateFormatter *dateFormatter = nil;
    if (!dateFormatter) {
        dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss'Z'"];
    }
    return dateFormatter;
}

@end
