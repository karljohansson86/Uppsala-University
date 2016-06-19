import java.util.*;


public class Player {
    
    /**
     * Players hp, get it to 180 to be able to graduate
     */
    private int hp = 0;
    
    /**
     * Maxsize of the backpack, that will hold the items(books and keys)
     */
    private int backPackSizeMax = 10;

    /**
     * Current size of the item stored in the backpack
     */
    private int CurrentBackPackSize = 0;
    
    /**
     * The backpack represented by an arraylist
     */
    private ArrayList<Item> backPack = new ArrayList<Item>();
    
    /**
     * Courses player is currently enrolled in
     */
    private ArrayList<Course> currentCourses = new ArrayList<Course>();
    
    /**
     * Courses player has finished 
     */
    private ArrayList<Course> finishedCourses = new ArrayList<Course>();


    /**
     * Constructor for player
     */
    public Player()
    {

    }

    /**
     * Add an item to players backpack 
     *
     * @param i to be added to the backpack if enough capacity left in the backpack
     */
    public void addToBackPack(Item i) {
        updateBackPack();
        int totSize = this.CurrentBackPackSize + i.getSize();
        
        if(totSize > backPackSizeMax) {
            //throw excemption
            return;
        }
        
        else{
            backPack.add(i);
            updateBackPack();
        }
    }
    
    /**
     * Get the current size of all the item stored in the backpack
     *
     * @return Sum of the size of the item stored
     */
    public int getCurrentBackPackSize() {
        return this.CurrentBackPackSize;
    }

    /**
     * Update the variable currentBackPackSize to correct amount
     */
    public void updateBackPack() {
        this.CurrentBackPackSize = 0;
        for(Item k :this.backPack) {
            this.CurrentBackPackSize += k.getSize();
        }
    }

    /**
     * Get the players hp 
     *
     * @return the players current hp
     */
    public int getHp(){
        updateHp();
        return this.hp;
        //return 180;
    }

    /**
     * Updates the players hp
     */
    public void updateHp()
    {
        this.hp = 0;
        for(Course c : this.finishedCourses){
            hp += c.getHp();
        }
    }
   

    /**
     * Get the items in the players backpack
     *
     * @return Arraylist of the items in the players backpack
     */
    public ArrayList<Item> getBackPack(){
        return this.backPack;
    }
    
    /**
     * Use a key from players backpack
     *
     * @return true if player had a key in the packpack else false
     */
    public boolean useKey()
    {
        for(Item i : this.backPack) {
            boolean isKey = i instanceof Key;
            if(isKey) {
                updateBackPack();
                backPack.remove(i);
                return true;
            }
        }
        
        return false;
            
    }

    /**
     * Enroll player in a course
     *
     * @param course to be enrolled in
     */
    public void enroll(Course course) {
        this.currentCourses.add(course);
    }

    /**
     * Get the current courses player is enrolled in
     *
     * @return Arraylist of the courses player is enrolled in
     */
    public ArrayList<Course> getCurrentCourses(){
        return this.currentCourses;
    }

    /**
     * Get the current courses player has finished
     *
     * @return Arraylist of the courses player has finished
     */    
    public ArrayList<Course> getFinishedCourses(){
        return this.finishedCourses;
    }
        


}
