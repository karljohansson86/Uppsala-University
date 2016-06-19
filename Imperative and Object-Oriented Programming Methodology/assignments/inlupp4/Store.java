import java.util.Random;
import java.util.ArrayList;

/**
 * Representation of a store 
 *
 * @author Marcus Andersson
 * @author Karl Johansson
 */

public class Store {


    /**
     * Class for handling exception if program tries to create a store without registers
     */
    public static class NoRegisterException extends RuntimeException{};

    /**
     * Registers in store
     */
    private Register[] register;
    
    /**
     * Number of registers in store
     */
    private int registers;
    

    /**
     * Constructor of Store
     * 
     * @param  registers    Number of registers in store
     * @throws NoRegisterException (runtime) if trying to create a store with 0 registers
     */
    public Store(int registers) {
 	if (registers < 1) { throw new NoRegisterException(); }

        this.registers = registers;
        
	this.register = new Register[registers];
	for (int i = 0; i < registers; i++) {
	    this.register[i] = new Register();
	}

    }

    /**
     * Opens a random amount of a stores registers
     *
     */
    public void openStore() {
        Random random = new Random();
        int randomNum = random.nextInt(this.registers-1) +1;

        while (randomNum > 0) {
            this.openNewRegister();
            randomNum = randomNum - 1;
        }
    }
    
    /**
     * Opens a specific amount of a stores registers
     *
     * @param amountOfRegToOpen amount of a stores registers to open
     * @throws NoRegisterException (runtime) if trying to open zero registers or more than the store has
     */
    public void openStore(int amountOfRegToOpen) {
	
	if (amountOfRegToOpen < 1 || amountOfRegToOpen > this.registers) 
	    { throw new NoRegisterException(); }
	
        while (amountOfRegToOpen > 0) {
            this.openNewRegister();
            amountOfRegToOpen = amountOfRegToOpen - 1;
        }
    }

    /**
     * Steps the simulation forward one step for a store, 
     * during each step each of the stores registers steps forward in the simulation
     *
     */
    public void step() {
        
	for (Register r : this.register) {
	    if (r.isOpen()) {
		r.step();
	    }
	}
    }

    /**
     * Adds a customer to the stores register with the shortest queue
     *
     * @param c customer to added to the queue
     */
    public void newCustomer(Customer c) {

	int length = -1;
	Register shortest = null;
        
	for (Register r : this.register) {
	    int currentLen = r.getQueueLength();
            if (r.isOpen()) {
                if (length == -1 || length > currentLen) {
                    length = currentLen;
                    shortest = r;
                }
            }
	}
	shortest.addToQueue(c);
    }


    /**
     * Removes the customers that are done from each of the stores registers queue 
     *
     * @return Queue containing the customers that are done
     */
    /*
    public Queue<Customer> getDoneCustomers() {

	Queue<Customer> doneCustomers = new LinkedQueue<Customer>();
        
	for (Register r : this.register) {
	    if (r.isOpen() && r.currentCustomerIsDone()) {
                Customer c = r.removeCurrentCustomer();
                doneCustomers.enqueue(c);

                if (r.getQueueLength() == 0) { r.close(); }
	    }
	}
	return doneCustomers;
    }
    */
    public ArrayList<Customer> getDoneCustomers() {

        ArrayList<Customer> doneCustomers = new ArrayList<Customer>();

        for (Register r: this.register) {
             if (r.isOpen() && r.currentCustomerIsDone()) {
                Customer c = r.removeCurrentCustomer();
                doneCustomers.add(c);

                if (r.getQueueLength() == 0) { r.close(); }
	    }
        }
        return doneCustomers;
    }
    
    /**
     * Get the average queue length in the store 
     *
     * @return Queue containing the customers that are done
     */
    public int getAverageQueueLength() {
        
	int totLength = 0;
	int totRegisters = 0;
        
	for (Register r : this.register) {
	    if (r.isOpen()) {
		totLength = totLength + r.getQueueLength();
		totRegisters = totRegisters + 1;
	    }
	}
        
	return (totRegisters == 0) ? 0 : totLength / totRegisters;	
    }

    /**
     *  Store should always have one register open
     *
     */    
    public void keepOneRegisterOpen() {

        boolean noRegistersOpen = true;
        for (Register r : this.register) {
	    if (r.isOpen()) {
                noRegistersOpen = false;
                break;
            }
	}

        if (noRegistersOpen) { this.register[0].open(); }
        
    }

    /**
     *  Open a new register in the store
     *
     */
    public void openNewRegister() {
	for (Register r : this.register) {
	    if (r.isOpen() == false) {
		r.open();
		return;
	    }
	}	
    }

    /**
     * Get a stringrepresentation of a store
     *
     * @return String representing a store
     */
    public String toString() {
        String store = "";
        for (Register r : this.register) {
            store = store + r + "\n\n";
	}

        return store;
    }
    

}
