#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

char FILE_IN[] = "../samples/sample_input_small.txt";
// char FILE_OUT[] = "my_output_small.txt";

int POPSIZE = 100;
int SPEED = 2;
int TURN = 10;
int GENERATIONS = 100;
int MUTATIONS = 5;

// double best;
//todo
struct Track *c_track;
	
struct Track {
  int size;
  int **points; //size * 3 * int
};

struct Tracks {
  int size;
  struct Track **tracks; //size * int
};
	
	
double calcDistance(int a[], int b[]){
    double distance = pow( (pow((a[0]-b[0]),2)) + (pow((a[1]-b[1]),2)), .5);
    //printf("distance: %f\n", distance);
    return distance;
}

void describeTracks(struct Tracks* tracks){
	// printf("  describe1 address %ld size %d\n", (long)tracks, tracks->size);
	for(int i = 0; i<tracks->size ;i++){
		struct Track *track = (tracks->tracks[i]);
		// printf("  describe3 iter %d size %d address %ld\n", i, track->size, (long)track);
		// todo
	}
}

double fitness(struct Track *track, int dude){

    double fitness = 0;
    int location[] = {0,0};
    // printf("dude: %d size: %d\n", dude, track->size);
    for (int i = 0; i < track->size; i++){
	    double cost;
		if ((dude >> i) % 2){
            // printf("visiting %d [%d,%d]\t", i, track->points[i][0], track->points[i][1]);
            double distance = calcDistance(location, track->points[i]);
            cost = (distance / SPEED) + TURN;
            location[0] = track->points[i][0];
            location[1] = track->points[i][1];

        } else {
            // printf("skipping %d [%d,%d]\t", i, track->points[i][0], track->points[i][1]);
            cost = track->points[i][2];
		}
		// printf("cost: %f\n", cost);
		fitness += cost;
	}
	// goto(lul) 100 100
	int ending[] = {100,100};
	double cost = (calcDistance(location, ending) / SPEED) + TURN;
    fitness += cost;

	//todo
	// if(fitness < best){
		// best = fitness;
	// }

    return fitness;
}
int comparitor_r(void *arg ,const void *a, const void *b){
	return 0;
}
int comparitor(const void *a, const void *b){

	// printf("1comparing %ld %ld\n", a, b);	
	// int *x = a;
	// int *y = b;
	//wtf
	int x = *(int *)a;
    int y = *(int *)b;
	// printf("2comparing %d %d\n", x, y);
	// return 0;
	double fitA = fitness(c_track, x);
	double fitB = fitness(c_track, y);
	int fee = fitA >= fitB;
	// if(fee == 0){
		// printf("\n\n");
		// fee = -1;
	// }
	// printf("comparing %d to %d on track: ?\n", x, y);
	return fee;
}

struct Tracks* opens(){

	FILE *fp;
    struct Tracks *tracks = malloc ( sizeof(struct Tracks));
	char tString[100];
	char *token;
	const char *delimiter = " ";
	int tracksize = 0;
	
	printf("opening: %s\n", FILE_IN);
    fp = fopen(FILE_IN, "r");
	
	tracks->tracks = malloc( sizeof(long));
	
	for(;; tracksize++){
		
		fgets(tString, 100, fp);

		//printf("op1 %s", tString);
		
        int leng = atoi(tString);
		//printf("op2\n");
		struct Track *track = NULL;
		if (leng == 0){
			break;
		} else {
			//printf("op3\n");
			tracks->tracks = realloc(tracks->tracks, sizeof(long) * (tracksize+1));
			//printf("op4\n");

			//printf("huh %ld\n", (long)tracks->tracks[0]);
			track = malloc( sizeof(struct Track));
			//printf("huh %ld\n", (long)tracks->tracks[0]);
			
			track->points = malloc(leng * sizeof(long));
			//printf("op5 %d\n", leng);
			track->size = leng;
			
			//printf("op6 %d\n", track->size);
			//catch deez
		}
        for(int j = 0; j < leng; j++){
			
			fgets(tString, 100, fp);
            // deeeeeeeeeez
			track->points[j] = malloc(3 * sizeof(int));
			// Get the tokein
			token = strtok(tString, delimiter);
			track->points[j][0] = atoi(token);
			
			token = strtok(NULL, delimiter);
			track->points[j][1] = atoi(token);
			
			token = strtok(NULL, delimiter);
			track->points[j][2] = atoi(token);
			
			// printf("line");
			// printf("line %d %d %d\n", track->points[j][0], track->points[j][1], track->points[j][2]);
		}
		
		// printf("insize %d iter %d\n", track->size, tracksize);

		tracks->tracks[tracksize] = track;
		tracks->size = tracksize+1;
		describeTracks(tracks);
		
		// printf("huh %d\n", (long)track);
		// printf("huh %ld %d\n", (long)tracks->tracks[tracksize], tracks->tracks[tracksize]->size);
		// printf("huh %ld %d\n", (long)tracks->tracks[0], tracks->tracks[0]->size);

	}
	// printf("isbroke\n");
	tracks->size = tracksize;
	// printf("fixit\n");

	// printf("size %d from %d and %d\n", tracks->size, tracksize, tracks->tracks[0]->size);
	// free(track);
	fclose(fp);
	// printf("fixd\n");

    return tracks;
}

int* seed(size){
	// printf("seed size %d\n", POPSIZE);
    int *dudes = malloc(POPSIZE * sizeof(int));
	// printf("size %d from %d\n", sizeof(dudes), POPSIZE);
	for (int i = 0; i<POPSIZE; i++){
        //dude = random.getrandbits(size)
		dudes[i] = rand() % (int)pow(2, size);
	}
    return dudes; // dudes
}

int mitosis(int leng, int dude){
 
	int child = 0;
	if(MUTATIONS <= 1){
		int mask = 1;
		int offset = rand() % (leng);
		mask = mask << offset;
		//printf("%d   ", mask);
		child = dude ^ mask;
	} else {
		child = dude;
		for(int i = 0; i < MUTATIONS; i++){
			int mask = 1;
			int offset = rand() % (leng);
			mask = mask << offset;
			//printf("%d   ", mask);
			int child = child ^ mask;
		}
	}
    //printf("%d\n", child);
	// if lazy
	// for(int = 0; i < 9; i++){
        // mask = 1 << random.randint(0, leng)
        // child = child ^ mask
    // }
    // printf("dude %d splits to %d ", dude, child);
    return child;
}

void go(struct Track *track){

	// build first generation 
	//printf("> going2 %d\n", track->size);
	int *pop = seed(track->size);
	// int next[POPSIZE];
	// int curr[POPSIZE];
	int temp[POPSIZE/2];
    // for (int i = 0; i<track->size; i++){
		// printf("> %d %d\n", pop[i], track->size);
	// }

    // int gen = 0;
	// fitness(track, pop[0]);
	c_track = track;
	
    for(int gen = 0; gen < GENERATIONS; gen++){
		// printf(".");
        // sort by fitness
		// printf("compare: %d %d %d %d %ld %ld\n", sizeof(pop), sizeof(pop) / sizeof(pop[0]), pop[0], pop[1] &(pop[0]), &(pop[1]));
        qsort(pop, POPSIZE, 4, comparitor);
        // qsort(pop, sizeof(pop), sizeof(pop) / sizeof(pop[0]), comparitor);
        // qsort_r(pop, POPSIZE * sizeof(int), sizeof(int), &comparitor, (void*)&track);
		//pop = sorted(pop, key = lambda dude: fitness(track, dude) ) 
        // debug(pop)
        
		//todo
		int i;
		// for(i = 0; i<POPSIZE; i++){
            // printf("member: %d fitness: %lf\n", pop[i], fitness(track, pop[i]));
		// }
		for(i = 0; i<POPSIZE /2; i++){
			temp[i] = pop[i];
		}
        // #next generation
        for(i = 0; i<POPSIZE /2; i++){
            pop[i*2] = temp[i];
            pop[(i*2)+1] = mitosis(track->size, temp[i]);
		}
        // debug("k", generation)
	}
	printf("  best: %d fitness: %lf\n", temp[0], fitness(track, temp[0]));
	// printf("\n  best: %d fitness: %lf\n", temp[0], fitness(track, temp[0]));
	return;
}

void freeTracks(struct Tracks *tracks){
	//printf("free0\n");
	for(int i; i<tracks->size ;i++){
		//printf("free %d\n", i);
		struct Track *track = (tracks->tracks[i]);
		free(track->points);
		free(track);
	}
	//printf("free1\n");
	free(tracks->tracks);
	free(tracks);
	//printf("free2\n");
}

int main(int argc, char *argv[]){
	
	// clock_t begining = clock();
	printf("hello\n");
	
	struct timespec ts1, tw1; // both C11 and POSIX
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1); // POSIX
    clock_gettime(CLOCK_MONOTONIC, &tw1); // POSIX; use timespec_get in C11
    // clock_t t1 = clock();
	
	srand(time(NULL));
	int size = 3;
	if(0){
		int *pop = malloc(POPSIZE * sizeof(int));
		for (int i = 0; i<POPSIZE; i++){
			//dude = random.getrandbits(size)
			pop[i] = rand() % (int)pow(2, size);
		}
		// printf("pcompar: %d %d %ld %ld %ld\n", pop[0], pop[1], &pop, &(pop[0]), &(pop[1]));

		qsort(pop, POPSIZE * sizeof(int), sizeof(int), comparitor);
	} else {
		struct Tracks *tracks = opens();

		for(int i = 0; i < tracks->size; i++){
		// for(int i = 1; i < 2; i++){
			// printf("> going: %d of %d size: %d\n", i, tracks->size, (tracks->tracks[i])->size);
			// best = 9999999;
			go(tracks->tracks[i]);	
		}

		
		freeTracks(tracks);
	}
    // double runtime = clock() - begining;
	// printf("Done in %lf\n", runtime);
	 
	struct timespec ts2, tw2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
    clock_gettime(CLOCK_MONOTONIC, &tw2);
    // clock_t t2 = clock();
 
    // double dur = 1000.0 * (t2 - t1) / CLOCKS_PER_SEC;
    double posix_dur = 1000.0 * ts2.tv_sec + 1e-6 * ts2.tv_nsec
                           - (1000.0 * ts1.tv_sec + 1e-6 * ts1.tv_nsec);
    double posix_wall = 1000.0 * tw2.tv_sec + 1e-6 * tw2.tv_nsec
                            - (1000.0 * tw1.tv_sec + 1e-6 * tw1.tv_nsec);
 
    // printf("CPU time used (per clock()): %.2f ms\n", dur);
    printf("CPU time used (per clock_gettime()): %.2f ms\n", posix_dur);
    printf("Wall time passed: %.2f ms\n", posix_wall);
	return 0;
}
