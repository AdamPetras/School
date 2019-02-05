package Base;

import Graphics.DrawableObject;
import Graphics.Rect;

public class Port implements DrawableObject {

    public Rect Rect;
    private Block _block;
    private Link _link;
    public boolean IsValueSet;
    private double _value;

    public Link GetLink() {
        return _link;
    }

    public void AddLink(Link link) {
        this._link = link;
    }

    public double GetValue()
    {
        return _value;
    }

    public void SetValue(double _value)
    {
        this._value = _value;
    }

    public Block GetBlock()
    {
        return _block;
    }

    public Port(Rect rect, Block block)
    {
        Rect = rect;
        _block = block;
        IsValueSet = false;
    }


    @Override
    public void Draw()
    {

    }
}
