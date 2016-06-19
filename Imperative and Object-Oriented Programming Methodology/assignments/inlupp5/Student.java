
/**
 * Student implementation of the creature interface
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */
import java.io.Console;

public class Student implements Creature
{
    //Fields
    /**
     * Course students is enrolled in
     */
    private Course current; 

    /**
     * Course students has finished
     */
    private Course done;

    /**
     * Students name
     */
    private String name;




    /**
     * Constructor for Student
     *
     * @param current Course student is enrolled in
     * @param done Course student has finished 
     * @param name Students name
     */
    public Student(Course current, Course done, String name)
    {
        this.current = current;
        this.done = done;
        this.name = name;
    }

    //Accessors
    /**
     * {@inheritDoc}
     */
    public String getName(){
        return this.name;
    }

    //Methods
    /**
     * {@inheritDoc}
     */
    @Override
    public void talk() {
        System.out.println("Hi! My name is " +this.name +
                           " and im a student here.\nIm currently taking "+ current.getName()+
                           " and am done with "+ done.getName() + ".");

    }

    /**
     * The book for the current course the student is enrolled in but does not have yet.
     * @return the book
     */
    public Book bookThatIWant(){
        return this.current.getBook();
    }

    /**
     * Trade book with the student in return for another book or answer to a course question
     *
     * @param book player want to give/trade with the student
     * @return book player traded for or null if student didn't want the book or player wanted the answer instead 
     */    
    public Book trade(Book book){
        if(this.current.getBook().equals(book)){
            Console c = System.console();            
            System.out.println("Thanks for the book! What do you want in return?");
            System.out.println("1. The answer to the course " + this.done.getName());
            System.out.println("2. The book for the course " + this.done.getName());
            String input = c.readLine();
            int i = Integer.parseInt(input);
            if(i == 1){
                System.out.println(this.done.getCorrectAnswer());
                return null;
                }
            else{
                System.out.println("So you want the book, well here you go.");
                return this.done.getBook();
            }                        
        }
        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        String what = "Student named ";
        return what + this.name;
    }
        
}
