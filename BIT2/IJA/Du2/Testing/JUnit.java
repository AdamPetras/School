package Testing;

import Graphics.Point2D;
import Graphics.Rect;
import org.junit.Before;
import org.junit.Test;

import static junit.framework.TestCase.assertEquals;

public class JUnit
{
    public Rect myRect;

    @Before
    public void prepare() {
        myRect = new Rect(10,10,10,10);
    }

    @Test
    public void PointTest()
    {
        Point2D point = new Point2D(5,5);
        assertEquals("Does point X = 5",5,point.X);
        assertEquals("Does point Y = 5",5,point.Y);
        assertEquals("Does point distance equal to 4",4f,(float)Point2D.Distance(point,new Point2D(5,9)));
        assertEquals("Does point distance equal to 4",4f,(float)Point2D.Distance(point,new Point2D(9,5)));
        assertEquals("Does point distance equal to sqrt(2)", Math.sqrt(8),Point2D.Distance(point,new Point2D(7,7)));
        assertEquals("Does point distance equal to sqrt(50)", Math.sqrt(50),Point2D.Distance(point,new Point2D(0,0)));
    }

    @Test
    public void RectContains()
    {
        assertEquals("Does Rect contains point X5 Y5.",false,myRect.Contains(new Point2D(5,5)));
        assertEquals("Does Rect contains point X5 Y10.",false,myRect.Contains(new Point2D(5,10)));
        assertEquals("Does Rect contains point X10 Y10.",true,myRect.Contains(new Point2D(10,10)));
        assertEquals("Does Rect contains point X20 Y10.",true,myRect.Contains(new Point2D(20,10)));
        assertEquals("Does Rect contains point X25 Y10.",false,myRect.Contains(new Point2D(25,10)));
        assertEquals("Does Rect contains point X20 Y20.",true,myRect.Contains(new Point2D(20,20)));
        assertEquals("Does Rect contains point X25 Y25.",false,myRect.Contains(new Point2D(20,25)));
        assertEquals("Does Rect contains point X0 Y0.",false,myRect.Contains(new Point2D(0,0)));
    }
    @Test
    public void RectContainsAdvanced()
    {
        assertEquals("Does Rect contains point XfloatMAX Y10.",false,myRect.Contains(new Point2D(Integer.MAX_VALUE,10)));
        assertEquals("Does Rect contains point X15 YfloatMAX.",false,myRect.Contains(new Point2D(15,Integer.MAX_VALUE)));
        assertEquals("Does Rect contains point XfloatMax YfloatMax.",false,myRect.Contains(new Point2D(Integer.MAX_VALUE,Integer.MAX_VALUE)));
        assertEquals("Does Rect contains point XfloatMin YfloatMin.",false,myRect.Contains(new Point2D(Integer.MIN_VALUE,Integer.MIN_VALUE)));
    }
    @Test
    public void RectIntersects()
    {
        assertEquals("Does Rect intersect Rect X0 Y0 W5 H5.",false,myRect.Intersect(new Rect(0,0,5,5)));
        assertEquals("Does Rect intersect Rect X0 Y0 W10 H10.",true,myRect.Intersect(new Rect(0,0,10,10)));
        assertEquals("Does Rect intersect Rect X20 Y20 W10 H10.",true,myRect.Intersect(new Rect(20,20,10,10)));
        assertEquals("Does Rect intersect Rect X21 Y21 W10 H10.",false,myRect.Intersect(new Rect(21,21,10,10)));
    }
    @Test
    public void RectIntersectsAdvanced()
    {
        assertEquals("Does Rect intersect Rect X0 Y0 W50 H50.",true,myRect.Intersect(new Rect(0,0,50,50)));
        assertEquals("Does Rect intersect Rect X0 Y0 WfloatMax HfloatMax.",true,myRect.Intersect(new Rect(0,0,Integer.MAX_VALUE,Integer.MAX_VALUE)));
        assertEquals("Does Rect intersect Rect X20 Y20 WfloatMax HfloatMax.",true,myRect.Intersect(new Rect(20,20,5000,5000)));
        assertEquals("Does Rect intersect Rect XfloatMin YfloatMin WfloatMax HfloatMax.",false,myRect.Intersect(new Rect(Integer.MIN_VALUE,Integer.MIN_VALUE,Integer.MAX_VALUE,Integer.MIN_VALUE)));
    }
}
