    
/**
 * Interface for a generic queue
 *
 * @author Marcus Andersson
 * @author Karl Johansson
 */
public interface Queue<T> {

    /**
     * Thrown when the user attempts to dequeue from an empty queue
     */
    public static class EmptyQueueException extends RuntimeException{};
    
    /**
     * Inserts data at the end of the queue
     *
     * @param data the element or data to be insert into the queue
     * @throws NullPointerException (runtime) if the data to be inserted is null
     */
    public void enqueue(T data);

    /**
     * Removes the first element from the queue and returns it
     *
     * @return The element that was first in the queue
     * @throws EmptyQueueException (runtime) if the queue is empty
     */
    public T dequeue();
    
    /**
     * Gets the first element T in the queue
     *
     * @return The first element in the queue if any, otherwise null
     */
    public T first();

    /**
     * Calculates the length of the queue
     *
     * @return Integer representing length of the queue
     */
    public int length();

    /**
     * Creates a string representation of the queue
     *
     * @return String representing the queue
     */
    public String toString();
    
}
