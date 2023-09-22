#include <stdio.h>
#include <stdbool.h>
//RaghadHamdeh 1191093
//method to examine if the system is safe
bool is_system_safe(int p, int r, int maximum[p][r], int allocation[p][r], int needs[p][r], int available[r]);
//method to examine wethier Grant the requested resources to the process
void grant_request_resource(int processID, int r,int p, int maximum[p][r], int request[r], int allocation[processID][r], int needs[processID][r], int available[r]);
//method for print all matrix's
void printData ( int p, int r,int maximum[p][r], int allocation[p][r], int needs[p][r], int available[r]) ;



int main()
{
    printf("**************Banker's Algorithm Simulation****************\n");
    char filename[100];
//ask user to enter the name of the file
    printf("Enter the name of the file: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");// Read the file by opening it.
    //if File not found...
    if (fp == NULL)
    {
        printf("Runtime error: invalid filename %s\n", filename);
        return 1;
    }
    //Read File Content
    const int p ; //number of processes running
    const int r ; // number of resource types
    fscanf(fp, "%d%d\n", &p, &r);
    /*Available – no. of available resources of each type with the system.
     Max – maximum need of any process for any resource
     Allocation – number of resources allocated to each process
     Need – is calculated based on the formula (Max – Allocation)

    */
    int maximum[p][r], allocation[p][r], needs[p][r], available[r];
//Read the maximum resource allocation for each process from the file
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            fscanf(fp, "%d", &maximum[i][j]);
        }
    }
//Read the allocation for each process from the file

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            fscanf(fp, "%d", &allocation[i][j]);
        }
    }
//Calculate need  for each process from the file which equal  (maximum- allocation).

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            needs[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
//Read the available from the file

    for (int i = 0; i < r; i++)
    {
        fscanf(fp, "%d", &available[i]);
    }

    fclose(fp);//close the file

    //print all matrix
    printData(p,r,maximum, allocation, needs, available);


    // Check if the system is safe
    bool isSystemSafe = is_system_safe(p, r, maximum, allocation, needs, available);
    if (isSystemSafe)
    {

        int processID;
        int request[r];

        printf("\nEnter the process ID and the resource requests: ");
        scanf("%d", &processID);
        for (int i = 0; i < r; i++)
        {
            scanf("%d", &request[i]);
        }
        // Print the resource ID and request
        printf("\nProcess ID: %d\n", processID);
        for (int i = 0; i < r; i++)
        {
            printf("Resource ID: %d, Request: %d\n", i, request[i]);
        }
        // Grant the requested resources if safe
        grant_request_resource(processID, r,p,maximum, request, allocation, needs, available);


    }
    else
    {
        printf("The system is not safe.\n");
    }
}
//method is system safe
bool is_system_safe(int p, int r, int maximum[p][r], int allocation[p][r], int needs[p][r], int available[r])
{
    bool finish[p];
    int work[r];
    int safeSequence[p];

    // Initialize Work vector
    for (int i = 0; i < r; i++)
    {
        work[i] = available[i];//Work = Available

    }

    // Initialize Finish vector
    for (int i = 0; i < p; i++)
    {
        finish[i] = false;
    }

    int count = 0;
    while (count < p && count <= p * r)
    {
        bool found = false;
        for (int i = 0; i < p; i++)
        {
            if (!finish[i])
            {
                bool isExecutable = true;
                for (int j = 0; j < r; j++)
                {
                    if (needs[i][j] > work[j])
                    {
                        isExecutable = false;
                        break;
                    }
                }
                if (isExecutable)
                {
                    for (int j = 0; j < r; j++)
                    {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSequence[count] = i;

                    count++;
                    found = true;
                }
            }
        }
        if (!found)
        {
            break;
        }
    }
    if (count == p)
    {
        printf("\nThe system is safe. The safe sequence of processes:  ");
        for (int i = 0; i < p; i++)
        {
            printf("P%d", safeSequence[i]);
            if (i != p - 1)
            {
                printf(" -> ");
            }
        }
        printf("\n");

        return true;
    }
    else
    {
        return false;
    }
}

void grant_request_resource(int processID, int r, int p, int maximum[p][r], int request[r], int allocation[][r], int needs[][r], int available[r])
{
    // Check if the request can be granted
// Step 1: Check if the request is valid
    for (int i = 0; i < r; i++)
    {
        if (request[i] > needs[processID][i])
        {
            printf("Error, The process has exceeded its maximum claim for resource type R%d.\n", i);
            return false;

        }
    }

// Step 2: Determine whether the request can be granted immediately.
    bool canGrant = true;
    for (int i = 0; i < r; i++)
    {
        if (request[i] > available[i])
        {
            canGrant = false;
            printf("Error,The process must wait until enough resources are available.\n");
            return false;
        }
    }


    // Step 3:Allocate resources temporarily
    for (int i = 0; i < r; i++)
    {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        needs[processID][i] -= request[i];
    }

    // Check to see if the end result is a safe state for resources.
    bool isSystemSafe = is_system_safe(p, r, maximum, allocation, needs, available);

    if (!isSystemSafe)
    {
        // Rollback the allocation if the resulting state is unsafe
        for (int i = 0; i < r; i++)
        {
            available[i] += request[i];
            allocation[processID][i] -= request[i];
            needs[processID][i] += request[i];
        }
        printf("The system is not safe. The allocation has been restored to its previous state.\n");
        return false;
    }
    printf("\n");
    printf("The request has been granted. The resources have been allocated to process P%d and the matrices have been updated.\n", processID);
    printf("\n");
    // Print the matrices and the array
    printf("The following  matrices updated after granting the request :\n");
    printf("\n");

    printData(p,r,maximum, allocation, needs, available);

}

void printData( int p, int r,int maximum[p][r], int allocation[p][r], int needs[p][r], int available[r])
{


    printf("The matrix MAXIMUM:\n");
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("The matrix ALLOCATION :\n");
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf(" The matrix NEEDS :\n");
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            printf("%d ", needs[i][j]);
        }
        printf("\n");
    }

    printf("The array AVAILABLE :\n");
    for (int i = 0; i < r; i++)
    {
        printf("%d ", available[i]);
    }
    printf("\n");
}


