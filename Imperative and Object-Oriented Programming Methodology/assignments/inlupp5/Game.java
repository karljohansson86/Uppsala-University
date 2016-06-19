/**
 * This class handles a game session.
 * @author Henrik Johansson
 * @author Karl Johansson
 */
import java.util.ArrayList;
import java.io.Console;
import java.util.Scanner;

public class Game {
    
    //Fields
    /**
     * Is true if the user wants to quit, else false.
     */
    private boolean quit = false;
    /**
     * All the rooms in the world.
     */
    private ArrayList<Room> rooms;
    /**
     * The player object.
     */
    private Player player;
    /**
     * The room the player is currently in.
     */
    private Room current;

    /**
     * All the actions the player can do.
     */
    private enum Actions {
        TALK,
        GO,
        ENROLL,
        GRADUATE,
        UNLOCK,
        COURSES,
        HP,
        TAKE,
        TRADE,
        INVENTORY,
        QUIT,
        OPTIONS
    }


    /**
     * Constructor     
     */
    public Game(ArrayList<Room> rooms, Player player) {
        this.rooms = rooms;
        this.player = player;
        current = rooms.get(0);
    }    


    /**
     * This is the game loop.     
     */
    public void startGame() {
        print("\033[2J\033[;H");
        
        while(!quit) {                
            print(current.getDescription());
            String input = getInput();
            print("\033[2J\033[;H");                
            getUserAction(input);
            this.player.updateHp();
        }
    }

    /**
     * Prints a string to the screen.
     * @param str The string to print
     */
    private void print(String str) {
        System.out.println(str);
    }

    /**
     * Prints a question to the user and returns the answer.
     * @return The users answer as a string.
     */
    private String getInput() {
        Console c = System.console();
        print("What do you do? Write \"options\" for list of options");
        String input = c.readLine().toUpperCase();
        return input;
    }
    
    /**
     * Takes a string and tries to convert it into an Action.
     * If the conversion is succesfull it will execute that action.
     * @param input String representation of an Action.
     */
    private void getUserAction(String input) {
        
        String[] arr = parseInput(input);
        Actions action = doesActionExist(arr[0]);
        if(action != null) {
            act(action,arr[1]);
        }
        else {

        }
    }

    /**
     * Executes an action upon a subject.
     * @param a The action to execute.
     * @param subject The subject
     */
    private void act(Actions a, String subject) {
        switch(a) {
        case GO:
            Room r1 = doesDirectionExist(subject);                
            go(r1,subject);
            break;
        case TALK:
                Creature c = doesCreatureExist(subject);
                if(c != null) { 
                    talk(c);
                }
                break;
        case UNLOCK:
            Room r2 = doesDirectionExist(subject);                                
            unlock(r2, subject);
            break;
        case OPTIONS:
            options();
            break;
        case ENROLL:
            Creature c2 = doesCreatureExist(subject);
            if(c2 != null) {
                enroll(c2);
            }
            break;
        case COURSES:
            courses();
            break;
        case HP:
            hp();
            break;
        case TAKE:
            take(subject);
            break;
        case QUIT:
            this.quit = true;
            break;
        case TRADE:
            Creature c3 = doesCreatureExist(subject);
            if(c3 != null) {
                trade(c3);
            }
            break;
        case INVENTORY:
            inventory();
            break;
        case GRADUATE:
            Creature c4 = doesCreatureExist(subject);
            if(c4 != null){
                if(graduate(c4)) {
                    this.quit = true;
                }
            }
            
            break;
        default:
            print("No such action!");
            break;
        }
    }
    
    /**
     * Splits user input into an string representation of an action and subject.
     * @param input The string to split.
     */
    private String[] parseInput(String input) {
        String action = null;
        String subject = null;
        Scanner s = new Scanner(input);
        s.useDelimiter(" ");
        if(s.hasNext()) {
            action = s.next();
        }
        if(s.hasNext()) {
            subject = s.next();                
        }
        
        
        return new String[]{action,subject};
        
    }

    
    /**
     * Check if a creature with a given name is in the current room.
     * @param subject Name of the creature.
     * @return The creature whose name == subject, else null.
     */
    private Creature doesCreatureExist(String subject) {
        if(subject == null) {return null;}
        return this.current.getCreature(subject);
    }

    /**
     * Checks if a string corresponds to a valid direction.
     * @param direction The string representation of a direction.
     * @return If the direction is valid, the room in that direction will be returned. Else null.
     */
    private Room doesDirectionExist(String direction) {
        if(direction == null){return null;}
        
        switch(direction) {
        case "NORTH":
            return this.current.getRoomNorth();
            
        case "EAST":
            return this.current.getRoomEast();
            
        case "SOUTH":
            return this.current.getRoomSouth();
            
        case "WEST":
            return this.current.getRoomWest();

        default:
            return null;
        }
    }

    /**
     * Checks id a string corresponds to a valid action.
     * @param act The string representation of an action.
     * @return An valid action, else null.
     */
    private Actions doesActionExist(String act) {
        if(act == null){return null;}
        try {
            Actions a = Actions.valueOf(act);
            return a;
        }
        catch(IllegalArgumentException e) {
            return null;
        }
    }
    /**
     * Executes the go action.
     * @param destination Which room the player wants to enter.
     * @param direction Which direction the player wants to go.
     */
    private void go(Room destination, String direction) {
        if(destination == null || direction == null){return;}
        
        boolean locked  = false;
        
        switch(direction) {
        case "NORTH":
            if(this.current.getDoorNorthLocked()) {locked = true;}
            break;
            
        case "EAST":
            if(this.current.getDoorEastLocked()) {locked = true;}
            break;
            
        case "SOUTH":
            if(this.current.getDoorSouthLocked()) {locked = true;}
            break;
            
        case "WEST":
            if(this.current.getDoorWestLocked()) {locked = true;}
            break;

        default:
            return;
        }
        
        if(locked) {
            print("The door is locked!");
        }
        else  {
            this.current = destination;
        }
    }

    /**
     * Executes the talk action.
     * @param c The creature to talk to.
     */
    private void talk(Creature c) {
        if(c == null){
            return;
        }
        else  if( c instanceof Student) {
            Student s = (Student)c;
            s.talk();
        }
        else if(c instanceof Teacher) {
            Teacher t = (Teacher)c;
            //Check if player have the book needed for the teachers course.
            boolean haveBook = false;
            for(Item i : player.getBackPack()) {
                if(i instanceof Book) {
                    Book b = (Book)i;
                    if(t.getCourse().getBook().equals(b)) {
                        haveBook = true;
                        break;
                    }
                }
            }
            t.talk(this.player.getCurrentCourses(), this.player.getFinishedCourses(), haveBook);
        }
        else if(c instanceof Sfinx){
            Sfinx sf = (Sfinx)c;
            sf.talk();
        }
    }

    /**
     * Executes the unlock action.
     * @param destination The room the player wants to ulock the door to.
     * @param direction The direction the player is going.
     */
    private void unlock(Room destination, String direction) {
        if(destination == null || direction == null) {return;}
        
        switch(direction) {
        case "NORTH":
            if(this.current.getDoorNorthLocked()) {
                if(this.player.useKey()) {
                    this.current.setDoorNorthLocked(false);
                    destination.setDoorSouthLocked(false);
                    print("You unlocked the door to the north");
                }
                else {
                    print("You have no key!");
                }
            }
            else {
                print("The door is not locked...");
            }
            break;

        case "EAST":
            if(this.current.getDoorEastLocked()) {
                if(this.player.useKey()) {
                    this.current.setDoorEastLocked(false);
                    destination.setDoorWestLocked(false);
                    print("You unlocked the door to the east");
                }
                else {
                    print("You have no key!");
                }
            }
            else {
                print("The door is not locked...");
            }                
            break;
            
        case "SOUTH":
            if(this.current.getDoorSouthLocked()) {
                if(this.player.useKey()) {
                    this.current.setDoorSouthLocked(false);
                    destination.setDoorNorthLocked(false);
                    print("You unlocked the door to the south");
                }
                else {
                    print("You have no key!");
                }
            }
            else {
                print("The door is not locked...");
            }                
            break;
            
        case "WEST":
            if(this.current.getDoorWestLocked()) {
                if(this.player.useKey()) {
                    this.current.setDoorWestLocked(false);
                    destination.setDoorEastLocked(false);
                    print("You unlocked the door to the west");
                }
                else {
                    print("You have no key!");
                }
            }
            else {
                print("The door is not locked...");
            }                
            break;
            
        default:
            break;
        }
    }
    /**
     * Executes the enroll action. Only succesfull if the creature is a teacher.
     * @param c The creature to execute enroll on.
     */
    private void enroll(Creature c){
        if(c == null) {return;}
        if(c instanceof Teacher){
            Teacher t = (Teacher)c;
            Course course = t.getCourse();
            this.player.enroll(course);
            print("You have enrolled in " + course.getName());
        }
        
    }
    
    /**
     * Executes the courses action. Prints the players finnished and unfinnished courses.
     */
    private void courses(){    
        ArrayList<Course> current = this.player.getCurrentCourses();

        if(current.size() != 0) {
                print("You are enrolled in the following courses:");
                for(Course c : current){
                    print(c.getName());
                }                
            }
        else {
            print("You are not enrolled in any courses.");
        }
        
        ArrayList<Course> done = this.player.getFinishedCourses();
        if(done.size() != 0){
                print("You are done with the following courses:");
                for(Course c : done){
                    print(c.getName());
                }                
            }
        else {
            print("You have not completed any courses.");
        }        
    }
    
    /**
     * Executes the options action. Prints all available actions to the player.
     */
    private void options() {
        print("[Your character can do the following]");
        for(Actions a : Actions.values()) {
            print(a.toString().toLowerCase());
        }
    }
    
    /**
     * Executes the hp action. Prints the players current hp.
     */
    private void hp(){
        print("You have "+ this.player.getHp() + " hp");
    }
    /**
     * Executes the take action. Tries to cast a string into an item, If succesfull that item is stored in the players backpack.
     * @param subject String representation of an item.
     */
    private void take(String subject){
        Item i = this.current.getItem(subject);
        if(i != null){
            this.player.addToBackPack(i);
            print("You picked up a " + subject.toLowerCase());
        }
        else{
            print("No such item");
        }
        

    }

    /**
     * Executes the trade action. Is only succesfull if the creature is a student.
     * @param c The creature to trade with.
     */
    private void trade(Creature c) {
        if(c == null){return;}
        Student s = null;
        
        try {
            s = (Student)c;     
        }
        catch(ClassCastException e) {
            print("You can only trade with students!");
            return;
        }
        
        Book toLookFor = s.bookThatIWant();            
        ArrayList<Course> current = this.player.getCurrentCourses();
        
        for(Course c1 : current){
            if(c1.getBook().equals(toLookFor)) {
                Book b = s.trade(toLookFor);
                c1.setBook(null);
                this.player.addToBackPack(b);
                return;
            }
        }

        ArrayList<Course> done = this.player.getFinishedCourses();
        for(Course c2 : done){
            if(c2.getBook().equals(toLookFor)) {
                Book b = s.trade(toLookFor);
                c2.setBook(null);
                this.player.addToBackPack(b);                
                return;
            }            

        }

        print("You dont have the book needed for the trade.");
        
        
        
    }

    /**
     * Executes the inventory action. Prints all items in the players backpack.
     */
    private void inventory(){

        ArrayList<Item> inventory = this.player.getBackPack();
        StringBuilder sb  = new StringBuilder();
        if(inventory.size() > 0){
            print("You have the following items in your backpack:");
            int amountOfKeys = 0;
            for(Item i : inventory){
                if(i instanceof Book) {
                    Book b = (Book)i;
                    sb.append("\n" +b.getBookName());
                }
                if(i instanceof Key) {
                    amountOfKeys++;
                }
                //print(i.toString());
            }    
            if(amountOfKeys > 0) {
                sb.append("\n " + amountOfKeys + " key[s]"); 
            }
            System.out.println(sb.toString());
        }
        else{
            print("Your backpack is empty!");
        }

    }

    /**
     * Executes the graduate action. Is only succesfull if the creature is a sfinx.
     * @param c The creature to use graduate on.
     */
    private boolean graduate(Creature c){
        if(c == null) {
            return false;
        }

        if(c instanceof Sfinx) {
            Sfinx s = (Sfinx)c;
            boolean graduated = s.graduate(this.player.getHp());
            return graduated;
        }
        return false;
    }

        
}
