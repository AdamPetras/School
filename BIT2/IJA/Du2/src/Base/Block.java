package Base;

import Graphics.DrawableObject;
import Graphics.Rect;

import java.util.ArrayList;
import java.util.List;



interface MyCallBack
{
    double MathFunction(double a, double b);
}

public class Block implements DrawableObject
{
    private EBlock _eBlock;
    private List<Port> _inputPortList;
    private Port _output;
    private int _numOfInput;
    private Rect _rect;

    public Port Output()
    {
        return _output;
    }

    public Port GetInputPort(int index)
    {
        return _inputPortList.get(index);
    }
    public Port GetEmptyInputPort()
    {
        for (Port port :_inputPortList) {
            if(!port.IsValueSet)
                return port;
        }
        return null;
    }

    public Block(EBlock eBlock, Rect rect, int numOfInput)
    {
        if(numOfInput < 2)
            System.out.println("ERROR");
        _eBlock = eBlock;
        _rect = rect;
        _numOfInput = numOfInput;
        _inputPortList = new ArrayList<>();
        AddInputPort();
    }

    public Block(EBlock eBlock, Rect rect, int... inputValues)
    {
        if(inputValues.length<2)
            System.out.println("ERROR");
        _eBlock = eBlock;
        _rect = rect;
        _inputPortList = new ArrayList<>();
        _numOfInput = inputValues.length;
        AddInputPort();
        for (int i = 0; i<_numOfInput;i++)
        {
            _inputPortList.get(i).SetValue(inputValues[i]);
            _inputPortList.get(i).IsValueSet = true;
        }
    }

    public Block(EBlock eBlock,Rect rect)
    {
        _eBlock = eBlock;
        _rect = rect;
        _numOfInput = 2;
        _inputPortList = new ArrayList<>();
        AddInputPort();
    }

    public void EditPortValue(int index, double value)
    {
        _inputPortList.get(index).SetValue(value);
        _inputPortList.get(index).IsValueSet = true;
    }

    public void SetPortValue(double value)
    {
        Port tmp = GetEmptyInputPort();
        tmp.SetValue(value);
        tmp.IsValueSet = true;
    }

    private void AddInputPort()
    {
        for (int i = 0;i< _numOfInput;i++)
        {
            _inputPortList.add(new Port(new Rect(_rect.X + 5,_rect.Y+10+i*30,5,5),this));
        }
        _output = new Port(new Rect(_rect.XMax() - 5,_rect.Y + 10,5,5),this);
    }

    public double Calculate()
    {
        if(_inputPortList.size() < 2)
        {
            System.out.println("ERROR!!");
        }
        else
        {
            if (_eBlock == EBlock.SUM)
                ForEach((a, b) -> a + b);
            else if (_eBlock == EBlock.SUB)
                ForEach((a, b) -> a - b);
            else if (_eBlock == EBlock.MUL)
                ForEach((a, b) -> a * b);
            else if (_eBlock == EBlock.DIV)
                ForEach((a, b) -> a / b);
        }
        return 0;
    }

    private void ForEach(MyCallBack callback)
    {
        double a = callback.MathFunction(_inputPortList.get(0).GetValue(), _inputPortList.get(1).GetValue());
        for (int i =2;i<_inputPortList.size();i++)
        {
            a = callback.MathFunction(a, _inputPortList.get(i).GetValue());
        }
        _output.SetValue(a);
        if(_output.GetLink() != null)
        {
            _output.GetLink().SetValue(a);
            _output.GetLink().GetOutput().SetValue(a);
        }
    }

    @Override
    public void Draw()
    {

    }
}
