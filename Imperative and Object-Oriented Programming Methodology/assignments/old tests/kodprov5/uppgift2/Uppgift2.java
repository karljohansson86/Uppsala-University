/// Här skall du skriva din klass

class Solution implements Sequence {
    Node first = null; 
    Node last = null;
    int size = 0;

    public Solution() {        
    }

    private class Node {
        Object element;
        Node next;
    
        public Node(Object o) {
            this.element = o;
            this.next = null;
        }

        public Object getElement() {
            return this.element;
        }
        
        public Node getNext() {
            return this.next;
        }
        
    }


    public void addFront(Object o){
        if(o == null) { return;}

        Node add = new Node(o);
        
        if(this.size == 0) {
            this.first = add;
            this.last = add;
            this.size++;
            return;
        }  

        if(this.size == 1) {
            add.next = this.last;
            this.first = add;
            this.size++;
            return;
        } 
        
        add.next = this.first;
        this.first = add; 
        this.size++;
    }

    public void addBack(Object o){
        if(o == null) { return;}

        Node add = new Node(o);
        
        if(this.size == 0) {
            System.out.println("HEJ");
            this.first = add;
            this.last = add;
            this.size++;
            return;
        } 

        if(this.size == 1) {
            System.out.println("HEJ");
            this.first.next = add;
            this.last = add;
            this.size++;
            return;
        }
        
        
        System.out.println("HEJ");
        
        this.last.next = add;
        this.last = add;
        this.size++;
    }

    public void insert(Object o, int index) throws IllegalArgumentException{
        
        int size = this.size();
        
        if(index > size || index < 0) {
            throw new IllegalArgumentException();
        }
        
        if(o == null) { 
            return;
        }
        
        this.size++;
        if(index == 0) {
            addFront(o);
            return;
        }

        if(index == size) {
            addBack(o);
            return;
        }

        Node add = new Node(o);
        Node current = this.first;
        
        
        for(int i = 0; i<size; i++) {
            if(i == index-1) {
                add.next = current.next;
                current.next = add;
                return;
            }
            current = current.next;
        }

    }

    public Object del(int index) throws IllegalArgumentException {
        int size = this.size();
        
        if(index > size-1 || index < 0) {
            throw new IllegalArgumentException();
        }
        
        if(index == 0) {
            Node del = this.first;
            this.first = del.next;
            this.size--;
            return del.element;
        }


        Node current = this.first;
        Node del = null;
        
        for(int i = 0; i<size; i++) {
            if(i == index-1) {
                del = current.next;
                current.next = del.next;
                
            }
            current = current.next;
        }
        this.size--;
        return del.element;
    }

    public Object get(int index) throws IllegalArgumentException {
        int size = this.size();
        System.out.println("Size:"+ size);
        System.out.println("Index:"+ index);
        if(index > size-1 || index < 0) {
            throw new IllegalArgumentException();
        }
        
        if(index == 0) {
            Node del = this.first;
            //this.first = del.next;
            return del.element;
        }


        Node current = this.first;
        Node del;
        
        for(int i = 0; i<size; i++) {
            if(i == index) {
                //System.out.println("KOMMER JAG TRALALA?");
                return current.element;                 
            }
            current = current.next;
        }
        System.out.println("KOMMER JAG HIT?");
        return null;                 

    } 

    public int size(){
        return this.size;
    }
}

/// Här börjar resten av koden
public class Uppgift2 {
    private static Sequence createSequence(Object... elements) {
	Sequence s = new Solution();
	for (Object o : elements) {
	    s.addBack(o);
	}
	return s;
    }
    // Denna funktion kan du ignorera!
    private static boolean compare(Sequence s, Object... elements) {
	if (s.size() != elements.length) return false;
	for (int i = 0; i < elements.length; ++i) {
            //System.out.println(elements[i]);
            //System.out.println(s.get(i));
	    if (elements[i].equals(s.get(i)) == false) return false;
	}
	return true;
    }
	
    public static void main(String args[]) {
	Sequence s = createSequence("1", "2", "3");
	assert(compare(s, "1", "2", "3"));

	s.addFront("5");
	assert(compare(s, "5", "1", "2", "3"));

	s.del(0);
	assert(compare(s, "1", "2", "3"));

	assert(s.get(2) == "2");

	s.insert("5", 2);
	assert(compare(s, "1", "2", "5", "3"));

	s.del(2);
	assert(compare(s, "1", "2", "3"));

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

	try {
	    s.insert(s.get(0), -100);
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	try {
	    s.insert(s.get(0), s.size() + 100);
	    assert(false);
	} catch (IllegalArgumentException iae) {
	    // This is expected
	}

	// Create some useless data objects for testing
	Data[] data = new Data[] { new Data(), new Data(), new Data(), new Data(), new Data(), new Data() };

	Sequence d = createSequence(data[1], data[2], data[3]);
	assert(compare(d, data[1], data[2], data[3]));

	d.addFront(data[5]);
	assert(compare(d, data[5], data[1], data[2], data[3]));

	d.del(0);
	assert(compare(d, data[1], data[2], data[3]));

	assert(d.get(2) == data[2]);

	d.insert(data[5], 2);
	assert(compare(d, data[1], data[2], data[5], data[3]));

	d.del(2);
	assert(compare(d, data[1], data[2], data[3]));

	assert(d.size() == 3);

	System.out.println("Alla tester passerar");
	System.out.println("\nObservera att O(1)-implementationen av size inte testas!");
    }

    static class Data {}
}

interface Sequence {
    /**
     * Adds an element at the front of the sequence. For example,
     * if a sequence s is [1,2,3], s.addFront(5) will yield
     * [5,1,2,3].
     *
     * @param o the element to be added to the front of the sequence
     */
    public void addFront(Object o); 

    /**
     * Adds an element at the end of the sequence. For example,
     * if a sequence s is [1,2,3], s.addFront(5) will yield
     * [1,2,3,5].
     *
     * @param o the element to be added to the end of the sequence
     */
    public void addBack(Object o);

    /**
     * Inserts an element somewere in the sequence. For example
     * if a sequence s is [1,2,3], s.addFront(2) will yield
     * [1,2,5,3]. 
     *
     * The index must be between 0 and size(). Using 0 for index
     * is the same as addFront(). Using size() for index is the
     * same as addBack().
     *
     * @param o the element to be added to the sequence
     * @param index the index for the new element
     * @throws IllegalArgumentException if index is invalid
     */
    public void insert(Object o, int index) throws IllegalArgumentException;

    /**
     * Removes an element from the sequence at the given index.
     * For example, if a sequence s is [1,2,3], s.del(1) will
     * yield [1,3] and 2 will be returned.
     *
     * The index must be between 0 and size()-1. 
     *
     * @param index the index of the element to be removed
     * @throws IllegalArgumentException if index is invalid
     * @return the deleted element
     */
    public Object del(int index) throws IllegalArgumentException;

    /**
     * Returns an the element from the sequence at the given
     * index. For example, if a sequence s is [1,2,3], s.get(1)
     * will return 2. The sequence is not changed.
     *
     * The index must be between 0 and size()-1. 
     *
     * @param index the index for the element to be returned
     * @throws IllegalArgumentException if index is invalid
     * @return the element at the given index
     */
    public Object get(int index) throws IllegalArgumentException;

    /**
     * Returns the size of the sequence calculated in O(1) time.
     *
     * @return the number of elements in the sequence
     */
    public int size(); 
}

