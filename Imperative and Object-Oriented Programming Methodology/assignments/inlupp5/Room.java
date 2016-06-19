
/**
 * Representation of a room
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

import java.lang.StringBuilder;
import java.util.ArrayList;

public class Room{
    
    /**
     * Room name
     */
    private String roomName;
    
    /**
     * Room located to the north of this room
     */
    private Room roomNorth;


    /**
     * Room located to the east of this room
     */
    private Room roomEast;
    
    /**
     * Room located to the south of this room
     */
    private Room roomSouth;
    
    /**
     * Room located to the west of this room
     */
    private Room roomWest;
    
    /**
     * Door the north locked or not
     */
    private boolean doorNorthLocked;
    
    /**
     * Door the east locked or not
     */
    private boolean doorEastLocked;
    
    /**
     * Door the south locked or not
     */
    private boolean doorSouthLocked;
    
    /**
     * Door the west locked or not
     */
    private boolean doorWestLocked;
    
    /**
     * Creatures in the room
     */
    private ArrayList<Creature> creatures = new ArrayList<Creature>();
    
    /**
     * Item in the room
     */
    private ArrayList<Item> items = new ArrayList<Item>();

    //Constructor
    /**
     * Constructor of the room
     * Everything but roomname set to null or false 
     * @param name of the room
     */
    public Room(String name) {
        this.roomName = name;
    }

    //Accessors
    /**
     * Get if the door to the north is locked
     *
     * @return true if door is locked else false
     */
    public boolean getDoorNorthLocked() {
        return this.doorNorthLocked;
    }

    /**
     * Get if the door to the east is locked
     *
     * @return true if door is locked else false
     */
    public boolean getDoorEastLocked() {
        return this.doorEastLocked;
    }

    /**
     * Get if the door to the south is locked
     *
     * @return true if door is locked else false
     */
    public boolean getDoorSouthLocked() {
        return this.doorSouthLocked;
    }

    /**
     * Get if the door to the west is locked
     *
     * @return true if door is locked else false
     */
    public boolean getDoorWestLocked() {
        return this.doorWestLocked;
    }    
        
    /**
     * Add a creature to the room
     *
     * @param c creature to be added to the room
     */
    public void addCreature(Creature c)
    {
        this.creatures.add(c);
    }

    /**
     * Add a key to the room
     *
     * @param k Key to be added to the room
     */
    public void addKey(Key k) {
        this.items.add(k);
    }

    /**
     * Add a book to the room
     *
     * @param b book to be added to the room
     */
    public void addBook(Book b) {
        this.items.add(b);
    }
    
    /**
     * Set the rooms name
     *
     * @param name to set as the rooms name
     */
    public void setName(String name) {
        this.roomName = name;
    }

    /**
     * get the rooms name
     *
     * @return string with the rooms name
     */
    public String getName() {
        return this.roomName;
    }
    
    /**
     * Get the room to the north of this room
     *
     * @return room located to the north
     */
    public Room getRoomNorth() {
        return this.roomNorth;
    }
    
    /**
     * Set the room to the north of this room
     *
     * @param r to be set
     */
    public void setRoomNorth(Room r) {
        this.roomNorth = r;
    }

    /**
     * Get the room to the east of this room
     *
     * @return room located to the east
     */
    public Room getRoomEast() {
        return this.roomEast;
    }

    /**
     * Set the room to the east of this room
     *
     * @param r to be set
     */
    public void setRoomEast(Room r) {
        this.roomEast = r;
    }
    
    /**
     * Get the room to the south of this room
     *
     * @return room located to the south
     */
    public Room getRoomSouth() {
        return this.roomSouth;
    }

    /**
     * Set the room to the south of this room
     *
     * @param r to be set
     */
    public void setRoomSouth(Room r) {
        this.roomSouth = r;
    }
    
    /**
     * Get the room to the west of this room
     *
     * @return room located to the west
     */
    public Room getRoomWest() {
        return this.roomWest;
    }

    /**
     * Set the room to the west of this room
     *
     * @param r to be set
     */
    public void setRoomWest(Room r) {
        this.roomWest = r;
    }
		
    /**
     * Set if the door to the north should be locked or not
     *
     * @param status true if it should be else false
     */
    public void setDoorNorthLocked(boolean status) {
        this.doorNorthLocked = status;
    } 

    /**
     * Set if the door to the east should be locked or not
     *
     * @param status true if it should be else false
     */	
    public void setDoorEastLocked(boolean status) {
        this.doorEastLocked = status;
    }

    /**
     * Set if the door to the south should be locked or not
     *
     * @param status true if it should be else false
     */    
    public void setDoorSouthLocked(boolean status) {
        this.doorSouthLocked = status;
    } 
    
    /**
     * Set if the door to the west should be locked or not
     *
     * @param status true if it should be else false
     */
    public void setDoorWestLocked(boolean status) {
        this.doorWestLocked = status;
    }

    /**
     * Get item from the room if any available
     *
     * @param item the item to get 
     */
    public Item getItem(String item){
        if(item == null){return null;}

        switch(item) {
        case "KEY":
            for(Item i : this.items) {
                if(i instanceof Key) {
                    Key k = (Key)i;
                    items.remove(i);
                    return k;
                }
            }
            break;
        
        default:
            for(Item i : this.items) {
                if(i instanceof Book) {
                    Book b = (Book)i;
                    if(b.getBookName().toUpperCase().equals(item)) {
                        items.remove(i);
                        return i;
                    }
                }
            }
            break;
        }
        return null;
    }

    /**
     * Get the creature in the room if any
     *
     * @param name of the creature
     * @return the corresponding creature to the parameter if equal to any creature in the room 
     * otherwise null
     */
    public Creature getCreature(String name) {
        for(Creature c : this.creatures) {
            if(c.getName().toUpperCase().equals(name)) {
                return c;
            }
        }
        
        return null;
    }
    

    //Methods

    /**
     * Get the descriction of the room and the creatures and item located in it
     *
     * @return String representing the description of the room
     */
    public String getDescription()
    {
        StringBuilder sb  = new StringBuilder();
        sb.append("\nYou are standing in " +this.roomName+".");
        for(Creature c : creatures) {
            sb.append("\nYou see a " + c.toString());
        }
        
        int amountOfKeys = 0;
        
        for(Item i : items) {
            if(i instanceof Book) {
                Book b = (Book)i;
                sb.append("\nYou see " + b.getBookName());
            }
            if(i instanceof Key) {
                amountOfKeys++;
            }
        }
        if(amountOfKeys > 0) {
           sb.append("\nYou see " + amountOfKeys + " key[s]"); 
        }
        
        
        if(this.roomNorth != null) {
            sb.append("\nThere is a door to the north.");
        }
        if(this.roomEast != null) {
            sb.append("\nThere is a door to the east.");
        }
        if(this.roomSouth != null) {
            sb.append("\nThere is a door to the south.");
        }
        if(this.roomWest != null) {
            sb.append("\nThere is a door to the west.");
        }        
        
        return sb.toString();
    }
    
    
    /**
     * Creates a string representing the rooms name
     *
     * @return String name of the room
     */
    public String toString() {
        return this.roomName;
    }
    

}
