
/**
 * Abstract class of a item
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

public abstract class Item {
    
    /**
     * Size of the item
     */
    private int size = 0;

    /**
     * Get the size of the item
     *
     * @return the size of the item
     */
    public int getSize(){
        return this.size;
    }
      
    /**
     * Creates a string representation of the item
     *
     * @return String representing the item
     */
    public String toString() {
        return "";
    }

}
