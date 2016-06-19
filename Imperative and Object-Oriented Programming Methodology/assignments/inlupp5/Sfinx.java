
/**
 * Sfinx implementation of the creature interface
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

public class Sfinx implements Creature{

    /**
     * Constructor for Sfinx
     */
    public Sfinx(){

    }

    /**
     * Checks if the players hp is 180. 
     *
     * @return true if player was able to graduate else false
     */
    public boolean graduate(int hp){
        if(hp < 180){
            System.out.println("That is not enough points to graduate!");
            return false;
        }
        else{
            System.out.println("Congratz! You have graduated!");
            return true;
        }            
                
    }
    
    /**
     * {@inheritDoc}
     */
    public void talk(){
        System.out.println("It's cool to stay in school!");
    }
    
    /**
     * {@inheritDoc}
     */
    public String getName(){
        return "Sfinx";
    }

    /**
     * {@inheritDoc}
     */
    public String toString(){
        return "Sfinx";
    }
}
