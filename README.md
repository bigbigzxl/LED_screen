
fadeToBlackBy: 
delta=1 loop 150;
delta = 10; loop 100

//smooth fade
count = 100;
screen.drawstring((unsigned char*)"AES PQ44");
unsigned long start = millis(); // current (relative) time in msecs.

while(count--)
{
    if (count > 75)
    {
        // screen.fadeOutChar(5, 1);
        screen.fadeOutAll(1);
    }
    else
    {
        // screen.fadeOutChar(5, 5);
        screen.fadeOutAll(10);
    }
    //  screen.fadeOutChar(5, 10);
    
    
    screen.render();
    //  delay(10);
}