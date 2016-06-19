
/**
 * Representation of a book, extends the Item class
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

public class Book extends Item {
    
    /**
     * Name of the book
     */
    private String name = null;
    
    /**
     * Author of the book
     */
    private String author = null;
    
    /**
     * Size of the book
     */
    private int size = 0;

    /**
     * Constructor of the book
     * Sets the name and author to null and the size to 0 
     */
    public Book(){
    }

    /**
     * Constructor of the book
     * 
     * @param name of the book
     */
    public Book(String name) {
        this.name = name;
    }
    
    /**
     * Set the size of the book
     */
    public void setSize(int size) {
        this.size = size;
    }

    /**
     * Get the size of the book
     *
     * @return the size of the book
     */
    public int getSize(){
        return this.size;
    }

    /**
     * Set the author of the book
     */
    public void setAuthor(String author){
        this.author = author;
    }

    /**
     * Set name of the book
     */
    public void setName(String name) {
        this.name = name;
    }
    
    /**
     * Get name of the book
     */
    public String getBookName() {
        return this.name;
    }

    /**
     * Creates a string representation of the book
     *
     * @return String representing the book
     */
    public String toString() {
        return this.name;
    }
    
    // public String toString() {
    //     StringBuilder sb = new StringBuilder();
    //     sb.append(this.name + ", " + this.author + ", " + this.size);
    //   // sb.append(". ");
    //   // sb.append("\n");
    //     return sb.toString();
    // }
}



