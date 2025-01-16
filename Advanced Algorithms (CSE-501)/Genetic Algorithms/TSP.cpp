#include <bits/stdc++.h>
using namespace std;

#define POPULATION_SIZE 20
#define NUM_CITIES 10
#define MAX_ITERATION 50
#define CROSSOVER_PROBABILITY 0.8
#define MUTATION_PROBABILITY 0.1
vector<vector<float>> distance_map = {
      {0, 29, 20, 21, 16, 31, 100, 12, 4, 31},
      {29, 0, 15, 29, 28, 40, 72, 21, 29, 41},
      {20, 15, 0, 15, 14, 25, 81, 9, 23, 27},
      {21, 29, 15, 0, 4, 12, 92, 12, 25, 13},
      {16, 28, 14, 4, 0, 16, 94, 9, 20, 16},
      {31, 40, 25, 12, 16, 0, 95, 24, 36, 3},
      {100, 72, 81, 92, 94, 95, 0, 90, 101, 99},
      {12, 21, 9, 12, 9, 24, 90, 0, 15, 25},
      {4, 29, 23, 25, 20, 36, 101, 15, 0, 35},
      {31, 41, 27, 13, 16, 3, 99, 25, 35, 0}};

// Create a random number generator engine
random_device rd;                                 // Obtain a random seed from the hardware
mt19937 randomGenerator(rd());                    // Initialize the Mersenne Twister engine
uniform_real_distribution<> Double(0.0, 1.0);     // Define the range [0.0, 1.0)
uniform_int_distribution<> Int(0, NUM_CITIES-1);

class Chromosome {
public:
    vector<int> cities;
    double fitness;
    double randomprob = Double(randomGenerator);

    Chromosome() : cities(NUM_CITIES, 0) {}

    Chromosome(vector<int> cities) {
      this->cities = cities;
      this->fitness = calculateFitness();
    }

    double calculateFitness() {
      // Calculate total distance traveled in the tour (fitness)
      double totalDistance = 0.0;
      for (int i = 0; i < NUM_CITIES - 1; i++)
          totalDistance += distance_map[this->cities[i]][this->cities[i+1]];
      
      // Add distance from last city to the starting city to complete the tour
      totalDistance += distance_map[this->cities[NUM_CITIES-1]][this->cities[0]];

      this->fitness = 1.0 / totalDistance;
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
            offspring1.cities[i] = population[k].cities[i];
            offspring2.cities[i] = population[k + 1].cities[i];
        }
        //Copy remaining elements
        int parent1Index = 0, parent2Index = 0, j=0, l=0;
        for (int i = 0; i < NUM_CITIES; i++) {
            if (find(offspring1.cities.begin(), offspring1.cities.end(), population[k + 1].cities[i]) == offspring1.cities.end()) {
                for (; j < NUM_CITIES; j++) {
                    if (offspring1.cities[j] == 0) {
                        offspring1.cities[j] = population[k + 1].cities[parent2Index];
                        break;
                    }
                }
            }
            parent2Index = (parent2Index + 1) % NUM_CITIES;  
          
            if (find(offspring2.cities.begin(), offspring2.cities.end(), population[k].cities[i]) == offspring2.cities.end()) {
                for (; l < NUM_CITIES; l++) {
                    if (offspring2.cities[l] == 0) {
                        offspring2.cities[l] = population[k].cities[parent1Index];
                        break;
                    }
                }
            }
            parent1Index = (parent1Index + 1) % NUM_CITIES;
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

        swap(offspring.cities[pos1], offspring.cities[pos2]);
        offspring.calculateFitness();
        mutatedPopulation.emplace_back(offspring);
    }
    return mutatedPopulation;
}

int main() {
    vector<Chromosome> population;
    for (int i=0; i<POPULATION_SIZE; i++) {    // O(POPULATION_SIZE*NUM_CITIES)
        vector<int> randomPermutation;
        for (int j=0; j<NUM_CITIES; j++) 
            randomPermutation.emplace_back(j);
        
        shuffle(randomPermutation.begin(), randomPermutation.end(), randomGenerator);
        population.emplace_back(randomPermutation);
    }

    for (int p = 0; p < MAX_ITERATION; p++) {   // O(MAX_ITERATION*(POPULATION_SIZE*NUM_CITIES^2 + POPULATION_SIZE*log(POPULATION_SIZE)))
        vector<Chromosome> selectedPopulation;
        // Select chromosomes with fitness greater than average fitness, TIME COMPLEXITY: O(POPULATION_SIZE)
        double avgFitness = accumulate(population.begin(), population.end(), 0.0,
            [](double sum, const Chromosome& chromosome) {
                return sum + chromosome.fitness;})/population.size();

        for (Chromosome chromosome : population)
            if (chromosome.fitness >= avgFitness or chromosome.randomprob < CROSSOVER_PROBABILITY)
                selectedPopulation.emplace_back(chromosome);

        vector<Chromosome> crossoverPopulation = orderedCrossover(selectedPopulation); // O(POPULATION_SIZE*NUM_CITIES)

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
    for (int city : bestSolution.cities)
        cout << city+1 << " ";
    
    cout << "\nShortest Tour Distance: " << 1.0 / bestSolution.fitness << endl;
      
    return 0;
}