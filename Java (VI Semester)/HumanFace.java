/*Java Program to Draw a Human Face using Applet*/
import java.applet.*;
import java.awt.*;
public class HumanFace extends Applet
{ 
    //Initialize the applet
    public void init()
    {
	setBackground(Color.white);
    }
    //Draw the human face
    public void paint(Graphics g) 
    { 
        //Change color to cream
        Color clr=new Color(255,179,86);
        g.setColor(clr);
        //Draw and fill the face
        g.drawOval(100,100,250,300);
        g.fillOval(100,100,250,300);
        //Change color to black
        g.setColor(Color.black);
        //Draw the left eye
        g.drawOval(160,185,40,25);
        g.fillOval(160,185,40,25);
        //Draw the right eye
        g.drawOval(250,185,40,25);
        g.fillOval(250,185,40,25);
        //Draw the Left Eyebrow
        g.drawArc(160,170,35,10,0,180);
        //Draw the Right Eyebrow
        g.drawArc(250,170,35,10,0,180);
        //Draw the Nose
        g.drawLine(210,265,210,275);
        g.drawLine(240,265,240,275);
        g.drawArc(210,275,30,10,0,-180); 
       //Draw the smile
        g.drawArc(175,300,100,50,0,-180);
    } 
} 

/* 
<applet code="HumanFace.class" width="300" height="300"> 
</applet> 
*/  