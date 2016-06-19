import static org.junit.Assert.assertTrue;
import org.junit.Test;


public class QueueTest {

    @Test
    public void testQueue() {

        Queue<Customer> q = new LinkedQueue<Customer>();

        Customer c1 = new Customer(1, 1);
        Customer c2 = new Customer(2, 2);
                    
        assertTrue(q.length() == 0);

        q.enqueue(c1);
        q.enqueue(c2);

        assertTrue(q.length() == 2);
        assertTrue(q.first() == c1);
        assertTrue(q.dequeue() == c1);
        assertTrue(q.first() == c2);
        assertTrue(q.dequeue() == c2);
        assertTrue(q.first() == null);
        
    }

    @Test
    public void testQueueToString() {
        
        Queue<Customer> q1 = new LinkedQueue<Customer>();
        Queue<Customer> q2 = new LinkedQueue<Customer>();

        Customer c1 = new Customer(1, 1);

        q1.enqueue(c1);
        q1.enqueue(c1);
        q1.enqueue(c1);

        assertTrue(q1.toString().equals("@@"));
        assertTrue(q2.toString().equals(""));

    }

}
