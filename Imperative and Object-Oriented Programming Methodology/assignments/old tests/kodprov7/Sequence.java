public interface Sequence<T>
{
    /**
     * Append e to the end of the list
     */
    public void append(T e);
    /**
     * Returns the nth element in the list (starting from 0). 
     */
    public T get(int index);
    /**
     * Append e to the front of the list
     */
    public void prepend(T e);
    /**
     * Removes and returns the nth element in the list (starting from 0). 
     */
    public T remove(int nth);
    /**
     * Return the length of the list
     */
    public int length();
}
