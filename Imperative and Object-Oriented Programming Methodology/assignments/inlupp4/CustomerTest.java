import static org.junit.Assert.assertTrue;
import org.junit.Test;


public class CustomerTest {

    Customer c = new Customer(5, 2);

    @Test
    public void testDone() {

        c.serve();
        c.serve();

        assertTrue(c.isDone());
        
        c.serve();
        assertTrue(c.isDone());
        
    }

    @Test
    public void testGetters() {
        assertTrue(c.getBornTime() == 5);
    }

    @Test
    public void testToString() {
        Customer c2 = new Customer(3, 4);
        assertTrue(c2.toString().equals("4"));
    }

    
}
