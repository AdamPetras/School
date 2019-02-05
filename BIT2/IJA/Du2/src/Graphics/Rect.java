package Graphics;

public class Rect extends Point2D
{
    private int _sizeX;
    private int _sizeY;
    private int _xMin;
    private int _xMax;
    private int _yMin;
    private int _yMax;
    private Point2D _center;
    //region Getters

    public int XMin() {
        return _xMin;
    }

    public int XMax() {
        return _xMax;
    }

    public int YMin() {
        return _yMin;
    }

    public int YMax() {
        return _yMax;
    }

    public Point2D Position()
    {
        return new Point2D(X,Y);
    }

    public Point2D Size()
    {
        return new Point2D(_sizeX,_sizeY);
    }

    public Point2D Center()
    {
        return _center;
    }

    public int Width()
    {
        return _sizeX;
    }

    public int Height()
    {
        return _sizeY;
    }
    //endregion


    public Rect(Point2D position, Point2D size)
    {
        X = position.X;
        Y = position.Y;
        _sizeX = size.X;
        _sizeY = size.Y;
        CalcInternalVariables();
    }

    public Rect(int positionX,int positionY, Point2D size)
    {
        X = positionX;
        Y = positionY;
        _sizeX = size.X;
        _sizeY = size.Y;
        CalcInternalVariables();
    }

    public Rect(Point2D position, int sizeX,int sizeY)
    {
        X = position.X;
        Y = position.Y;
        _sizeX = sizeX;
        _sizeY = sizeY;
        CalcInternalVariables();
    }

    public Rect(int positionX,int positionY, int sizeX,int sizeY)
    {
        X = positionX;
        Y = positionY;
        _sizeX = sizeX;
        _sizeY = sizeY;
        CalcInternalVariables();
    }

    private void CalcInternalVariables()
    {
        _xMin = X;
        _xMax = X+_sizeX;
        _yMin = Y;
        _yMax = Y+_sizeY;
        _center = new Point2D(X+(_sizeX/2),Y+(_sizeY/2));
    }

    public boolean Contains(Point2D point)
    {
        return point.X >=  X && point.X <= _xMax && point.Y >= Y && point.Y <= _yMax;
    }

    public boolean Intersect(Rect rect)
    {
        int xmin = Math.max(X, rect.X);
        int xmax = Math.min(_xMax, rect._xMax);
        if (xmax >= xmin) {
            int ymin = Math.max(Y,rect.Y);
            int ymax = Math.min(_yMax, rect._yMax);
            if (ymax >= ymin) {
                return true;
            }
        }
        return false;
    }

    @Override
    public String toString() {
        return "Rect: X = "+X+" Y = "+Y+" Width = "+_sizeX+" Height = "+_sizeY;
    }
}
