import java.util.Iterator;
import java.lang.Iterable;
import java.util.Stack;

public class Uppgift2 {
    public static void main(String[] args) {
        Object t1 = new Tree("B", "A", "C");
        Object t2 = new Tree("A", "C", "B");

        if (t1.equals(t2))
            System.out.println("Träden " + t1 + " och " + t2 + " är lika");
        else
            System.out.println("Träden " + t1 + " och " + t2 + " är INTE lika");

        if (t2.equals(t1))
            System.out.println("Träden " + t2 + " och " + t1 + " är lika");
        else
            System.out.println("Träden " + t2 + " och " + t1 + " är INTE lika");

    }
}

class Tree implements Iterable<Comparable> {
    private Node root;

    public Tree(final Comparable r, final Comparable... cs) {
        this.root = new Node(r);
        for (final Comparable c : cs) {
            this.insert(c);
        }
    }

    public void insert(final Comparable c) {
        this.root.insert(c);
    }

    public String toString() {
        return this.root.toString();
    }

    public int depth() {
        return this.root.depth();
    }

    public int size() {
        return this.root.size();
    }

    public boolean contains(Comparable c) {
	return this.root.contains(c);
    }

    // Tillåter for (Comparable c : myTree) { ... }
    public Iterator<Comparable> iterator() {
        return new TreeIterator(this.root);
    }
}

class Node {
    Comparable element;
    Node left;
    Node right;

    public Node(final Comparable e) {
        this.element = e;
    }

    public void insert(final Comparable c) {
        final int order = element.compareTo(c);

        if (order < 0) {
            if (left == null) {
                left = new Node(c);
            } else {
                left.insert(c);
            }
        }
        if (order > 0) {
            if (right == null) {
                right = new Node(c);
            } else {
                right.insert(c);
            }
        }
    }

    public boolean contains(Comparable c) {
        final int order = element.compareTo(c);

        if (order == 0) return true;

        if (order <  0) {
            return left == null ? false : left.contains(c);
        } else {
            return right == null ? false : right.contains(c);
        }
    }

    public String toString() {
        return "(" +
            this.element +
	    ((this.left == null && this.right == null) ? "" :
                ((this.left == null ? "()" : this.left.toString()) +
	         (this.right == null ? "()" : this.right.toString()))) +
            ")";
    }

    public int depth() {
        int leftDepth = (this.left == null ? 0 : this.left.depth());
        int rightDepth = (this.right == null ? 0 : this.right.depth());
        return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    }

    public int size() {
        int leftSize = (this.left == null ? 0 : this.left.size());
        int rightSize = (this.right == null ? 0 : this.right.size());
        return 1 + leftSize + rightSize;
    }
}

// Du behöver nog inte titta på denna kod!!!
class TreeIterator implements Iterator<Comparable> {
    private Stack<Node> stack = new Stack<Node>();
    private Node cursor;

    public TreeIterator(Node n) {
	this.cursor = findSmallest(n, stack);
    }

    public boolean hasNext() {
	return cursor != null;
    }

    private Node findSmallest(Node n, Stack<Node> s) {
	while (n.left != null) {
	    s.push(n);
	    n = n.left;
	}
	return n;
    }

    public void remove() {
	throw new RuntimeException("Not implemented");
    }

    public Comparable next() {
	Comparable c = cursor.element;
	if (cursor.right == null) {
	    if (stack.size() == 0) {
		cursor = null;
	    } else {
		cursor = stack.pop();
	    }
	} else {
	    cursor = findSmallest(cursor.right, stack);
	}
	return c;
    }
}
