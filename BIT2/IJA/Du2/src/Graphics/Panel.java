package Graphics;

import Base.Block;
import Base.EBlock;
import Base.Link;

public class Panel
{
    public static void main(String [] args)
    {
        Block block = new Block(EBlock.SUM,new Rect(10,10,10,10),10,20,30,40);
        Block block2 = new Block(EBlock.SUM,new Rect(10,10,10,10));
        Block bl3 = new Block(EBlock.MUL,new Rect(0,0,0,0),3);
        new Link(block.Output(),block2.GetEmptyInputPort());
        new Link(block2.Output(),bl3.GetEmptyInputPort());
        bl3.SetPortValue(3);
        bl3.SetPortValue(2);
        block2.SetPortValue(5);
        block.Calculate();
        block2.Calculate();
        bl3.Calculate();
        System.out.println(block.Output().GetValue());
        System.out.println(block2.Output().GetValue());
        System.out.println(bl3.Output().GetValue());
    }
    private static void CreateLink(Block firstBlock, Block secondBlock)
    {
        Link link = new Link(firstBlock.Output(),secondBlock.GetEmptyInputPort());
    }
}
