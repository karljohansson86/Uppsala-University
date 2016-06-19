
/**
 * Representation of a key, extends the Item class
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

public class Key extends Item {
    
    /**
     * Size of the key
     */
    private int size = 1;
    
    /**
     * Constructor of the key
     * Sets the size of the key 
     */
    public Key() {
        this.size = 1;
    }   

    public int getSize(){
        return this.size;
    }

    /**
     * Creates a string representation of the key
     *
     * @return String representing the key
     */
    public String toString() {
        return "a key";
    }    
}
