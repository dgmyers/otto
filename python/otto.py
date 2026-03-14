import random
import time

POPSIZE = 100
SPEED = 2
TURN = 10
DEBUG = 0
GENERATIONS = 10
MUTATIONS = 5
INPUT = "../samples/sample_input_small.txt"
# OUTPUT = "my_output_large.txt"

def debug(*args):
    if DEBUG:
        print(list(args))

def calcDistance(a,b):
    distance = ((a[0]-b[0])**2 + (a[1]-b[1])**2) ** .5
    debug("distance: ", a, b, distance)
    return distance

def fitness(track, dude):
    i = 0
    fitness = 0
    location = [0,0]
    debug("dude: ", dude)
    for point in track:
        if ((dude >> i) % 2) == 1:
            debug("visiting")
            distance = calcDistance(location, track[i])
            cost = distance / SPEED + TURN
            debug("cost: ", cost)
            location = [track[i][0], track[i][1]]
            fitness += cost
        else:
            debug("skipping")
            fitness += track[i][2]
        i += 1
    cost = calcDistance(location, [100,100]) / SPEED + TURN
    fitness += cost
    # go to 100 100
    return fitness

def seed(size, seed = POPSIZE):
    dudes = []
    for i in range(seed):
        dude = random.getrandbits(size)
        # print dude
        dudes.append(dude)
    debug("dudes: ", dudes)
    return dudes

def mite(leng, dude):
 
    mask = 1 << random.randint(0, leng)
    child = dude ^ mask
    
    # lazy
    i = 0
    while(i < MUTATIONS):
        mask = 1 << random.randint(0, leng)
        child = child ^ mask
        i += 1
    debug("dude splits to: ", dude, child)
    return child

def go(track):
    
    # build first generation 
    pop = seed(len(track))
    
    gen = 0
    while( gen < GENERATIONS ):
        # sort by fitness
        pop = sorted(pop, key = lambda dude: fitness(track, dude) ) 
        debug(pop)
        
        #next generation
        generation = []
        i = 0
        while(i<POPSIZE/2):
            generation.append(pop[i])
            generation.append(mite(len(track), pop[i]))
            i += 1
        debug("k", generation)
        pop = generation
        gen += 1
    print ("best: ", pop[0], "fitness: ", fitness(track, pop[0]))

def opens(fn = INPUT):

    debug("")
    fp = open(fn)

    tracks = []
    while(1):
        leng = int(fp.readline())
        debug("length: ", leng)
        last = []
        for i in range(leng):
            rawline = fp.readline()
            line = []
            for j in rawline.split():
                line.append(int(j))
            last.append(line)
            debug(line)
        
        if leng > 0:
            tracks.append(last)
        else:
            break
    debug(tracks)
    return tracks
    
if __name__ == "__main__":

    # dude = 5
    # for i in range (3):
    #     # tf = dude/(2 ** i)
    #     wat = ((dude >> i) % 2)
    #     print("wat", wat, wat == 1) 
    #     # if ((dude/(2 ** i)) % 2) == 1:
    #     #     print("yes", i, dude, tf)
    #     # else:
    #     #     print("no", i, dude, tf)
    # # debug("sanity", (dude/(2**i) % 2))
    
    # pt1 = time.process_time()
    t1 = time.time()
    print("hello world") 

    tracks = opens()
    for track in tracks:
        go(track)

    # pt2 = time.process_time()
    t2 = time.time()
    # print("CPU Runtime: ", (pt2 - pt1)*1000)
    print("Wall Runtime: ", (t2 - t1)*1000)
    # print("goodbye")
