const fs = require('fs');
const readline = require('readline');

const MAPSIZE = 100;
const MAPTIME = 10;
const SPEED = 2;
const POPTRIM = 800;
const POPSIZE = POPTRIM * 2;
const GENSIZE = 1200;
const FILENAME = "../samples/sample_input_small.txt";

// Read file
function badRead(filename = FILENAME){

	//console.log("Reading synchronously");
	data = fs.readFileSync(filename);
	//console.log(data.toString());
	//console.log('\n\n beep \n');
	const lineArray = data.toString().split( '\n' );
	const warehouses = Array();

	for(var i = 0;;){
		//console.log(i, ' > ',lineArray[i]);
		size = parseInt(lineArray[i]);
		//console.log('  ', typeof size, size);
		if(size == 0){
			break;
		}
		const warehouse = Array(size)
		i++;
		for(var j = 0; j < size; i++, j++){
			//console.log('    ', i, ' ', lineArray[i]);
			const currentLine = lineArray[i].split(' ');
			warehouse[j] = Array(parseInt(currentLine[0]), parseInt(currentLine[1]), parseInt(currentLine[2]));
		}
		warehouses.push(warehouse)
		//i = i + size + 1;
	}

	return warehouses;
}

function jankReadLine(filename = FILENAME){	
	const readInterface = readline.createInterface({
		input: fs.createReadStream(filename),
		output: process.stdout,
		console: false
	});
	readInterface.on('line', function(line) {
		console.log(line);
	});
	console.log('\n\n beep \n');
	console.log("Reading synchronously");
	//data = fs.readFileSync(filename);
	data = fs.openSync(filename, 'r');
	for (const line of data.readLines()) {
		console.log(line);
	}
}

// problem solving
function travel(start, finish){
	const distance = Math.pow(Math.pow((start[0] - finish[0]),2) + Math.pow((start[1] - finish[1]),2), 0.5);
	const travelTime = (1/SPEED) * distance;
	//console.log('travelTime: ', travelTime);
	return travelTime;
}

function plotTime(warehouse = [[50, 50, 20]], solver = [1]){
	
	var totalTime = MAPTIME;
	var current = [0, 0];
	
	for (var i = 0; i < warehouse.length; i++){
		//console.log(`i: ${i}`);
		if(solver[i]){
			//console.log('stop');
			//console.log('values: ', current[0], warehouse[i][0], current[1], warehouse[i][1]);
			totalTime = totalTime + travel(current, warehouse[i]) + MAPTIME;
			current = Array(warehouse[i][0], warehouse[i][1]);
		} else {
			//console.log('skip');
			totalTime = totalTime + warehouse[i][2];
		}
	}
	totalTime = totalTime + travel(current, [100, 100]);
	//console.log(`PlotTime: solver ${JSON.stringify(solver)}.  total: ${totalTime}`);
	return totalTime;
}

function compareFunction(a, b) {
  return a[1] - b[1];
}

// smart
function describeSolver(solver){
	return solver.join('');
}

function randomSolver(size = 1){

	const solver = Array(size);
	for(var i = 0; i<size; i++){
		solver[i] = Math.floor(Math.random() * 2);
	}
	//console.log(JSON.stringify(solver));
	return solver;
}

function mutate(parent){
	const solver = Array();
	for(var i = 0; i<parent.length; i++){
		// todo be less gross
		solver[i] = Math.floor(Math.random() * 6);
		if (solver[i] > 1){
			solver[i] = parent[i];
		}
	}
	return solver;
}

function processWarehouse(warehouse){
	//console.log(`now processing ${warehouse} of size ${warehouse.length}`)
	var population = Array(POPSIZE);
	for(var i = 0; i < POPSIZE; i++){
		const solver = randomSolver(warehouse.length);
		population[i] = Array(solver, plotTime(warehouse, solver));
	}
}

function processWarehouse_GA(warehouse){
	//console.log(`now processing ${warehouse}`)
	var population = Array();
	for(var i = 0; i < POPSIZE; i++){
		const solver = randomSolver(warehouse.length);
		population[i] = Array(solver, plotTime(warehouse, solver));
	}
	//console.log('bip');
	for(var g = 1; g < GENSIZE; g++){
		//console.log('beep');
		population.sort(compareFunction);
		
		
		//for(var i = 0; i < POPSIZE; i++){
		//	console.log(`Gen ${g} ${i} ${describeSolver(population[i][0])} ${population[i][1]}`);
		//}
		//console.log('Current: ', population.length, '  description: ', JSON.stringify(population));
		//const child = mutate(population[0][0]);
		
		const nextPopulation = Array();
		for(var i = 0; i < POPTRIM; i++){
			const child = mutate(population[i][0]);
			nextPopulation.push([child, plotTime(warehouse, child)]);
			nextPopulation.push(population[i]);
		}
		//for(var i = 0; i < POPSIZE; i++){
		//	console.log(`Pst ${g} ${i} ${describeSolver(nextPopulation[i][0])} ${nextPopulation[i][1]}`);
		//}
		population = nextPopulation;
	}
	population.sort(compareFunction);
	//console.log('final: ', population[0][0], population[0][1]);
	console.log('final: ', population[0][1]);
}

function processWarehouse_Random(warehouse){
	//console.log(`now processing ${warehouse}`)
	var solver = randomSolver(warehouse.length);
	console.log(`solver: ${solver}`);
	plotTime(warehouse, solver);
}

if (typeof require !== 'undefined' && require.main === module) {
	
	console.log('alright im awake');

	var warehouses = badRead();
	console.log(`there are ${warehouses.length} houses`);
	for( const i in warehouses){
		processWarehouse_GA(warehouses[i]);
	}
	console.log('doneskii');
}
