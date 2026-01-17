#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 5
#define MAX_RESOURCES 5
#define MAX_NAME_LEN 20

typedef enum {
    READ = 1,
    WRITE = 2,
    EXECUTE = 4
} Permission;

typedef struct {
    char name[MAX_NAME_LEN];
} User;

typedef struct {
    char name[MAX_NAME_LEN];
} Resource;

// ACL Structures
typedef struct {
    char username[MAX_NAME_LEN];
    int permissions;
} ACLEntry;

typedef struct {
    Resource resource;
    ACLEntry aclEntries[MAX_USERS];
    int aclCount;
} ACLControlledResource;

// Capability Structures
typedef struct {
    char resourceName[MAX_NAME_LEN];
    int permissions;
} Capability;

typedef struct {
    User user;
    Capability capabilities[MAX_RESOURCES];
    int capCount;
} CapabilityUser;

// Utility Function
void printPermissions(int perm) {
    if (perm & READ) printf("Read ");
    if (perm & WRITE) printf("Write ");
    if (perm & EXECUTE) printf("Execute ");
}

int hasPermission(int userPerm, int requiredPerm) {
    return (userPerm & requiredPerm) == requiredPerm;
}

// ACL Access Check
void checkACLAccess(ACLControlledResource *res, const char *userName, int perm) {
    for (int i = 0; i < res->aclCount; i++) {
        if (strcmp(res->aclEntries[i].username, userName) == 0) {
            if (hasPermission(res->aclEntries[i].permissions, perm)) {
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf("on %s: Access GRANTED\n", res->resource.name);
            } else {
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf("on %s: Access DENIED\n", res->resource.name);
            }
            return;
        }
    }
    printf("ACL Check: User %s has NO entry for resource %s: Access DENIED\n", userName, res->resource.name);
}

// CBAC Access Check
void checkCapabilityAccess(CapabilityUser *user, const char *resourceName, int perm) {
    for (int i = 0; i < user->capCount; i++) {
        if (strcmp(user->capabilities[i].resourceName, resourceName) == 0) {
            if (hasPermission(user->capabilities[i].permissions, perm)) {
                printf("Capability Check: User %s requests ", user->user.name);
                printPermissions(perm);
                printf("on %s: Access GRANTED\n", resourceName);
            } else {
                printf("Capability Check: User %s requests ", user->user.name);
                printPermissions(perm);
                printf("on %s: Access DENIED\n", resourceName);
            }
            return;
        }
    }
    printf("Capability Check: User %s has NO capability for resource %s: Access DENIED\n", user->user.name, resourceName);
}

int main() {
    // Setup users and resources
    User users[MAX_USERS] = {{"Alice"}, {"Bob"}, {"Charlie"}, {"Shroud"}, {"XQC"}};
    Resource resources[MAX_RESOURCES] = {{"File1"}, {"File2"}, {"File3"}, {"File4"}, {"File5"}};

    // Setup ACLs
    ACLControlledResource aclResources[MAX_RESOURCES] = {
        {resources[0], {{"Alice", READ | WRITE | EXECUTE}, {"Bob", READ}}, 2},
        {resources[1], {{"Charlie", EXECUTE}}, 1},
        {resources[2], {{"Alice", READ | EXECUTE}}, 1},
        {resources[3], {{"Shroud", WRITE}}, 1},
        {resources[4], {{"XQC", READ | WRITE | EXECUTE}}, 1}
    };

    // Setup Capabilities
    CapabilityUser capUsers[MAX_USERS] = {
        {users[0], {{"File1", READ | WRITE | EXECUTE}, {"File3", EXECUTE}}, 2},
        {users[1], {{"File1", READ}}, 1},
        {users[2], {{"File2", EXECUTE}}, 1},
        {users[3], {{"File4", WRITE}}, 1},
        {users[4], {{"File5", READ | WRITE | EXECUTE}}, 1}
    };

    // Test ACL
    checkACLAccess(&aclResources[0], "Alice", READ);
    checkACLAccess(&aclResources[0], "Alice", WRITE);
    checkACLAccess(&aclResources[0], "Alice", EXECUTE);
    checkACLAccess(&aclResources[3], "Alice", EXECUTE);
    checkACLAccess(&aclResources[0], "Bob", WRITE);
    checkACLAccess(&aclResources[2], "Bob", EXECUTE);
    checkACLAccess(&aclResources[0], "Charlie", READ);
    checkACLAccess(&aclResources[3], "Shroud", WRITE);
    checkACLAccess(&aclResources[4], "XQC", EXECUTE);
    
    
    printf("===================================================\n");
    
    // Test Capability
    checkCapabilityAccess(&capUsers[0], "File1", READ);
    checkCapabilityAccess(&capUsers[0], "File1", WRITE);
    checkCapabilityAccess(&capUsers[0], "File1", EXECUTE);
    checkCapabilityAccess(&capUsers[0], "File3", WRITE);
    checkCapabilityAccess(&capUsers[1], "File1", WRITE);
    checkCapabilityAccess(&capUsers[1], "File5", READ);
    checkCapabilityAccess(&capUsers[2], "File2", EXECUTE);
    checkCapabilityAccess(&capUsers[3], "File4", WRITE);
    checkCapabilityAccess(&capUsers[4], "File5", EXECUTE);

    return 0;
}

