class Point {

    private double x;
    private double y;

    public Point() {
        this(0, 0);
    }

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y){
        this.y = y;
    }

    public double distance() {
        return distance(new Point(0, 0));
    }

    public double distance(double x, double y) {
        return distance(new Point(x, y));
    }

    public double distance(Point a) {
        return Math.sqrt( (a.getX()-x)*(a.getX()-x) + (a.getY()-y)*(a.getY()-y) );
    }
}

public class Constructor {

    public static void main(String[] args) {

        Point point1 = new Point();  // (0,0)
        Point point2 = new Point(6.0, 8.0);  // (6.0,8.0)

        System.out.println(point1.distance());  // 0.0
        System.out.println(point1.distance(3,4));  // 5.0

        System.out.println(point1.distance(new Point(33.525, 40.75)));  // 52.7682
        System.out.println(point1.distance(point2));  // 10.0

    }
}