import java.util.Iterator;
import java.lang.Iterable;

public class Uppgift2 {
    public static void main(String[] args) {
        Object l1 = new List("B", "A", "C");
        Object l2 = new List("A", "C", "B");
        Object l3 = new List("C", "B", "C", "A");
        Object l4 = new List("A", "B", "C", "B", "Q");

        if (l1.equals(l2))
            System.out.println("Listorna " + l1 + " och " + l2 + " är lika");
        else
            System.out.println("Listorna " + l1 + " och " + l2 + " är INTE lika");

        if (l3.equals(l4))
            System.out.println("Listorna " + l3 + " och " + l4 + " är lika");
        else
            System.out.println("Listorna " + l3 + " och " + l4 + " är INTE lika");

    }
}

class List implements Iterable<Comparable> {
    private Node first;
    private Node last;

    public List(final Comparable f, final Comparable... cs) {
        this.first = new Node(f);
        this.last = this.first;
        for (final Comparable c : cs) {
            this.append(c);
        }
    }

    @Override
    public boolean equals(Object o) {
        
        if(o instanceof List) {
            
            List l = (List)o;
            
            Iterator i = this.iterator();
            
            
            while(i.hasNext()) {
                Comparable c = (Comparable)i.next();
                if(!l.contains(c)) {
                    return false;
                }
                
                
            }
            
        
            Iterator k = l.iterator();

            while(k.hasNext()) {
                Comparable c = (Comparable)k.next();
                if(!this.contains(c)) {
                    return false;
                }
                
                
            }

            return true;
        }
        return false;
    }

    public void append(final Comparable c) {
        this.last.next = new Node(c);
        this.last = this.last.next;
    }

    public String toString() {
        return this.first.toString();
    }

    public int size() {
        return this.first.size();
    }

    public boolean contains(Comparable c) {
        return this.first.contains(c);
    }

    // Tillåter for (Comparable c : myTree) { ... }
    public Iterator<Comparable> iterator() {
        // Du behöver nog inte titta på denna kod!!!
        return new Iterator() {

            private Node cursor = first;

            public boolean hasNext() {
                return cursor != null;
            }

	    public void remove() {
		throw new RuntimeException("Not implemented");
	    }

            public Comparable next() {
                Comparable c = cursor.element;
                cursor = cursor.next;
                return c;
            }
        };
    }
}

class Node {
    Comparable element;
    Node next;

    public Node(final Comparable e) {
        this.element = e;
    }

    public boolean contains(Comparable c) {
        if (this.element.compareTo(c) == 0) return true;

        return (this.next == null) ? false : this.next.contains(c);
    }

    public String toString() {
        return this.element.toString()
            + (this.next == null ? "" : (", " + this.next.toString()));
    }

    public int size() {
        return 1 + (this.next == null ? 0 : this.next.size());
    }
}
