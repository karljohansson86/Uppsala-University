/// Här skall du skriva din klass


/// Här börjar resten av koden
public class Uppgift2 {
    private static OrderedSet createOrderedSet(Integer... elements) {
	OrderedSet s = new Solution();
	for (Integer o : elements) {
	    s.add(o);
	}
	return s;
    }
    // Denna funktion kan du ignorera!
    private static boolean compare(OrderedSet s, Integer... elements) {
	if (s.size() != elements.length) return false;
	for (int i = 0; i < elements.length; ++i) {
	    if (elements[i].equals(s.get(i)) == false) return false;
	}
	return true;
    }
	
    public static void main(String args[]) {
	OrderedSet s = createOrderedSet(1, 2, 3);
	assert(compare(s, 1, 2, 3));

	s.add(5);
	assert(compare(s, 1, 2, 3, 5));

	s.del(3);
	assert(compare(s, 1, 2, 3));

	assert(s.get(0) == 1);
	assert(s.get(1) == 2);
	assert(s.get(2) == 3);

	assert(s.size() == 3);

	// Test that argument exceptions are thrown OK
	try {
	    s.del(-1);
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	try {
	    s.del(s.size());
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	try {
	    s.get(-10);
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	try {
	    s.get(s.size() + 10);
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	System.out.println("Alla tester passerar");
	System.out.println("\nObservera att O(1)-implementationen av size inte testas!");
    }

    static class Data {}
}

interface OrderedSet {
    /**
     * Adds a value to the set. Duplicates are ignored. For
     * example if a set s is {1,2,3}, s.add(2) will yield {1,2,3}
     * (the set is unchanged) and s.add(0) will yield {0,1,2,3}.
     *
     * In an ordered set, values are stored in ascending order so
     * if the set s is {0,2,4}, s.add(1) will yield {0,1,2,4},
     * etc. This means that s.get(i) <= s.get(j) when i <= j for
     * any set s, when i and j are valid indexes.
     *
     * @param o the element to be added to the set
     * @throws IllegalArgumentException if index is invalid
     */
    public void add(int o) throws IllegalArgumentException;

    /**
     * Removes an element from the set at the given index.
     * For example, if a set s is {1,2,3}, s.del(1) will
     * yield {1,3} and 2 will be returned.
     *
     * The index must be between 0 and size()-1. 
     *
     * @param index the index of the element to be removed
     * @throws IllegalArgumentException if index is invalid
     * @return the deleted element
     */
    public int del(int index) throws IllegalArgumentException;

    /**
     * Returns an the element from the set at the given
     * index. For example, if a set s is {1,2,3}, s.get(1)
     * will return 2. The set is not changed.
     *
     * The index must be between 0 and size()-1. 
     *
     * @param index the index for the element to be returned
     * @throws IllegalArgumentException if index is invalid
     * @return the element at the given index
     */
    public int get(int index) throws IllegalArgumentException;

    /**
     * Returns the size of the OrderedSet calculated in O(1) time.
     *
     * @return the number of elements in the set
     */
    public int size(); 
}

