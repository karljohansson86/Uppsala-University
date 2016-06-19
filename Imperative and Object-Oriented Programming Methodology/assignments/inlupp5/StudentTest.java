import static org.junit.Assert.assertTrue;
import org.junit.Test;
import java.util.ArrayList;

public class StudentTest {

    @Test
    public void createStudentTest() {
        
        Course a = new Course();
        Course b = new Course();
        String name = "namn";
        
        Student s = new Student(a,b,name);
                
        assertTrue(s != null);        
        assertTrue(s.getName().equals("namn"));        
    }
    
    @Test
    public void talkTest() {
        
        Course a = new Course();
        a.setName("CourseNotDone");
        Course b = new Course();
        b.setName("CourseDone");
        String name = "Name1";
        
        Student s = new Student(a,b,name);

        //How to test Talk?
        //s.talk();
    }

    
    @Test
    public void bookStudentWantTest() {
        
        Course a = new Course();
        a.setName("CourseNotDone");
        Book book = new Book("CourseNotDoneBook");
        a.setBook(book);
        
        Course b = new Course();
        b.setName("CourseDone");
        String name = "Name1";
        
        Student s = new Student(a,b,name);
        
        assertTrue(s.bookThatIWant().equals(book));
    }

    @Test
    public void toStringTest() {
        
        Course a = new Course();
        Course b = new Course();
        String name = "Name1";
        
        Student s = new Student(a,b,name);
        
        assertTrue(s.toString().equals("Student named " + name));
    }
    

    // Below test require user input

    // @Test
    // public void tradeTest() {
        
    //     Course a = new Course();
    //     a.setName("CourseNotDone");
    //     Book book1 = new Book("CourseNotDoneBook");
    //     a.setBook(book1);

    //     Course b = new Course();
    //     b.setName("CourseDone");
    //     Book book2 = new Book("CourseDoneBook");
    //     b.setBook(book2); 
    //     Qanda q = new Qanda("Fråga", "korrekt svar", "fel svar" , "fel svar2", 1);
    //     b.setQuestion(q);

    //     String name = "Name1";
        
    //     Student s = new Student(a,b,name);
    //     assertTrue(s.trade(book2) == null);
    //     assertTrue(s.trade(book1) == book2);
    //     assertTrue(s.trade(book1) == null);
    // }

    
}
