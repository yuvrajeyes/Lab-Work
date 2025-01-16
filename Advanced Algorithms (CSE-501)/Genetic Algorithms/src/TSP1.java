import java.util.ArrayList;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;
import java.util.Comparator;
import java.security.SecureRandom;;

public class TSP1 {
    // CONSTANTS
    private static final int MAX_ITERATIONS = 50;
    private static final int POPULATION_SIZE = 20;
    private static final int NUM_CITIES = 10;
    private static final double CROSSOVER_PROBABILITY = 0.8;
    private static final double MUTATION_PROBABILITY = 0.1;
    private static int NUM_ELITE;
    private static SecureRandom random = new SecureRandom();

    private static final double[][] distanceMatrix = {
        {0, 29, 20, 21, 16, 31, 100, 12, 4, 31},
        {29, 0, 15, 29, 28, 40, 72, 21, 29, 41},
        {20, 15, 0, 15, 14, 25, 81, 9, 23, 27},
        {21, 29, 15, 0, 4, 12, 92, 12, 25, 13},
        {16, 28, 14, 4, 0, 16, 94, 9, 20, 16},
        {31, 40, 25, 12, 16, 0, 95, 24, 36, 3},
        {100, 72, 81, 92, 94, 95, 0, 90, 101, 99},
        {12, 21, 9, 12, 9, 24, 90, 0, 15, 25},
        {4, 29, 23, 25, 20, 36, 101, 15, 0, 35},
        {31, 41, 27, 13, 16, 3, 99, 25, 35, 0}
    };

    private static class Chromosome {
        ArrayList<Integer> cities;
        double fitness = 0.0;
        double randomprob;

        Chromosome(ArrayList<Integer> cities) {
            this.cities = cities;
            this.randomprob = random.nextDouble();
        }

        // TIME COMPLEXITY: O(NUM_CITIES)
        double calculateFitness(double[][] distanceMatrix) {
            // Calculate total distance traveled in the tour (fitness)
            double totalDistance = 0.0;
            for (int i = 0; i < NUM_CITIES - 1; i++) {
                int city1 = this.cities.get(i) - 1; // Adjust for 0-based index
                int city2 = this.cities.get(i + 1) - 1; // Adjust for 0-based index
                double distance = distanceMatrix[city1][city2];
                totalDistance += distance;
            }
            // Add distance from last city to the starting city to complete the tour
            int firstCity = this.cities.get(0) - 1; // Adjust for 0-based index
            int lastCity = this.cities.get(cities.size() - 1) - 1; // Adjust for 0-based index
            totalDistance += distanceMatrix[lastCity][firstCity];

            // Handle division by zero
            if (totalDistance == 0.) {
                this.fitness = Double.MAX_VALUE; // Set fitness to a large value
            } else {
                this.fitness = 1.0/totalDistance;
            }
            return this.fitness;
        }
    }

    // TIME COMPLEXITY: O(NUM_CITIES^2)
    private static Chromosome orderedCrossover(Chromosome parent1, Chromosome parent2, int startPos, int endPos) {
        // Create an empty offspring
        ArrayList<Integer> childCities = new ArrayList<>(Collections.nCopies(NUM_CITIES, 0));

        // Copy the selected subset from parent1 to the offspring
        for (int i = startPos; i <= endPos; i++) {
            childCities.set(i, parent1.cities.get(i));
        }

        // Fill the remaining positions with cities from parent2, maintaining the order of appearance
        int parent2Index = 0, j=0;
        for (int i = 0; i < NUM_CITIES; i++) {
            if (!childCities.contains(parent2.cities.get(parent2Index))) {
                for (; j < NUM_CITIES; j++) {
                    if (childCities.get(j) == 0) {
                        childCities.set(j, parent2.cities.get(parent2Index));
                        break;
                    }
                }
            }
            parent2Index = (parent2Index + 1) % NUM_CITIES;
        }

        return new Chromosome(childCities);
    }

    // TIME COMPLEXITY: O(NUM_CITIES)
    private static Chromosome alleleReplacementMutation(Chromosome chromosome) {
        int pos1 = random.nextInt(NUM_CITIES);
        int pos2 = random.nextInt(NUM_CITIES);
    
        // Ensure pos1 is different from pos2
        while (pos1 == pos2) {
            pos2 = random.nextInt(NUM_CITIES);
        }
    
        // Create a new chromosome with mutated alleles
        ArrayList<Integer> newCities = new ArrayList<>(chromosome.cities);
        int temp = newCities.get(pos1);
        newCities.set(pos1, newCities.get(pos2));
        newCities.set(pos2, temp);
    
        return new Chromosome(newCities);
    }
    

    public static void main(String[] args) {
        // Initialize the population with Permutation Encoding of cities, TIME COMPLEXITY: O(POPULATION_SIZE*NUM_CITIES)
        ArrayList<Chromosome> population = new ArrayList<>();
        for (int i = 0; i < POPULATION_SIZE; i++) {
            ArrayList<Integer> randomPermutation = new ArrayList<>();
            for (int j = 1; j <= NUM_CITIES; j++) {
                randomPermutation.add(j);
            }
            Collections.shuffle(randomPermutation, random);
            population.add(new Chromosome(randomPermutation));
        }

        // TIME COMPLEXITY: O(MAX_ITERATION)
        for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
            // Compute the fitness value of each Chromosome, TIME COMPLEXITY: O(POPULATION_SIZE)
            for (Chromosome chromosome : population) {
                chromosome.calculateFitness(distanceMatrix);
            }

            Collections.sort(population, Comparator.comparingDouble(chromosome -> -chromosome.fitness)); // Sort in descending order of fitness
            
            // Selection: Calculate the average fitness of feasible solutions, TIME COMPLEXITY: O(POPULATION_SIZE)
            double totalFitness = 0.0;
            for (Chromosome chromosome : population) {
                totalFitness += chromosome.fitness;
            }
            double averageFitness = totalFitness / POPULATION_SIZE;

            // Select chromosomes with fitness greater than average fitness, TIME COMPLEXITY: O(POPULATION_SIZE)
            ArrayList<Chromosome> selectedChromosomes = new ArrayList<>();
            for (Chromosome chromosome : population) {
                if (chromosome.fitness > averageFitness) {
                    selectedChromosomes.add(chromosome);
                }
            }

            Set<Chromosome> crossoverChromosome = new HashSet<>(selectedChromosomes);
            for (Chromosome chromosome: population) {
                if (chromosome.randomprob < CROSSOVER_PROBABILITY) {
                    crossoverChromosome.add(chromosome);
                }
            }
            ArrayList<Chromosome> crossoverChromosomes = new ArrayList<>(crossoverChromosome);

            ArrayList<Chromosome> offspringPopulation = new ArrayList<>();
            int no_offspring_crossover = 0;
            int no_offspring_mutation = 0;
            while (crossoverChromosomes.size() >= 2) {
                // Crossover: Order Crossover (OX) with crossover probability
                Chromosome parent1 = crossoverChromosomes.remove(0);
                Chromosome parent2 = crossoverChromosomes.remove(0);
                    int startPos = random.nextInt(NUM_CITIES);
                    int endPos = random.nextInt(NUM_CITIES);

                // Ensure start position is smaller than the end position
                if (startPos > endPos) {
                    int temp = startPos;
                    startPos = endPos;
                    endPos = temp;
                }
                offspringPopulation.add(orderedCrossover(parent1, parent2, startPos, endPos));
                // offspringPopulation.add(orderedCrossover(parent2, parent1, startPos, endPos));
                no_offspring_crossover++;
            }

            int offspring_size = offspringPopulation.size();
            for (int i=0; i < offspring_size; i++) {
                Chromosome parent = offspringPopulation.get(i);
                if (parent.randomprob < MUTATION_PROBABILITY) {
                    offspringPopulation.add(alleleReplacementMutation(parent));
                    no_offspring_mutation++;
                }
            }

            NUM_ELITE = POPULATION_SIZE - (no_offspring_crossover + no_offspring_mutation);
            ArrayList<Chromosome> eliteChromosomes = new ArrayList<>();
            for (int i=0; i < NUM_ELITE; i++) {
                eliteChromosomes.add(population.get(i));
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
        System.out.println("Best Solution: " + bestSolution.cities);
        System.out.println("Shortest Tour Distance: " + 1.0 / bestSolution.fitness);
    }
}