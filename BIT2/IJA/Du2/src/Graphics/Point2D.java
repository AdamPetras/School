package Graphics;

public class Point2D
{
    public int X;
    public int Y;

    public Point2D(int x,int y)
    {
        X = x;
        Y = y;
    }

    public Point2D()
    {

    }

    public static double Distance(Point2D first, Point2D second)
    {
        return Math.sqrt(Math.pow(first.X -second.X,2)+Math.pow(first.Y -second.Y,2));
    }

    @Override
    public String toString() {
        return "Point: X = "+ X + " Y = "+ Y;
    }
}
