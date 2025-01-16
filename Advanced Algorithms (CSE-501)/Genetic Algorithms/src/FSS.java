import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Random;

public class FSS {
    private static final int MAX_ITERATIONS = 50;
    private static final int POPULATION_SIZE = 20;
    private static final int NUM_JOBS = 10;
    private static final int NUM_MACHINES = 3;
    private static final double CROSSOVER_PROBABILITY = 0.80;
    private static final double MUTATION_PROBABILITY = 0.10;
    private static final double NUM_ELITE = 2;
    private static final Random random = new Random();
    private static final double [][] jobMatrix = {
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

    private static class Chromosome {
        ArrayList<Integer> jobs = new ArrayList<>();
        double fitness = 0.0;

        Chromosome(ArrayList<Integer> jobs) {
            this.jobs = jobs;
        }

        double calculateFitness(double [][] jobMatrix) {
            double[][] completionTime = new double[NUM_JOBS][NUM_MACHINES];

            // Initialize the completion time of the first job on each machine
            for (int i = 0; i < NUM_MACHINES; i++) {
                completionTime[0][i] += jobMatrix[this.jobs.get(0)-1][i];
            }

            // Calculate the completion time for each job on each machine
            for (int i=1; i < NUM_JOBS; i++) {
                for (int j=0; j < NUM_MACHINES; j++) {
                    double prevCompletionTime;
                    if (j==0) {
                        prevCompletionTime = completionTime[i-1][j];
                    } else {
                        prevCompletionTime = Math.max(completionTime[i-1][j], completionTime[i][j-1]);
                    }
                    double currProcessingTime = jobMatrix[this.jobs.get(i)-1][j];
                    completionTime[i][j] = prevCompletionTime + currProcessingTime;
                }
            }

            // Find the completion time for last job on any machine
            double makespan = completionTime[NUM_JOBS - 1][2];  // makespan = lastCompleted job
            for (int i = 1; i < NUM_MACHINES; i++) {
                if (completionTime[NUM_JOBS- 1][i] > makespan) {
                    makespan = completionTime[NUM_JOBS- 1][i];
                }
            }

            this.fitness = 1.0 / makespan;
            return this.fitness;
        }
    }

    private static Chromosome orderedCrossover(Chromosome parent1, Chromosome parent2, int startPos, int endPos) {
        // Create an empty offspring
        ArrayList<Integer> childJobs = new ArrayList<>(Collections.nCopies(NUM_JOBS, 0));

        // Copy the selected subset from parent1 to the offspring
        for (int i = startPos; i <= endPos; i++) {
            childJobs.set(i, parent1.jobs.get(i));
        }

        // Fill the remaining positions with cities from parent2, maintaining the order of appearance
        int parent2Index = 0;
        for (int i = 0; i < NUM_JOBS; i++) {
            if (!childJobs.contains(parent2.jobs.get(parent2Index))) {
                for (int j = 0; j < NUM_JOBS; j++) {
                    if (childJobs.get(j) == 0) {
                        childJobs.set(j, parent2.jobs.get(parent2Index));
                        break;
                    }
                }
            }
            parent2Index = (parent2Index + 1) % NUM_JOBS;
        }

        return new Chromosome(childJobs);
    }

    private static Chromosome alleleReplacementMutation(Chromosome chromosome) {
        int pos1 = random.nextInt(NUM_JOBS);
        int pos2 = random.nextInt(NUM_JOBS);
    
        // Ensure pos1 is different from pos2
        while (pos1 == pos2) {
            pos2 = random.nextInt(NUM_JOBS);
        }
    
        // Create a new chromosome with mutated alleles
        ArrayList<Integer> newJob = new ArrayList<>(chromosome.jobs);
        Collections.swap(newJob, pos1, pos2);

        return new Chromosome(newJob);
    }

    public static void main(String[] args) {
        // Initialize the population with Permutation Encoding of jobs
        ArrayList<Chromosome> population = new ArrayList<>();
        for (int i=0; i < POPULATION_SIZE; i++) {
            ArrayList<Integer> randomPermutation = new ArrayList<>();
            for (int j=1; j <= NUM_JOBS; j++) {
                randomPermutation.add(j);
            }
            Collections.shuffle(randomPermutation, random);
            population.add(new Chromosome(randomPermutation));
        }

        for (int iteration=0; iteration < MAX_ITERATIONS; iteration++) {
            // Compute the fitness value of each Chromosomes
            for (Chromosome chromosome: population) {
                chromosome.calculateFitness(jobMatrix);
            }

            // Selection: Elitism: Preserve the top NUM_ELITE chromosomes in the offspring population
            ArrayList<Chromosome> eliteChromosomes = new ArrayList<>();
            Collections.sort(population, Comparator.comparingDouble(chromosome -> -chromosome.fitness));  // sort in decreasing order of fitness
            for (int i=0; i < NUM_ELITE; i++) {
                eliteChromosomes.add(population.get(i));
            }

            // Selection: Calculate the average fitness of feasible solutions
            double totalFitness = 0.0;
            for (Chromosome chromosome : population) {
                totalFitness += chromosome.fitness;
            }
            double averageFitness = totalFitness / POPULATION_SIZE;

            // Select chromosomes with fitness greater than average fitness
            ArrayList<Chromosome> selectedChromosomes = new ArrayList<>();
            for (Chromosome chromosome : population) {
                if (chromosome.fitness > averageFitness) {
                    selectedChromosomes.add(chromosome);
                }
            }

            ArrayList<Chromosome> offspringPopulation = new ArrayList<>();
            while (offspringPopulation.size() < POPULATION_SIZE-NUM_ELITE) {
                // Check if selectedChromosomes is not empty before proceeding with selection
                if (!selectedChromosomes.isEmpty()) {
                    // Crossover: Order Crossover (OX) with crossover probability
                    Chromosome parent1 = selectedChromosomes.get(random.nextInt(selectedChromosomes.size()));
                    Chromosome parent2 = selectedChromosomes.get(random.nextInt(selectedChromosomes.size()));
                    if (random.nextDouble() < CROSSOVER_PROBABILITY) {
                        int startPos = random.nextInt(NUM_JOBS);
                        int endPos = random.nextInt(NUM_JOBS);

                        // Just ensure that startPos is smaller than the endPos
                        if (startPos > endPos) {
                            int temp = startPos;
                            startPos = endPos;
                            endPos = temp;
                        }
                        offspringPopulation.add(orderedCrossover(parent1, parent2, startPos, endPos));
                        offspringPopulation.add(orderedCrossover(parent2, parent1, startPos, endPos));
                    } else {
                        offspringPopulation.add(parent1);
                        offspringPopulation.add(parent2);
                    }
                } else {
                    // If selectedChromosomes is empty, add a randomly selected chromosome from the population
                    offspringPopulation.add(population.get(random.nextInt(population.size())));
                }

                // Mutation : swap mutation with MUTATION_PROBABILITY
                Chromosome parent = population.get(random.nextInt(population.size()));
                if (random.nextDouble() < MUTATION_PROBABILITY) {
                    offspringPopulation.add(alleleReplacementMutation(parent));
                } else {
                    offspringPopulation.add(parent);
                }
            }

            // Replace the old population with the new offspring population (including elites)
            population = new ArrayList<>(offspringPopulation);
            population.addAll(eliteChromosomes);
        }

        // Find the best solution (chromosome with the highest fitness value)
        Chromosome bestSolution = population.get(0);
        for (Chromosome chromosome : population) {
            if (chromosome.fitness > bestSolution.fitness) {
                bestSolution = chromosome;
            }
        }

        // Output the best solution
        System.out.println("Best Solution: " + bestSolution.jobs);
        System.out.println("Shortest Tour Distance: " + 1.0 / bestSolution.fitness);
    }
}
