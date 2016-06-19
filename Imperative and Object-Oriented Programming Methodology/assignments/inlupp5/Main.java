
public class Main
{
    
    public static void main(String args[])
    {        
        CreateWorld c = new CreateWorld();
        Player p = createPlayer();
        Game g = new Game(c.getWorld(), p);
        g.startGame();
        
    }

    public static Player createPlayer()
    {
        //Ask for name etc..
        return new Player();
    }

        

}
