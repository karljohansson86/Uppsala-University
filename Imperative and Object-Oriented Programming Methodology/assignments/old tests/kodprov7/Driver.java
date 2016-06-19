
class List<T> implements Sequence<T> {
   
    Node first;
    Node last;
    int size = 0;
    
    private class Node {
        T element;
        Node next;

        public Node(T e) {
            this.element = e;
            this.next = null;
        }
    }

    
    /**
     * Append e to the end of the list
     */
    public void append(T e) {

        if(e == null) {
            return;
        }
        
        Node n = new Node(e);
        
        if(this.size == 0) {
            this.first = n;
            this.last = n;
            this.size++;
            //System.out.println("append  + sieze 0" + e);
            return;
        } else {
            this.last.next = n;
            this.last = n;
            this.size++;
            //System.out.println("append " + e);
        }
    }
    
    /**
     * Returns the nth element in the list (starting from 0). 
     */
    public T get(int index) throws IllegalArgumentException {
        
        int size = this.size;
        if(index > size-1 || index < 0) {
            throw new IllegalArgumentException();
        }
        
        
        
        if(index == 0) {
            //System.out.println("Första " + this.first.element);
            return this.first.element;
        }
        
        if(index == size-1) {
            //System.out.println("Sista " + this.last.element);
            return this.last.element;
        }
        
        Node current = this.first;
        
        // int counter = 0;

        // while (counter != index-1) {
            
        //     //System.out.println(counter);
        //     current = current.next;
        //     counter++;
        // }
        
        //return current.element;


        for (int i = 0; i < size; i++) {
            if(i == index-1) {
                //System.out.println("Mitt i " + current.element);
                return current.element;
            } else {
                current = current.next;
            }
        }
        System.out.println("HELEVETE");
        throw new IllegalArgumentException();
    }






    /**
     * Append e to the front of the list
     */
    public void prepend(T e) {        
        if(e == null) {
            return;
        }
        
        Node n = new Node(e);
        
        if(this.size == 0) {
            this.first = n;
            this.last = n;
            this.size++;
            //System.out.println("prepend + size 0 " + e);
            return;
        }
        if(this.size == 1){
            n.next = this.first;
            this.first = n;
            this.size++;
            //System.out.println("prepend  + size 1" + e);
            return;
        }
        else {
            n.next = this.first;
            this.first = n;
            this.size++;
            //System.out.println("first " + this.first.element + " 2nd: " +this.first.next.element);
            //System.out.println("prepend " + e);
            
        }
    }

    /**
     * Removes and returns the nth element in the list (starting from 0). 
     */
    public T remove(int index) {
        int size = this.size;
        if(index> size-1 || index < 0 || size == 0) {
            throw new IllegalArgumentException();
        }

        Node delete;

        if(size == 1) {
            if(index == 0) {
                delete = this.first;
                this.first = null;
                this.last = null;
                this.size--;
                return delete.element;
            } else {
                throw new IllegalArgumentException();
            }
        }

        if(index == 0) {
            delete = this.first;
            this.first = this.first.next;
            this.size--;
            return delete.element;
        }
        
        Node current = this.first;
        
        for(int i = 0; i<index;i++) {
            if(i == index-1) {
                delete = current.next;
                current.next = delete.next;
                this.size--;
                if(i == size-1){
                    this.last = current;
                }
                return delete.element;
            }
            current = current.next;
        }
        
        throw new IllegalArgumentException();
        
    }

    /**
     * Return the length of the list
     */
    public int length() {
        return this.size;
    }

 
}

public class Driver {
    // This static method takes a sequence and prints it from both
    // ends in an alternating fashion
    public static <T> void printList(Sequence<T> list) {
        int front = 0;
        int end = list.length() - 1;
        
        for (int i = 0; i < list.length(); ++i)
            {
                if (i % 2 == 0)
                    {
                        System.out.print(list.get(front));
                        ++front;
                    }
                else
                    {
                        System.out.print(list.get(end));
                        --end;
                    }
                System.out.print(" ");
            }
        System.out.println("");
    }
    
    public static void main(String args[])
    {
        // Create a new empty sequence
        Sequence<String> x = new List<String>();
        
        // Add x, y and z to x using prepend, i.e., [z, y, x] is order in main
        x.prepend("x");
        // for(int i = 0; i< x.length(); i++) {
        //     System.out.println("Printar el. " + x.get(i));
        // }
        x.prepend("y");
        // for(int i = 0; i< x.length(); i++) {
        //     System.out.println("Printar el. " + x.get(i));
        // }
        x.prepend("z");

        for(int i = 0; i< x.length(); i++) {
            //System.out.println("Printar el. " + x.get(i));
        }

        // Print the list
        printList(x);
        // Remove the last element
        x.remove(2);
        // Print the list
        printList(x);
        // Remove the last element
        x.remove(1);
        // Print the list
        printList(x);
        
        // Create a new empty list
        Sequence<String> s = new List<String>();

        // Append argument words
        for (String arg : args)
            {
                System.out.println(arg);
                s.append(arg);
            }

        // Print the list
        printList(s);
    }
}
    
