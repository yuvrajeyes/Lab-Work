#include <bits/stdc++.h>
using namespace std;

#define POPULATION_SIZE 20
#define NUM_JOBS 10
#define NUM_MACHINES 3
#define MAX_ITERATION 50
#define CROSSOVER_PROBABILITY 0.8
#define MUTATION_PROBABILITY 0.1

vector<vector<float>> jobMatrix = {
        {8, 2, 6},
        {4, 7, 3},
        {9, 5, 10},
        {1, 6, 8},
        {3, 9, 2},
        {5, 10, 7},
        {6, 1, 4},
        {10, 3, 5},
        {2, 4, 9},
        {7, 8, 1}
    };


// Create a random number generator engine
random_device rd;                                 // Obtain a random seed from the hardware
mt19937 randomGenerator(rd());                    // Initialize the Mersenne Twister engine
uniform_real_distribution<> Double(0.0, 1.0);     // Define the range [0.0, 1.0)
uniform_int_distribution<> Int(0, NUM_JOBS-1);    // Define the range [0, NUM_JOBS-1)


class Chromosome {
public:
    vector<int> jobs;
    double fitness;
    double randomprob = Double(randomGenerator);

    Chromosome() : jobs(NUM_JOBS, 0) {}

    Chromosome(vector<int> jobs) {
      this->jobs = jobs;
      this->fitness = calculateFitness();
    }

    double calculateFitness() {
        double completionTime [NUM_JOBS][NUM_MACHINES];

        // Initialize the completion time of the first job on each machine
        for (int i = 0; i < NUM_MACHINES; i++) {
            completionTime[0][i] += jobMatrix[this->jobs[0]][i];
        }

        // Calculate the completion time for each job on each machine
        for (int i=1; i < NUM_JOBS; i++) {
            for (int j=0; j < NUM_MACHINES; j++) {
                double prevCompletionTime;
                if (j==0) {
                    prevCompletionTime = completionTime[i-1][j];
                } else {
                    prevCompletionTime = max(completionTime[i-1][j], completionTime[i][j-1]);
                }
                double currProcessingTime = jobMatrix[this->jobs[i]][j];
                completionTime[i][j] = prevCompletionTime + currProcessingTime;
            }
        }

        // Find the completion time for last job on any machine
        this->fitness = 1.0 / completionTime[NUM_JOBS - 1][NUM_MACHINES-1];  // makespan = lastCompleted job
        return this->fitness;
    }
};

vector<Chromosome> orderedCrossover(vector<Chromosome> population) {
    vector<Chromosome> crossoverPopulation;
    for (int k=0; k + 1 < population.size(); k+=2) {  
        Chromosome offspring1, offspring2;

        int startPos = Int(randomGenerator);
        int endPos = Int(randomGenerator);

        if (startPos > endPos) 
            swap(startPos, endPos);
        
        //Copy same elements
        for (int i = startPos; i < endPos; i++) {
            offspring1.jobs[i] = population[k].jobs[i];
            offspring2.jobs[i] = population[k + 1].jobs[i];
        }
        //Copy remaining elements
        int parent1Index = 0, parent2Index = 0, j=0, l=0;
        for (int i = 0; i < NUM_JOBS; i++) {
            if (find(offspring1.jobs.begin(), offspring1.jobs.end(), population[k + 1].jobs[i]) == offspring1.jobs.end()) {
                for (; j < NUM_JOBS; j++) {
                    if (offspring1.jobs[j] == 0) {
                        offspring1.jobs[j] = population[k + 1].jobs[parent2Index];
                        break;
                    }
                }
            }
            parent2Index = (parent2Index + 1) % NUM_JOBS;  
          
            if (find(offspring2.jobs.begin(), offspring2.jobs.end(), population[k].jobs[i]) == offspring2.jobs.end()) {
                for (; l < NUM_JOBS; l++) {
                    if (offspring2.jobs[l] == 0) {
                        offspring2.jobs[l] = population[k].jobs[parent1Index];
                        break;
                    }
                }
            }
            parent1Index = (parent1Index + 1) % NUM_JOBS;
        }

        offspring1.calculateFitness();
        offspring2.calculateFitness();

        crossoverPopulation.emplace_back(offspring1);        
        crossoverPopulation.emplace_back(offspring2);
    }
    return crossoverPopulation;
}

vector<Chromosome> alleleReplacementMutation(vector<Chromosome> population) {
    vector<Chromosome> mutatedPopulation;
    for (Chromosome offspring : population) {
        int pos1 = Int(randomGenerator);
        int pos2 = Int(randomGenerator);

        while (pos1 == pos2)
          pos2 = Int(randomGenerator);

        swap(offspring.jobs[pos1], offspring.jobs[pos2]);
        offspring.calculateFitness();
        mutatedPopulation.emplace_back(offspring);
    }
    return mutatedPopulation;
}

int main() {
    vector<Chromosome> population;
    for (int i=0; i<POPULATION_SIZE; i++) {    // O(POPULATION_SIZE*NUM_JOBS)
        vector<int> randomPermutation;
        for (int j=0; j<NUM_JOBS; j++) 
            randomPermutation.emplace_back(j);
        
        shuffle(randomPermutation.begin(), randomPermutation.end(), randomGenerator);
        population.emplace_back(randomPermutation);
    }

    for (int p = 0; p < MAX_ITERATION; p++) {   // O(MAX_ITERATION*(POPULATION_SIZE*NUM_JOBS^2 + POPULATION_SIZE*log(POPULATION_SIZE)))
        vector<Chromosome> selectedPopulation;
        // Select chromosomes with fitness greater than average fitness, TIME COMPLEXITY: O(POPULATION_SIZE)
        double avgFitness = accumulate(population.begin(), population.end(), 0.0,
            [](double sum, const Chromosome& chromosome) {
                return sum + chromosome.fitness;})/population.size();

        for (Chromosome chromosome : population)
            if (chromosome.fitness >= avgFitness or chromosome.randomprob < CROSSOVER_PROBABILITY)
                selectedPopulation.emplace_back(chromosome);

        vector<Chromosome> crossoverPopulation = orderedCrossover(selectedPopulation); // O(POPULATION_SIZE*NUM_JOBS^2)

        selectedPopulation.clear();
        for (Chromosome chromosome : crossoverPopulation)
            if (chromosome.randomprob < MUTATION_PROBABILITY)
                selectedPopulation.emplace_back(chromosome);

        // Mutation
        vector<Chromosome> mutatedPop = alleleReplacementMutation(selectedPopulation);  // O(POPULATION_SIZE)

        // Elitism
        sort(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) { return a.fitness > b.fitness;});

        int NUM_ELITES = POPULATION_SIZE - crossoverPopulation.size() - mutatedPop.size();

        if (NUM_ELITES <=0) NUM_ELITES = 2;
        vector<Chromosome> elitePop;
        copy_n(population.begin(), NUM_ELITES, back_inserter(elitePop));

        // New population
        population = crossoverPopulation;
        population.insert(population.end(), mutatedPop.begin(), mutatedPop.end());
        population.insert(population.end(), elitePop.begin(), elitePop.end());
        
        sort(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) { return a.fitness > b.fitness;});

        vector<Chromosome> newPopulation(population.begin(), population.begin()+POPULATION_SIZE);

        population = newPopulation;
    }
    
    // Output the best solution
    Chromosome bestSolution = population[0];
    for (const Chromosome& chromosome : population) 
        if (chromosome.fitness > bestSolution.fitness) 
            bestSolution = chromosome;

    cout << "Best Solution: ";
    for (int city : bestSolution.jobs)
        cout << city+1 << " ";
    
    cout << "\nMin Time Taken: " << 1.0 / bestSolution.fitness << endl;
      
    return 0;
}