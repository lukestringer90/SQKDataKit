//
//  NSManagedObject+SQKAdditions.h
//  SQKDataKit
//
//  Created by Luke Stringer on 05/12/2013.
//  Copyright (c) 2013 3Squared. All rights reserved.
//

@import CoreData;

/**
 *  Additions to NSManagedObject to reduce boilerplate and simplify common operations.
 *  @warning These methods never should __never__ be called directly on NSManagedObject (e.g.
 * [NSManagedObject entityName]), but instead only on subclasses.
 */
@interface NSManagedObject (SQKAdditions)

/**
 *  @name Entity descriptions.
 */

/**
*  The name of the entity, derived from the class name.
*
*  @return The name of the entity.
*/
+ (NSString *)sqk_entityName;

/**
 *  A convenience method for obtaining a new NSEntityDescription.
 *
 *  @param context The managed object context to use. Must not be nil.
 *
 *  @return The entity for the calling class from the managed object model associated with context’s
 *  persistent store coordinator.
 */
+ (NSEntityDescription *)sqk_entityDescriptionInContext:(NSManagedObjectContext *)context;

/**
 *  @name Insertion.
 */

/**
 *  Insert and return a new instance of NSManagedObject subclass.
 *
 *  @param context The managed object context to use. Must not be nil.
 *
 *  @return A new, autoreleased, fully configured instance of the class. The instance has its entity
 *  description set and is inserted it into context.
 */
+ (instancetype)sqk_insertInContext:(NSManagedObjectContext *)context;

/**
 *  Find an instance of NSManagedObject subclass in the NSManagedObjectContext matching the key and
 *  value. If no match is found, a new object is inserted with the it's key value set appropriately.
 *
 *  @param key     The name of the object property to match on.
 *  @param value   The value of the property specified by key.
 *  @param context The managed object context to use. Must not be nil.
 *  @param error   A pointer to an NSError object. You do not need to create an NSError object. The
 *  save operation aborts after the first failure if you pass NULL.
 *
 *  @return A managed object retrieved from the context, or a new object inserted to the context
 *  with key set to value.
 */
+ (instancetype)sqk_insertOrFetchWithKey:(NSString *)key
                                   value:(id)value
                                 context:(NSManagedObjectContext *)context
                                   error:(NSError **)error;

/**
 *  @name Fetching
 */

/**
 *  Returns a fetch request configured for the entity.
 *
 *  @discussion This method provides a convenient way to create a fetch request without having to
 *  retrieve an NSEntityDescription object.
 *  @return A fetch request configured to fetch using the subclass' entity.
 */
+ (NSFetchRequest *)sqk_fetchRequest;

/**
 *  @name Insert or update.
 */

/**
 *  A block called during the batch insert-or-update method to facilitate setting properties of a
 *  managed object. You should not initiate any other fetch requests here, you should only apply the
 *  logic necessary to set the properties of the managed object.
 *
 *  @param dictionary    The dictionary of data to be used in setting the managed object properties.
 *  @param managedObject The inserted or fetched managed object.
 */
typedef void (^SQKPropertySetterBlock)(NSDictionary *dictionary, id managedObject);

/**
 *  Perform a batch insert-or-update.
 *  This method codifies the pattern found in Apple's Core Data Programming Guide under "Implementing Find-or-Create Efficiently".
 *
 *  In order to be fast this method only executes only one fetch request. Therefore you must be careful what you do inside the propertySetterBlock.
 *  For example, if you insert an object in the propertySetterBlock with the same ID as one in your remote data, this method will not know that 
 *  it already exists and will insert it again, leading to duplicates.
 *
 *  You should call this method from inside performBlockAndWait.
 *
 *  @param remoteData          Array of kvc-compliant objects you wish to use for insert/update. This is most likely data from a remote source, i.e. a web service.
 *  @param modelKey            The key path of the primary key property of the managed object being inserted/updated. Must be a kvc-compliant property of the model.
 *  @param remoteDataKey       The key path of the objects in `remoteData` to map to the primary key for the managed object. Must be a kvc-compliant property of the objects in remoteData.
 *  @param propertySetterBlock A block called to facilitate setting properties of the managed object. You should avoid initiating any Core Data operations here - you should only apply the logic necessary to set the properties of the managed object.
 *  @param privateContext      A managed object context with the concurrency type NSPrivateQueueConcurrencyType. Use the `newPrivateContext` method of `SQKContextManager` to obtain one.
 *  @param error               If there is a problem executing the fetch, upon return contains an instance of NSError that describes the problem.
 */
+ (void)sqk_insertOrUpdate:(NSArray *)remoteData
            uniqueModelKey:(id)modelKey
           uniqueRemoteKey:(id)remoteDataKey
       propertySetterBlock:(SQKPropertySetterBlock)propertySetterBlock
            privateContext:(NSManagedObjectContext *)context
                     error:(NSError **)error;

/**
 *  @name Deletion.
 */

/**
 *  Delete the NSManagedObject from its current context.
 */
- (void)sqk_deleteObject;

/**
 *  Remove all objects of the class from the store asynchronously.
 *
 *  @param context The managed object context to use. Must not be nil.
 *  @param error   If there is a problem executing the fetch, upon return contains an instance of
 *  NSError that describes the problem.
 */
+ (void)sqk_deleteAllObjectsInContext:(NSManagedObjectContext *)context error:(NSError **)error;

/**
 *  Remove all objects of the class matching the given predicate from the store asynchronously.
 *
 *  @param context   The managed object context to use. Must not be nil.
 *  @param predicate The predicate to be used to filter the objects to be deleted.
 *  @param error     If there is a problem executing the fetch, upon return contains an instance of NSError that describes the problem.
 */
+ (void)sqk_deleteAllObjectsInContext:(NSManagedObjectContext *)context withPredicate:(NSPredicate *)predicate error:(NSError **)error;

/**
 *  @name Property description.
 */

/**
 *  Convenience method for retrieving an NSPropertyDescription.
 *
 *  @param name    The name of the property.
 *  @param context The managed object context to use. Must not be nil.
 *
 *  @return A property description configured for property name.
 */
+ (NSPropertyDescription *)sqk_propertyDescriptionForName:(NSString *)name
                                                  context:(NSManagedObjectContext *)context;

@end
