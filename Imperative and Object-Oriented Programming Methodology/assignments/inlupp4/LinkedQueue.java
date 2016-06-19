
/**
 * Linked implementation of the Queue interface
 *
 * @author Marcus Andersson
 * @author Karl Johansson
 */
public class LinkedQueue<T> implements Queue<T> {

    private Node first = null;
    private Node last  = null;
    private int length = 0;
    
    /**
     * {@inheritDoc}
     */
    @Override
    public int length() {
	return this.length;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void enqueue(T data) {

	if (data == null) { throw new NullPointerException("Cannot enqueue null"); }
        
	Node n = new Node(data);
	
	if (this.first == null) {
            this.first = n;            
	} else {
	    this.last.setNext(n);
	}

        this.last = n;
	this.length = this.length + 1;

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T dequeue() {
        
	if (this.first == null) { throw new EmptyQueueException(); }
        
	T data = this.first.getData();
	this.first = this.first.getNext();
        
	if (this.first == null) {
	    this.last = null;
	}
        
	this.length = this.length - 1;
	return data;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T first() {
	return (this.first == null) ? null : this.first.getData();
    }

    
    private class Node {
        
	private T data; 
	private Node next = null;

	public Node(T data) {
	    this.data = data;
	}
    
	public T getData() {
	    return this.data;
	}
        
	public Node getNext() {
	    return this.next;
	}
	public void setNext(Node n) {
	    this.next = n;
	}
    }

    
    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        int i = this.length();
        String queue = "";
        Node n = this.first;
        while (i > 1) {
            queue = queue + n.getNext().getData().toString();//"@";
            n = n.getNext();
            i = i - 1;
        }

        return queue;
    }

}
