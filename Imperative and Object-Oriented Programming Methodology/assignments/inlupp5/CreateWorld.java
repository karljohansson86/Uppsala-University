/**
 * A class for createing the game world.
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

import java.util.*;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.text.ParseException;


public class CreateWorld {
    

    /**
     * All the rooms in the world.
     */
    private ArrayList<Room> rooms = new ArrayList<Room>();
    
    /**
     * All the courses in the world.
     */    
    private ArrayList<Course> courses = new ArrayList<Course>();
    
    /**
     * All the books in the world.
     */
    private ArrayList<Book> books = new ArrayList<Book>();
    
    /**
     * The number of locked doors in the world.
     */
    private int lockedDoors;


    /**
     * Constructor of world
     *
     */
    public CreateWorld() {               
        createRooms();
        createKeys();
        createBooks();
        createCourses();
        createQandas();        
        createCreatures();
    }

    
    /**
     * Creates keys and places them randomly in different rooms.
     * The amount of keys created is half the value of lockedDoors.
     */
    private void createKeys() {
        Random random = new Random();
        int randomNum;
        int amountOfKeys = this.lockedDoors;
        for(int i = 0; i < amountOfKeys ; ++i) {
            Key k = new Key();
            randomNum = random.nextInt(rooms.size());
            Room r = rooms.get(randomNum);
            r.addKey(k);            
        }
        
    }


    /**
     * Reads all creature spec-files and creates Creatures accordingly.
     */
    private void createCreatures() {
        ArrayList<String[]> list = parseFile("students.txt",4);
        for(String [] strArr : list) {
            try{
            createStudent(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }

        list = parseFile("teachers.txt",3);
        
        for(String[] strArr : list) {
            try{
            createTeacher(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }

        list = parseFile("sfinx.txt",1);
        for(String[] strArr : list){
            try{
            createSfinx(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
                }
        }
                                   
    }
    /**
     * Creates a sfinx.
     * @param specs Specifies how the Sfinx will be created.
     */
    public void createSfinx(String[] specs) throws CreationException{
        Room room = doesRoomExist(specs[0]);
        if(room == null){
            throw new CreationException("Couldnt find room " + specs[0] +" to place the Sfinx in.");
        }

        room.addCreature(new Sfinx());
    }

    /**
     * Creates a teacher.
     * @param specs Specifies how the teacher will be created.
     */
    public void createTeacher(String[] specs) throws CreationException{
        Room room = doesRoomExist(specs[0]);
        if(room == null) {
            throw new CreationException("Couldnt find "+ specs[0] + " to place the Teacher in.");
        }
    
        
        Course course = doesCourseExist(specs[1]);
        if(course == null) {
            throw new CreationException("Couldnt find course "+specs[1]+" when creatin teacher.");
            
        }
        
        String name = specs[2];

        room.addCreature(new Teacher(course,name));
    }

    /**
     * Creates a student.
     * @param specs Specifies how the student will be created.
     */
    public void createStudent(String[] specs) throws CreationException {
        //Get current course
        Course current = doesCourseExist(specs[1]);        
        //Get finished course
        Course finished = doesCourseExist(specs[2]);
        String name = specs[3];
        
        if(current == null || finished == null) {
            throw new CreationException("Course not found when creating student.");
        }
        
        //Get the room to place student in
        Room room = doesRoomExist(specs[0]);
        if(room == null) {
            throw new CreationException("Room " +specs[0]+ " not found when trying to place student.");
        }
               
        room.addCreature(new Student(current ,finished,name));                    
    }

    /**
     * Creates courses according do a spec-file.   
     */
    private void createCourses() {
        ArrayList<String[]> list = parseFile("courses.txt",3);
        for(String [] strArr : list) {
            try{
            createCourse(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }
    }

    /**
     * Creates a course.
     * @param specs Specifies how the course will be created.
     */
    private void createCourse(String[] specs) throws CreationException {
        Course c = doesCourseExist(specs[0]);
        if(c != null) {
            throw new CreationException("The course "+specs[0]+" allready exists.");
        }
        
        Book b = doesBookExist(specs[1]);
        if(b == null) {
            throw new CreationException("The course "+specs[1]+" allready exists.");
        }
        
        c = new Course();
        c.setName(specs[0]);
        int hp = Integer.parseInt(specs[2]);
        c.setHp(hp);
        c.setBook(b);
        courses.add(c);
        
    }

    /**
     * Creates qandas(question and answers) according to a spec-file.
     */
    private void createQandas() {
        ArrayList<String[]> list = parseFile("qanda.txt",6);
        for(String[] strArr : list) {
            try{
            createQandas(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }
    }

    /**
     * Creates a qanda.
     * @specs Specifies how the qanda will be created.
     */
    private void createQandas(String[] specs) throws CreationException {
        Course c = doesCourseExist(specs[0]);
        if(c == null) {
            throw new CreationException("Couldnt find course "+specs[0]+" to place qanda in.");
        }

        String question = specs[1];
        String answer1 = specs[2];
        String answer2 = specs[3];
        String answer3 = specs[4];
        int correct = 0;
        try{
            correct = Integer.parseInt(specs[5]);
        }
        catch(NumberFormatException e){
            System.out.println("Parse failed when creating Qanda");
        }


        c.setQuestion(new Qanda(question, answer1, answer2, answer3,correct));
        
    }

    /**
     * Creates books according to a spec-file.
     */
    private void createBooks() {
        ArrayList<String[]> list = parseFile("books.txt",3);
        for(String[] strArr : list) {
            try{
            createBook(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }
        
        Random random = new Random();
        int randomNum;
        
        int len = books.size() / 2;

        for(int i = 0; i < len; ++i) {
            randomNum = random.nextInt(books.size());
            Book b = books.get(randomNum);
            randomNum = random.nextInt(rooms.size());
            Room r = rooms.get(randomNum);
            r.addBook(b);
        }
    }

    /**
     * Creates a book.
     *
     * @param specs Specifies how the book will be created.
     */
    private void createBook(String[] specs) throws CreationException {
        Book book = doesBookExist(specs[0]);
        
        if(book == null) {
            book = new Book(specs[0]);
            book.setAuthor(specs[1]);
            
            int size = 0;
            
            try{
            size = Integer.parseInt(specs[2]);
            }
            catch(NumberFormatException e){
                System.out.println(e.toString());
                System.exit(1);
            }
            
            book.setSize(size);
            books.add(book);
        }
        else {
            throw new CreationException("Book allready exists!");
        }
    }

    /**
     * Creates rooms accordinf to a spec-file..
     */
    private void createRooms() {
        ArrayList<String[]> list = parseFile("rooms.txt",9);
        for(String[] strArr : list) {
            try{                
            createRoom(strArr);
            }
            catch(CreationException e){
                System.out.println(e.toString());
                System.exit(1);
            }
        }
    }

     /**
     * Creates a Room.
     * @param spec Specifies how the room will be created.
     */
    private void createRoom(String[] spec) throws CreationException {
        Room room;
        
        //Current room
        Room currentRoom = doesRoomExist(spec[0]);
        if(currentRoom == null && !spec[0].equals("X")) {
            currentRoom = new Room(spec[0]);
            rooms.add(currentRoom);
        }
        else {
            //throw new CreationException("A room name "+ spec[0]+" allready exists.");
        }
        
        //North
        room = doesRoomExist(spec[1]);
        if(room == null && !spec[1].equals("X")) {
            room = new Room(spec[1]);
            rooms.add(room);
        }
        
        currentRoom.setRoomNorth(room);
        
        
        //East
        room = doesRoomExist(spec[2]);
        if(room == null && !spec[2].equals("X")) {
            room = new Room(spec[2]);
            rooms.add(room);
        }
        currentRoom.setRoomEast(room);

        //South
        room = doesRoomExist(spec[3]);
        if(room == null && !spec[3].equals("X")) {
            room = new Room(spec[3]);
            rooms.add(room);
        }
        currentRoom.setRoomSouth(room);

        //West
        room = doesRoomExist(spec[4]);
        if(room == null && !spec[4].equals("X")) {
            room = new Room(spec[4]);
            rooms.add(room);
        }
        currentRoom.setRoomWest(room);

        //Doors
        boolean status =  Boolean.valueOf(spec[5]);
        currentRoom.setDoorNorthLocked(status);
        if(status == true){this.lockedDoors++;}
        
        status = Boolean.valueOf(spec[6]);
        currentRoom.setDoorEastLocked(status);
        if(status == true){this.lockedDoors++;}
        
        status = Boolean.valueOf(spec[7]);        
        currentRoom.setDoorSouthLocked(status);
        if(status == true){this.lockedDoors++;}        
        
        status = Boolean.valueOf(spec[8]);
        currentRoom.setDoorWestLocked(status);
        if(status == true){this.lockedDoors++;}        

                   
        
    }

    /**
     * Chops a string into substrings and saves them in an array.
     * @param line The string to chop.
     *
     */
    private String[] getWords(String line, int segments) {
        String[] words = new String[segments];
        Scanner scanner = new Scanner(line);
        scanner.useDelimiter(";");

        int index = 0;
        while(scanner.hasNext()) {
            words[index] = scanner.next();
            index++;                
        }
        return words;        
    }
	
    /**
     * Checks if a room exists in the ArrayList rooms.
     * @param roomName Name of the room to look for.
     * @return A room if its found, else null.
     */
    private Room doesRoomExist(String roomName) {
        for(Room r : this.rooms) {
            if(r.getName().equals(roomName)) {
               return r;
 
            }
        }
        return null;
    }
    /**
     *Checks if a course exists in the ArrayList courses.
     *@param courseName Name of the course to look for.
     *@return A course if its found, else null.
     */    
    private Course doesCourseExist(String courseName) {
        for(Course c : this.courses) {
            if(c.getName().equals(courseName)) {
                return c;
            }
        }
        return null;
    }

    /**
     * Checks if a book exists in the ArrayList books.
     * @param bookName Name of the book to look for.
     * @return A room if its found, else null.
     */    
    public Book doesBookExist(String bookName) {
        for(Book b : this.books) {
            if(b.getBookName().equals(bookName)) {
                return b;
            }
        }
        return null;
    }    


    /**
     * Splits a text file into segments where each line is an string array
     * and each segment, defined by a delimiter, is an element in that array.
     * @param path The path to the textfile
     * @param segments How many segments there should be.
     * @return A list where each element is an String array containing the segments.
     */
    private ArrayList<String[]> parseFile(String path, int segments) {
        
            ArrayList<String[]> l = new ArrayList<String[]>();  
        
        try {                          
            BufferedReader br = new BufferedReader(new FileReader(path));            

            while(br.ready()) {
                
                String line = br.readLine();
                l.add(getWords(line, segments));
            }
            br.close();            
        }
        catch(IOException e) {
            e.printStackTrace();
        }
        
        return l;

    }
   
    /**
     * Returns a list of all the created rooms.     
     * @return A list with all the rooms in the world.
     */
    public ArrayList<Room> getWorld() {
        return rooms;
    }

}


