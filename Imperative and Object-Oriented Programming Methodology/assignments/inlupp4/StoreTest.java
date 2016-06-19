import static org.junit.Assert.assertTrue;
import org.junit.rules.ExpectedException;
import org.junit.Test;


public class StoreTest {


    @Test (expected = Store$NoRegisterException.class)
    public void testOpenNoRegisterException() {
        Store s = new Store(4); 

    	// Opening zero registers should throw an exception
    	s.openStore(0);	
    }
    

    @Test
    public void testOpenStore() {
        Store s = new Store(1);
        String emptyStore = "   [ ]\n\n";
	
    	s.openStore(1);
	assertTrue(s.toString().equals(emptyStore));

        Store s2 = new Store(2); 
	
    	s2.openStore(2);
	assertTrue(s2.toString().equals(emptyStore + emptyStore));
    }

    
    @Test
    public void testNewCustomerAverageQueueLength() {
        Store s = new Store(1);
        Customer c = new Customer(1, 1);

        s.openStore(1);

        assertTrue(s.getAverageQueueLength() == 0);

        s.newCustomer(c);
        s.newCustomer(c);

        assertTrue(s.getAverageQueueLength() == 2);

        Store s2 = new Store(2);

        s2.openStore(2);

        assertTrue(s2.getAverageQueueLength() == 0);

        s2.newCustomer(c);
        s2.newCustomer(c);
        s2.newCustomer(c);

        assertTrue(s2.getAverageQueueLength() == 1);

        s2.newCustomer(c);

        assertTrue(s2.getAverageQueueLength() == 2);

    }

    
    @Test
    public void testGetDoneCustomers() {
        
        Store s = new Store(1);
        Customer c = new Customer(1, 1);

        s.openStore(1);
        Queue<Customer> served = s.getDoneCustomers();
        assertTrue(served.length() == 0);

        s.newCustomer(c);
        served = s.getDoneCustomers();
        assertTrue(served.length() == 0);

        s.step();
        served = s.getDoneCustomers();
        assertTrue(served.length() == 1);

        
        Store s2 = new Store(2);
        Customer c1 = new Customer(1, 1);
        Customer c2 = new Customer(1, 1);
        Customer c3 = new Customer(1, 1);

        s2.openStore(2);
        served = s2.getDoneCustomers();
        assertTrue(served.length() == 0);

        s2.newCustomer(c1);
        s2.newCustomer(c2);
        s2.newCustomer(c3);

        served = s2.getDoneCustomers();
        assertTrue(served.length() == 0);

        s2.step();
        served = s2.getDoneCustomers();
        assertTrue(served.length() == 2);

        s2.step();
        served = s2.getDoneCustomers();
        assertTrue(served.length() == 1);

    }


    @Test
    public void testKeepOneRegisterOpen() {
        
        Store s = new Store(2);
        String emptyOpenStore = "   [ ]\n\n";
        String emptyClosedStore = " X [ ]\n\n";

	assertTrue(s.toString().equals(emptyClosedStore + emptyClosedStore));

        s.keepOneRegisterOpen();
        assertTrue(s.toString().equals(emptyOpenStore + emptyClosedStore));

        s.keepOneRegisterOpen();
        assertTrue(s.toString().equals(emptyOpenStore + emptyClosedStore));

    }


    @Test
    public void testOpenNewRegister() {
        
        Store s = new Store(2);
        String emptyOpenStore = "   [ ]\n\n";
        String emptyClosedStore = " X [ ]\n\n";
	
	assertTrue(s.toString().equals(emptyClosedStore + emptyClosedStore));
        
        s.openNewRegister();
        assertTrue(s.toString().equals(emptyOpenStore + emptyClosedStore));

        s.openNewRegister();
        assertTrue(s.toString().equals(emptyOpenStore + emptyOpenStore));

    }
    
}
