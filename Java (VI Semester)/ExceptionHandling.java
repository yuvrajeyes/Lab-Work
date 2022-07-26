public class ExceptionHandling {

    public static void divide(String a, String b)  {
        try {
            System.out.println("length of a is " + a.length());
            System.out.println("length of b is " + b.length());
            int a_ = Integer.parseInt(a);
            int b_ = Integer.parseInt(b);
            System.out.println(a_ / b_);
        } catch (NullPointerException e) {
            System.out.println("NullPointerException => " + e.getMessage());
        }
        catch (ArithmeticException e) {
            System.out.println("ArithmeticException => " + e.getMessage());
        } catch (NumberFormatException e) {
            System.out.println("NumberFormatException => " + e.getMessage());
        } catch (Error e) {
            System.out.println(e.getMessage());
        }
        finally {
            System.out.println("Exiting");
        }
    }

    public static void main(String[] args) {
        divide("8","1");

    }
}
