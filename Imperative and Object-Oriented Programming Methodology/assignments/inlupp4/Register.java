
/**
 * Representation of a register 
 *
 * @author Marcus Andersson
 * @author Karl Johansson
 */

public class Register{

    /**
     * If a register is open or closed
     */
    private boolean open = false;

    /**
     * Queue containing customers
     */
    private Queue<Customer> queue;

    /**
     * Constructor of register
     * Sets register to closed and creates a new empty queue
     *  
     */
    public Register() {
        this.queue = new LinkedQueue<Customer>();
    }

    /**
     * Steps the simulation forward one step for a register, 
     * during each step the register serves the first customer in the queue if any
     *
     */
    public void step() {
        Customer c = this.queue.first();
	if (c != null) { c.serve(); }
    }

    /**
     * Open register
     *
     */
    public void open() {
	this.open = true;
    }

    /**
     * Close register
     *
     */
    public void close() {
	this.open = false;
    }
    
    /**
     * Check if register is open
     *
     * @return true if register is open otherwise false
     */
    public boolean isOpen() {
	return this.open;
    }

    /**
     * Check if register has customers
     *
     * @return true if register has customers otherwise false
     */
    public boolean hasCustomers() {
	return this.queue.length() > 0;
    }

    /**
     * Check if first customer in the registers queue is done
     *
     * @return true if customer is done otherwise false
     */
    public boolean currentCustomerIsDone() {
        Customer c = this.queue.first();
	return (c != null) ? c.isDone() : false;
    }
    
    /**
     * Removes the first customer in the registers queue
     *
     * @return Customer that was first in the queue
     */
    public Customer removeCurrentCustomer() {
	return this.queue.dequeue();
    }

    /**
     * Adds a customer to the registers queue
     *
     * @param c Customer to be added to the registers queue
     */
    public void addToQueue(Customer c) {
	this.queue.enqueue(c);
    }

    /**
     * Get registers queue length
     *
     * @return int representing the registers queue length
     */
    public int getQueueLength(){
	return this.queue.length();
    }

    /**
     * Get a stringrepresentation of a register
     *
     * @return String representing a register
     */
    public String toString() {
        Customer c = this.queue.first();

        String reg = (this.isOpen()) ? "   " : " X ";
        String customer = (c == null) ? " " : "" + c.getGroceries();
        
        return reg + "[" + customer + "] " + this.queue;
    }
    
}
