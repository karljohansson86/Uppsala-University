
/**
 * Interface for a creature
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */


public interface Creature
{

    /**
     * Interact with the creature // bör vara ngot annat
     *
     */
    public void talk();
    

    /**
     * Gets the creatures name
     *
     * @return The creatures name
     */
    public String getName();

    
    /**
     * Creates a string representation of the Creature
     *
     * @return String representing the Creature
     */
    public String toString();
}
