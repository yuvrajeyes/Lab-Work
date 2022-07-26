public class Operators {

    public static void main(String[] args) {
        System.out.println("Relational Operators are ==, !=, >, <, >=, <= .");
        relationalOperators(7, 11);

        System.out.println();
        System.out.println("Logical Operators are &(logical AND), ||(logical OR), !(logical NOT).");
        logicalOperators(5, 3, 8);
    }

    public static void relationalOperators(int a, int b) {
        // print the value of a and b
        System.out.println("a is " + a + " and b is " + b);

        // == operator
        System.out.println(a == b);

        // != operator
        System.out.println(a != b);

        // > operator
        System.out.println(a > b);

        // < operator
        System.out.println(a < b);

        // >= operator
        System.out.println(a >= b);

        // <= operator
        System.out.println(a <= b);
    }

    public static void logicalOperators(int a, int b, int c) {
        // print the value of a, b and c
        System.out.println("a is " + a + ", b is " + b + " and c is " + c);
        // && operator
        System.out.println((a > b) && (c > a));
        System.out.println((a > b) && (c < a));

        // || operator
        System.out.println((a < b) || (c > a));
        System.out.println((a > b) || (c < a));
        System.out.println((a < b) || (c < a));

        // ! operator
        System.out.println(!(a == b));
        System.out.println(!(a > b));
    }
}

