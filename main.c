#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct garage {
    int x;
    int y;
    char name[21]; // accounting for the null operator
} garage;

float distance(garage g1, garage g2);
void printbestpairs(garage *garages, int *bestperm, float *distances, int n);
void perms(int *perm, int *used, int k, int n, float *bestdistance, garage *garages, int *bestperm, float *bestdistances, float currentdistance);
float distanceofpairs(garage *garages, int *perm, int n);

float distance(garage g1, garage g2) { // helper function that just does simple distance calculation
    return sqrt(pow(g1.x - g2.x, 2) + pow(g1.y - g2.y, 2));
}

float distanceofpairs(garage *garages, int *perm, int n) {
    float sum = 0;
    for (int i = 0; i < n; i += 2) { // increment by 2 since we are pairing
        sum += distance(garages[perm[i]], garages[perm[i + 1]]);
    }
    return sum;
}

void printbestpairs(garage *garages, int *bestperm, float *distances, int n) {
    for (int i = 0; i < n; i += 2) {
        printf("(%s, %s, %.3f)\n", garages[bestperm[i]].name, garages[bestperm[i + 1]].name, distances[i / 2]); // distances[i/2] because we are incrementing by 2
    }
}

void perms(int *perm, int *used, int k, int n, float *bestdistance, garage *garages, int *bestperm, float *bestdistances, float currentdistance) {
    if (k == n) { // base case
        if (currentdistance < *bestdistance) {
            *bestdistance = currentdistance; // changing best distance if the curr distance is less than the best distance

            // copying best permutation to the best permutation array
            for (int i = 0; i < n; i++) {
                bestperm[i] = perm[i];
            }

            // store distances in bestdistances
            for (int i = 0; i < n; i += 2) {
                // i/2 is used to store pairs in their respective spot in the best distances array
                bestdistances[i / 2] = distance(garages[bestperm[i]], garages[bestperm[i + 1]]);
            }
        }
        return;
    }

    // finding the first garage and if the garage is not being used then set it to first and break
    int first = -1;
    for(int i = 0; i < n; i++){
        if(!used[i]){
            first = i;
            break;
        }
    }

    // pair the first unpaired garage with every other unpaired garage
    // mark the first garage as used
    used[first] = 1;
    for (int j = first + 1; j < n; j++) { // recursive case
        if (!used[j]) { // if j is also not being used
            // create a pair: (first, j)
            perm[k] = first;
            perm[k+1] = j;
            used[j] = 1; // mark j as used

            // calculate the new distance incrementally
            float newdistance = currentdistance + distance(garages[first], garages[j]);

            // Prune: only recurse if newdistance is better than the best known distance
            if(newdistance < *bestdistance){
                perms(perm, used, k + 2, n, bestdistance, garages, bestperm, bestdistances, newdistance);
            }

            used[j] = 0; // after the recursive call, set the j element back to unused for the next process
        }
    }
    used[first] = 0; // backtrack: unmark first as read
}

int main(void) {
    garage mygarage[16]; 
    int perm[16] = {0}; // the curr permutation
    int used[16] = {0}; // what index is being used.
    int bestperm[16]; // To store the best permutation
    float bestdistances[8]; // Array to hold distances of pairs
    int expressway;

    scanf("%d", &expressway);
    int numgarages = 2 * expressway;

    for (int i = 0; i < numgarages; i++) {
        scanf("%d %d %s", &mygarage[i].x, &mygarage[i].y, mygarage[i].name); // taking input for the garage position and name
    }
    
    float bestdistance = pow(10, 8); // big number
    perms(perm, used, 0, numgarages, &bestdistance, mygarage, bestperm, bestdistances, 0.0f);

    // output the result
    printf("%.3f\n", bestdistance);
    printbestpairs(mygarage, bestperm, bestdistances, numgarages); // corrected parameters

    return 0;
}
