

/**
 * Representation of a course
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */


public class Course{

    /**
     * Course name
     */
    private String name = null;
    
    /**
     * Course question 
     */
    private Qanda question = null;
    
    /**
     * Course 
     */
    private int hp = 10;
    
    /**
     * Specific book for the course
     */
    private Book book;


    /**
     * Constructor of the course
     * Sets name,question and book to null and hp to 0
     */
    public Course(){
    }

    /**
     * Set course name
     *
     * @param name course name
     */
    public void setName(String name)
    {
        this.name = name;
    }

    /**
     * Get course name
     *
     * @return course name
     */
    public String getName() {
        return this.name;
    }

    /**
     * Set course hp
     *
     * @param hp value for the course
     */
    public void setHp(int hp) {
        this.hp = hp;
    }

    /**
     * Get the courses hp value
     *
     *  @return int with courses hp
     */
    public int getHp() {
        return this.hp;
    }
    
    /**
     * Set course book
     *
     * @param book to be set as the courses book
     */
    public void setBook(Book book) {
        this.book = book;
    }

    /**
     * Get the courses book
     *
     * @return the courses book
     */
    public Book getBook(){
        return this.book;
    }
       
    /**
     * Check if its the correct answer
     *
     * @param answer to be checked if correct 
     * @return True if parameter answer was the correct answer else false
     */
    public boolean checkAnswer(int answer) {
        return(question.checkAnswer(answer));
    }
    
    /**
     * Get the correct answer to the courses question
     *
     * @return String that is the correct answer
     */
    public String getCorrectAnswer() {
        return this.question.getCorrectAnswer();
    }
    
    /**
     * Set course question
     *
     * @param q to be set as courses question
     */
    public void setQuestion(Qanda q) {
        this.question = q;
    }

    /**
     * Get the course question
     *
     * @return the courses question
     */
    public Qanda getQuestion(){
        return this.question;
    }
    
    

}







