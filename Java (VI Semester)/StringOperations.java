import java.util.Arrays;

public class StringOperations {
    public static void main(String[] args) {
        String str1 = "Hello";
        String str2 = "World"; 

        //1. concatenation
        System.out.println("1(a). concatenation using + operator -> " + str1 + str2);
        System.out.println("1(b) . concatenation using concat() -> " + str1.concat(str2));

        //2. equals()
        System.out.println("2. equals() -> " + str1.equals("hElLo"));

        //3.
        System.out.println("3. equalsIgnoreCase() -> " + str1.equalsIgnoreCase("hElLo"));

        //4. replace()
        System.out.println("4. replace() -> " + str1.replace(" ", ", " + str2));

        //5. toUpperCase() -> It returns the string in uppercase.
        System.out.println("5. toUpperCase() -> " + "It returns the string in uppercase.".toUpperCase());

        //6. toLowerCase() -> It returns the string in lowercase.
        System.out.println("6. toLowerCase() -> " + "IT RETURNS THE STRING IN LOWERCASE.".toLowerCase());

        //7. length() -> It return the length(total number of character) in the string.
        System.out.println("7. length() -> " + "It return the length(total number of character) in the string.".length());

        //8. split() -> It returns a split string matching regex.
        System.out.println("8. split() -> " + Arrays.toString("It returns a split string matching regex.".split(" ")));

        //9. trim() -> It removes the beginning and ending spaces of the string.
        System.out.println("9. trim() -> " + "   It removes the beginning and ending spaces of the string.   ".trim());

        //10. join() -> It returns a joined string.
        System.out.println("10. join() -> " + String.join(" ", "It returns a joined string.".split(" ")));

        //11. indexOf() -> It returns the specified char index value.
        System.out.println("11. indexOf() -> " + "It returns the specified char index value.".indexOf("p"));
    }
}
