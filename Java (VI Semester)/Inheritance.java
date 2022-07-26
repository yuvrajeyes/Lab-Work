class Animal {

    private final String name;
    private final int brain;

    public Animal() {
        this("Default Name", 1);
        System.out.println("Empty Constructor is called.");
    }

    public Animal(String name) {
        this(name, 1);
    }

    public Animal(String name, int brain) {
        this.name = name;
        this.brain = brain;
    }

    public int getBrain() {
        return brain;
    }

    public String getName() {
        return name;
    }
}

class Dog extends Animal{

    private final int tail;
    private final int eye;
    private final int legs;

    public Dog (String name) {
        super(name, 1);
        this.tail = 1;
        this.eye = 2;
        this.legs = 4;
    }

    public int getTail() {
        return tail;
    }

    public int getEye() {
        return eye;
    }

    public int getLegs() {
        return legs;
    }
}

public class Inheritance {
    public static void main(String[] args) {
        Dog labrador1 = new Dog("Roxy1");
        System.out.println("Name: " + labrador1.getName());     // inherited from parent(Animal) class
        System.out.println("Brain: " + labrador1.getBrain());   // inherited from parent(Animal) class
        System.out.println("Legs: " + labrador1.getLegs());
        System.out.println("Eye: " + labrador1.getEye());
        System.out.println("Tail: " + labrador1.getTail());

        System.out.println();

        Animal labrador2 = new Animal("Roxy2");
        System.out.println("Name: " + labrador2.getName());
        System.out.println("Brain: " + labrador2.getBrain());

        System.out.println();

        Animal labrador3 = new Dog("Roxy3");
        System.out.println("Name: " + labrador3.getName());
        System.out.println("Brain: " + labrador3.getBrain());
    }
}
