import static org.junit.Assert.assertTrue;
import org.junit.Test;
import java.util.ArrayList;

public class PlayerTest {

    @Test
    public void createPlayer() {

        Player p = new Player();
        assertTrue(p != null);
    }

    @Test
    public void addToBackPackTest() {
        Player p = new Player();
        Key k = new Key();
        
        assertTrue(p.getCurrentBackPackSize() == 0 );
        
        p.addToBackPack(k);
        assertTrue(p.getCurrentBackPackSize() == 1);        
    }

    @Test
    public void hpTest() {
        
        Player p = new Player();
        Course c = new Course();
        
        assertTrue(p.getHp() == 0);
        p.getFinishedCourses().add(c);
        assertTrue(p.getHp() == c.getHp());
    }

    @Test
    public void getBackPackContentTest() {
        
        Player p = new Player();
        Book b = new Book("BOK");
        Book c = new Book("Bok");
        p.addToBackPack(b);
        
        ArrayList<Item> arr = p.getBackPack();
        
        assertTrue((Book)arr.get(0) == b);
        assertTrue((Book)arr.get(0) != c);
        
    }

    @Test
    public void useKeyTest() {
        
        Player p = new Player();
        Key k = new Key();
        
        assertTrue(p.useKey() == false);

        p.addToBackPack(k);

        assertTrue(p.useKey() == true);       
    }

    @Test
    public void enrollTest() {
        
        Player p = new Player();
        Course c = new Course();        
        
        assertTrue(p.getCurrentCourses().size() == 0);
        
        p.enroll(c);
        
        assertTrue(p.getCurrentCourses().size() == 1);
    }

}
