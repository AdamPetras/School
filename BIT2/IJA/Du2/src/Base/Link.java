package Base;

import Graphics.DrawableObject;
import Graphics.LineMesh;

public class Link implements DrawableObject
{
    private Port _input;
    private Port _output;
    private double _value;
    private LineMesh mesh;

    public void SetValue(double value) {
        _value = value;
    }

    public double GetValue() {
        return _value;
    }

    public void SetInput(Port input) {
        this._input = input;
        _value = _input.GetValue();
        _input.AddLink(this);
    }

    public void SetOutput(Port output) {
        this._output = output;
        _output.AddLink(this);
        _output.SetValue(_value);
        _output.IsValueSet = true;
    }

    public Port GetOutput() {
        return _output;
    }

    public Link(Port input, Port output)
    {
        SetInput(input);
        SetOutput(output);
        mesh = new LineMesh(input.Rect.Center(),output.Rect.Center());
    }


    @Override
    public void Draw()
    {

    }
}
