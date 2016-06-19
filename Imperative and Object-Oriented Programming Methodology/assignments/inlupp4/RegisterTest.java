import static org.junit.Assert.assertTrue;
import org.junit.rules.ExpectedException;
import org.junit.Test;


public class RegisterTest {

    @Test
    public void testOpenClose() {

        Register r = new Register();

	assertTrue(!r.isOpen());
        
	r.open();
	assertTrue(r.isOpen());
        
        r.close();
        assertTrue(!r.isOpen());

    }

    @Test
    public void testHasCustomers() {

        Register r = new Register();
        Customer c1 = new Customer(1, 1);

	assertTrue(!r.hasCustomers());
        
        r.addToQueue(c1);
	assertTrue(r.hasCustomers());
        
        r.removeCurrentCustomer();
        assertTrue(!r.hasCustomers());

    }

    @Test
    public void testCurrentCustomerIsDone() {

        Register r = new Register();

        Customer c1 = new Customer(1, 1);
        Customer c2 = new Customer(2, 2);
        
	assertTrue(!r.currentCustomerIsDone());
        
        r.addToQueue(c1);
	assertTrue(!r.currentCustomerIsDone());
        
        r.step();
        assertTrue(r.currentCustomerIsDone());

    }

    @Test
    public void testAddRemoveLength() {

        Register r = new Register();

        Customer c1 = new Customer(1, 1);
        Customer c2 = new Customer(2, 2);
        

	assertTrue(r.getQueueLength() == 0);
        
        r.addToQueue(c1);
        assertTrue(r.getQueueLength() == 1);

        r.addToQueue(c2);
	assertTrue(r.getQueueLength() == 2);

	assertTrue(r.removeCurrentCustomer() == c1);
	assertTrue(r.removeCurrentCustomer() == c2);

        assertTrue(r.getQueueLength() == 0);

    }


    @Test (expected = Queue$EmptyQueueException.class)
    public void testRemoveException() {
        // Removing from an empty register/queue should throw an exception
        Register r = new Register();
        r.removeCurrentCustomer();
    }
	
	
    @Test
    public void testToString() {
	
	Register r = new Register();
	
        Customer c1 = new Customer(1, 1);
        Customer c2 = new Customer(2, 2);
	
	assertTrue(r.toString().equals(" X " + "[ ]"));
	
	r.addToQueue(c1);
	r.addToQueue(c2);
	
        assertTrue(r.toString().equals(" X " + "[1]@"));

    }
    
}


