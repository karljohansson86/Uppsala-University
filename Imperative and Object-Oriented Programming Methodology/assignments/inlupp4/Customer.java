
/**
 * Representation of a customer with an amount of groceries and the time the customer was created
 *
 * @author Marcus Andersson
 * @author Karl Johansson
 */
public abstract class Customer {

    /**
     * Time customer created
     */
    private final int bornTime;

    /**
     * Amount of groceries
     */
    private int groceries;


    /**
     * Constructor for customer
     *
     * @param startTime Time when customer is created  
     * @param amountOfGroceries Amount of groceries customer wants to purchase
     * @throws  IllegalArgumentException (runtime) if any of the parameters are negative 
     */
    public Customer(int startTime, int amountOfGroceries) {
	if (startTime < 0 || amountOfGroceries < 0) {
            throw new IllegalArgumentException("Neither argument may be negative ");
        }
        this.bornTime = startTime;
        this.groceries = amountOfGroceries;
    }
    

    /**
     * Removes one of the customers groceries
     *
     */
    public void serve() {

        if (this.groceries < 1) { return; }
        this.groceries = this.groceries - 1;
        
    };
    

    /**
     * Checks if a customer is done, has zero groceries
     *
     * @return True if customer is done otherwise false
     */
    public boolean isDone() {

        return (this.groceries == 0);
    };


    /**
     * Get a customers borntime
     *
     * @return int representing a customers borntime
     */
    public int getBornTime() {
	return this.bornTime;
    }


    /**
     * Get a customers remaining groceries
     *
     * @return int representing a customers remaining groceries
     */
    public int getGroceries() {
	return this.groceries;
    }


    /**
     * Set a customers groceries
     * ** Customers can have 0 groceries **
     * @throws IllegalArgumentException (runtime) if amount of groceries is negative
     * @return int representing a customers remaining groceries
     */
    protected void setGroceries(int groceries) {
	if (groceries < 0) { throw new IllegalArgumentException
		("Customer cannot have negative amount of groceries"); }
	this.groceries = groceries;
    }
    

    /**
     * Get a string representation of a customer
     */
    public abstract String toString();

}


    
