package Base;

public class MyExceptions extends Throwable {
    public static class PortsOutOfBoundaries extends Exception
    {
        public PortsOutOfBoundaries(String msg)
        {
            super(msg);
        }
    }
}
