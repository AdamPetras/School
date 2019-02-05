package Graphics;

public class LineMesh
{
    private Rect _first;
    private Rect _middle;
    private Rect _last;
    public LineMesh(Point2D startPoint, Point2D finalPoint)
    {
        CalcRects(startPoint,finalPoint);
    }

    private void CalcRects(Point2D startPoint, Point2D finalPoint)
    {
        int middleX = (startPoint.X + finalPoint.X) /2;
        _first = new Rect(startPoint.X,startPoint.Y+1,middleX,3);
        _middle = new Rect(middleX-1,startPoint.Y,3,finalPoint.Y);
        _last = new Rect(middleX,finalPoint.Y+1,middleX,3);
    }
}
