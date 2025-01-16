#include <bits/stdc++.h>
using namespace std;

#define POPULATION_SIZE 3
#define NUM_JOBS 10
#define NUM_MACHINES 3
#define MAX_ITERATION 1
#define CROSSOVER_PROBABILITY 0.8
#define MUTATION_PROBABILITY 0.1
int TOTAL_TASK;
vector<vector<pair<int, int>>> jobMatrix = {
        {{1, 3}, {2, 2}, {3, 5}},
        {{3, 2}, {1, 2}, {0, 0}},
        {{2, 4}, {3, 1}, {1, 2}},
        {{1, 2}, {2, 2}, {0, 0}},
        {{3, 3}, {1, 4}, {2, 2}},
        {{2, 1}, {3, 2}, {0, 0}},
        {{1, 2}, {2, 1}, {3, 2}},
        {{3, 2}, {1, 1}, {2, 3}},
        {{2, 2}, {3, 1}, {1, 2}},
        {{1, 3}, {2, 5}, {0, 0}},
    };

// Create a random number generator engine
random_device rd;                                 // Obtain a random seed from the hardware
mt19937 randomGenerator(rd());                    // Initialize the Mersenne Twister engine
uniform_real_distribution<> Double(0.0, 1.0);     // Define the range [0.0, 1.0)
uniform_int_distribution<> Int(0, NUM_JOBS-1);    // Define the range [0, NUM_JOBS-1)

struct Task {
    int job_id, task_no, machine_no, processing_time;

    bool operator==(const Task& other) const {
        return job_id == other.job_id && task_no == other.task_no && machine_no == other.machine_no;
    }
};

class Chromosome {
public:
    vector<Task> jobs;
    double fitness, randomprob = Double(randomGenerator);

    Chromosome() {}

    Chromosome(vector<Task> jobs) {
      this->jobs = jobs;
      this->fitness = calculateFitness();
    }

    void set_size(int size) {
        Task task = {0, 0, 0, 0};
        for (int i=0; i<size; i++)
            this->jobs.push_back(task);
    }

    double calculateFitness() {
        unordered_map<int, int> machine_completion_times, task_completion_times;
        for (Task &job : this->jobs) {
            int prev_machine_completion = machine_completion_times[job.machine_no];
            int prev_task_completion = task_completion_times[job.job_id];

            int start_time = max(prev_machine_completion, prev_task_completion);
            int end_time = start_time + job.processing_time;

            machine_completion_times[job.machine_no] = end_time;
            task_completion_times[job.job_id] = end_time;
        }
        int makespan = 0;
        for (auto &entry : machine_completion_times)
            makespan = max(makespan, entry.second);
        
        return 1.0/makespan;
    }
};

void print_chromosome(Chromosome & chromosome) {
    cout<<"[ ";
    for (Task &task: chromosome.jobs)
        cout<<"("<<task.job_id+1<<", "<<task.task_no+1<<", "<<task.machine_no<<", "<<task.processing_time<<"), ";
    cout<<"\b]"<<endl;
}

void print_population(vector<Chromosome> &population) {
    for (Chromosome &chromosome : population) print_chromosome(chromosome);
}

vector<Chromosome> generatePopulation(vector<vector<pair<int, int>>> &jobMatrix) {
    vector<int> seq;
    for (int i = 0; i < NUM_JOBS; i++)
        for (int j = 0; j < NUM_MACHINES; j++)
            if (jobMatrix[i][j].second != 0) 
                seq.push_back(i);

    TOTAL_TASK = seq.size();
    vector<Chromosome> population;
    for (int x = 0; x < POPULATION_SIZE; x++) {
        shuffle(seq.begin(), seq.end(), randomGenerator);

        vector<Task> chromosomeJobs;
        unordered_map<int, int> mp;
        
        for (int jobIndex : seq) {
            int taskIndex;
            if (mp[jobIndex]) {
                taskIndex = mp[jobIndex]++;
            } else {
                taskIndex = 0;
                mp[jobIndex] = 1;
            }
            // job_id, task_no, machine_no, processing_time
            Task task = {jobIndex, taskIndex, jobMatrix[jobIndex][taskIndex].first, jobMatrix[jobIndex][taskIndex].second};
            chromosomeJobs.emplace_back(task);
        }
        population.emplace_back(chromosomeJobs);
    }
    return population;
}

vector<Chromosome> crossover(vector<Chromosome> &population) {
    vector<Chromosome> crossoverPopulation;
    for (int k=0; k + 1 < population.size(); k+=2) {  
        Chromosome parent1 = population[k], parent2 = population[k+1];
        Chromosome offspring1, offspring2;
        offspring1.set_size(TOTAL_TASK);
        offspring2.set_size(TOTAL_TASK);

        int job_id = Int(randomGenerator);
        // Copy same elements
        for (int i = 0; i < TOTAL_TASK; i++) {
            if (parent1.jobs[i].job_id == job_id)
                offspring1.jobs[i] = parent1.jobs[i];
            if (parent2.jobs[i].job_id == job_id)
                offspring2.jobs[i] = parent2.jobs[i];
        }
        //Copy remaining elements
        int parent1Index = 0, parent2Index = 0, j=0, l=0;
        for (int i = 0; i < TOTAL_TASK; i++) {
            if (find(offspring1.jobs.begin(), offspring1.jobs.end(), parent2.jobs[i]) == offspring1.jobs.end())
                for (; j < TOTAL_TASK; j++)
                    if (offspring1.jobs[j].job_id == 0) {
                        offspring1.jobs[j] = parent2.jobs[parent2Index];
                        break;
                    }
            parent2Index = (parent2Index + 1) % TOTAL_TASK;
            if (find(offspring2.jobs.begin(), offspring2.jobs.end(), parent1.jobs[i]) == offspring2.jobs.end()) 
                for (; l < NUM_JOBS; l++) 
                    if (offspring2.jobs[l].job_id == 0) {
                        offspring2.jobs[l] = parent1.jobs[parent1Index];
                        break;
                    }
            parent1Index = (parent1Index + 1) % TOTAL_TASK;
            cout<<"UUUUU:\n";
            for (auto o: offspring1.jobs) cout<<o.job_id<<" ";
            cout<<endl;
        }
        offspring1.calculateFitness();
        offspring2.calculateFitness();

        crossoverPopulation.emplace_back(offspring1);        
        crossoverPopulation.emplace_back(offspring2);
    }
    return crossoverPopulation;
}

vector<Chromosome> mutation(vector<Chromosome> &population) {
    uniform_int_distribution<> RandomGene(0, TOTAL_TASK-1);
    vector<Chromosome> mutatedPopulation;
    for (Chromosome parent : population) {
        Chromosome offspring=parent;
        int geneA = RandomGene(randomGenerator);
        int geneB = RandomGene(randomGenerator);

        while (geneA == geneB)
          geneB = RandomGene(randomGenerator);

        for (int i=geneA+1; i<geneB; i++) {
            offspring.jobs[i-1] = parent.jobs[i];
        }
        offspring.jobs[geneB] = parent.jobs[geneA];

        for (int i=geneB+1; i<TOTAL_TASK; i++) {
            offspring.jobs[i] = parent.jobs[i];
        }

        offspring.calculateFitness();
        mutatedPopulation.emplace_back(offspring);
    }
    return mutatedPopulation;
}

int main() {
    vector<Chromosome> population = generatePopulation(jobMatrix);  // O(POPULATION_SIZE*NUM_JOBS*NUM_MACHINES)

    for (int p = 0; p < MAX_ITERATION; p++) {   // O(MAX_ITERATION*(POPULATION_SIZE*(NUM_JOBS*NUM_MACHINES)^2 + POPULATION_SIZE*log(POPULATION_SIZE)))
        vector<Chromosome> selectedPopulation;
        // Select chromosomes with fitness greater than average fitness, TIME COMPLEXITY: O(POPULATION_SIZE)
        double avgFitness = accumulate(population.begin(), population.end(), 0.0,
            [](double sum, const Chromosome& chromosome) {
                return sum + chromosome.fitness;})/population.size();

        for (Chromosome chromosome : population)
            if (chromosome.fitness >= avgFitness or chromosome.randomprob < CROSSOVER_PROBABILITY)
                selectedPopulation.emplace_back(chromosome);

        vector<Chromosome> crossoverPopulation = crossover(selectedPopulation); // O(POPULATION_SIZE*(NUM_JOBS*NUM_MACHINES)^2)

        selectedPopulation.clear();
        for (Chromosome chromosome : crossoverPopulation)
            if (chromosome.randomprob < MUTATION_PROBABILITY)
                selectedPopulation.emplace_back(chromosome);

        // Mutation
        vector<Chromosome> mutatedPop = mutation(selectedPopulation);  // O(POPULATION_SIZE*NUM_JOBS*NUM_MACHINES)

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
    for (auto city : bestSolution.jobs)
        cout << city.job_id+1 << " ";
    cout << "\nMakespan: " << 1.0 / bestSolution.fitness << endl;
    return 0;
}