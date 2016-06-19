import static org.junit.Assert.assertTrue;
import org.junit.Test;
import java.util.ArrayList;

public class GameTest {

    @Test
    public void createGame() {
        Room r = new Room("Rum");
        ArrayList<Room> rooms = new ArrayList<Room>();
        rooms.add(r);
        Player p = new Player();
        
        Game g = new Game(rooms, p);
       
        assertTrue(1 == 1);
    }


}
