#![allow(unused)]
use std::fs::File;
use std::io::prelude::*;
use std::io::{self, BufRead};
use std::path::Path;
use std::convert::TryFrom;
use rand::{Rng, SeedableRng};

const FILE_IN:&str = "../samples/sample_input_small.txt";
// const FILE_OUT:&str = "my_output_small.txt";
const POPSIZE:usize = 10;
const SPEED:f32 = 2.0;
const TURN:f32 = 10.0;
const GENERATIONS:i32 = 5;
const MUTATIONS:usize = 1;

// #[derive(Debug)]
// struct Point {
//     x: i32,
//     y: i32,
//     z: i32
// }

fn opens() -> Vec<Vec<Vec<i32>>>{

    let mut tracks: Vec<Vec<Vec<i32>>> = Default::default();

    // Create a path to the desired file
    let path = Path::new(FILE_IN);
    let display = path.display();

    // Open the path in read-only mode, returns `io::Result<File>`
    let mut file = match File::open(&path) {
        Err(why) => panic!("couldn't open {}: {}", display, why),
        Ok(file) => file,
    };

    let mut reader = io::BufReader::new(file);
    // for line in reader.map_while(Result::ok) ?
    loop {
        let mut line: String = Default::default();
        let c = reader.read_line(&mut line);
        println!(">{}", line.trim());
        if line.trim() == "" {
            break;
        }

        // str to int conversion
        let mut parts = line.trim().split(" ");
        let t = parts.next().expect("Flail");
        // dbg!(t);       
        let track_size = t.parse::<i32>().unwrap();
        // dbg!(track_size);

        let mut track: Vec<Vec<i32>> = Default::default();
        for i in 0..track_size {
            // todo hmmmmmmm
            let mut line: String = Default::default();
            let c = reader.read_line(&mut line);
            println!(">{}", line.trim());

            // str to int conversion
            // let mut parts = line.trim().split(" ");
            // let collection = parts.collect::<Vec<&str>>();
            let collection = line.trim().split(" ").collect::<Vec<&str>>();
     
            // let x = collection[0].parse::<i32>().unwrap();
            // let y = collection[1].parse::<i32>().unwrap();
            // let z = collection[2].parse::<i32>().unwrap();
            let mut point: Vec<i32> = Default::default();
            point.push(collection[0].parse::<i32>().unwrap());
            point.push(collection[1].parse::<i32>().unwrap());
            point.push(collection[2].parse::<i32>().unwrap());

            // let point = Point {
            //     x: collection[0].parse::<i32>().unwrap(),
            //     y: collection[1].parse::<i32>().unwrap(),
            //     z: collection[2].parse::<i32>().unwrap()
            // };

            // dbg!(point);
            track.push(point);    
        }
        if (track.len() > 0){
            tracks.push(track);
        }
    }
    tracks
}

fn calc_distance(a: [i32; 2], b: [i32; 2]) -> f32{
    
    // let base: u64 = 3;
    // let exp: i32 = 17;
    // let number = base.pow(exp);
    // println!("{} raised to the {} power = {}", base, exp, number);
    // let x = (a[0]-b[0]);
    // let y = x.pow(2);
    // dbg!("xyz: ", x, y);

    let flt = ((a[0]-b[0]).pow(2) + (a[1]-b[1]).pow(2)) as f32;
    let distance: f32 = flt.sqrt();
    // println!("  distance float: {} sqrt: {}", flt, distance);
    distance
}

fn fit(mut track: Vec<Vec<i32>>, dude: usize) -> f32 {
  /*i = 0
    fitness = 0
    location = [0,0]
    debug("dude: ", dude)
    for point in track:
        if (dude/(i+1) % 2) == 1:
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
    return fitness */
    let two: usize = 2; // ffs

    let mut fitness = 0.0;
    let mut location = [0,0];
    // for i in 0..track.len() {
    let mut i = 0;
    for p in &track {
        // let p = track.wrapping_add(i);
        // println!("dude {} {}", i, dude/(two.pow(i.try_into().unwrap())) % 2);
        if (dude/(two.pow(i.try_into().unwrap())) % 2) == 1 {
            // dbg!("visit");
            let mut temp: [i32; 2] = [p[0].clone(), p[1].clone()];
            let distance = calc_distance(location, temp);
            // let distance = calc_distance(location, [&track[i][0], &track[i][1]]);
            let cost = (distance / SPEED) + TURN;
            location = [p[0].clone(), p[1].clone()];
            fitness += cost;
            // dbg!("visit", fitness);
        } else {
            fitness += p[2] as f32;
            // dbg!("skip", fitness);
        }
        i += 1;
    }
    // go to 100 100
    let cost = (calc_distance(location, [100,100]) / SPEED) + TURN;
    fitness += cost;
    // println!("dude {} {}\n", dude, fitness);

    fitness
}

fn mite(length: &u32, parent: &usize) -> usize{
    let mut child = parent.clone();
    
    // lazy
    for i in 0..MUTATIONS {
        let mask: usize = 1 << rand::thread_rng().gen_range(0 .. *length);
        child = child ^ mask;
    }
    // println!("dude {} splits to {}", parent, child);

    child
}

fn go(mut track :Vec<Vec<i32>>) {
    // dbg!(&track);
    let mut popu = Vec::<usize>::with_capacity(POPSIZE);
    let two: usize = 2;
    let length = track.len() as u32;
    let result = two.pow(length);
    for g in 0..GENERATIONS {
        for i in 0..POPSIZE { 
            // println!("{} to the power of {} is {}", two, cast, result);
            popu.push(rand::thread_rng().gen_range(0 .. result));
        }
        // dbg!(">1", &popu);
        // test fitness fn
        // let a: f32 = fit(track.clone(), popu[0]);
        // dbg!(">2", &a);
        popu.sort_by(|a, b| fit(track.clone(), *a).total_cmp(&fit(track.clone(), *b)));
        // dbg!(">3", &popu);

        // let mut popu = Vec::<usize>::with_capacity(POPSIZE);
        let fiddy = POPSIZE/2;
        for i in 0..fiddy {
            popu[i+fiddy] = mite(&length, &popu[i]);
        }
    }
    println!("best {} with time: {}",popu[0], fit(track.clone(), popu[0]));
}

fn main() {
	println!("hello rust");
    if false { // debugging crap
        // let a: [i32; 2] = [0,0];
        // let b: [i32; 2] = [3,4];
        // let c = calc_distance(a, b);
        // assert_eq!(c, 5.0);

        // let mut rng = rand::thread_rng();
        // let die = rng.gen_range(1..=6); // Generates a number between 1 and 6, inclusive
        // println!("The die was: {}", die);
        // let random_float: f32 = rng.r#gen();
        // println!("The float was: {}", random_float);
    } else {
        // let mut tracks: Vec<Vec<Point>> = Default::default();
        let tracks: Vec<Vec<Vec<i32>>> = opens();

        for track in tracks {
            // dbg!(&track);
            go(track);
        }
    }
}
