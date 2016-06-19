
/**
 * Teacher implementation of the creature interface
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

import java.util.Random;
import java.util.ArrayList;
import java.io.Console;

public class Teacher implements Creature {

    /**
     * Course teacher teaches
     */
    private Course course;

    /**
     * Teachers name
     */    
    private String name;

    
    /**
     * Constructor for teacher
     *
     * @param course teacher is teaching
     * @param name Teachers name
     */
    public Teacher(Course course, String name) {
        this.course = course;
        this.name = name;
    }

    /**
     * {@inheritDoc}
     */
    public String getName() {
        return this.name;
    }


    //Methods
    /**
     * {@inheritDoc}
     */
    public void talk()
    {
        System.out.println("Im teaching the course " + this.course.getName());
    }
    
      
    public void talk(ArrayList<Course> unfinnished_courses, ArrayList<Course> finnished_courses, boolean haveBook){
        Random rnd = new Random();
        int i = rnd.nextInt(4);
        Course c = correspondingCourse(unfinnished_courses);

        //Ask question on a unfinnished course.
        if(c != null)
            {
                if(i != 0) //Can be any number as long as its 75%
                    {
                        if(askQuestion(c, haveBook)){
                            finnished_courses.add(c);
                            unfinnished_courses.remove(c);
                            System.out.println("Correct! You are now done with this course.");
                        }
                        else{
                            System.out.println("Wrong! Come back when you have studied.");
                        }
                        return;
                    }
            }

        c = correspondingCourse(finnished_courses);
        //Ask question on a finnished course.
        if(c != null)
            {
                if(i <= 1) // Can be any number as long as its 50%
                    {
                        if(!askQuestion(c,haveBook)){
                            unfinnished_courses.add(c);
                            finnished_courses.remove(c);
                            System.out.println("Wrong! Sadly you must now retake this course.");                           
                        }
                        else{

                            System.out.println("Correct! You dont have to retake this course.");                             
                        }
                        return;
                    }
            }

        talk();                                   
    }

    /**
     *Checks if any of the courses in the list is the same course that the teacher teaches.
     *
     *@param courses List of courses
     *@return The corresponding course if it exists, else null.
     */
    private Course correspondingCourse(ArrayList<Course> courses)
    {
        if(courses == null) {return null;}
        
        for(Course c : courses)
            {
                if(c.equals(this.course))
                    {
                        return c;
                    }
            }
        return null;
    }

    /**
     *Prints the questions for a course, prompts the player for an answer and check if its right.
     *If haveBook is true, one of the wrong answer options will not be printed.
     *
     *@param c The course that have the question that will be printed.
     *@param haveBook Indicates if the player have the course book.
     *@return True if the player answers the question correct, false otherwise.
     */
    private boolean askQuestion(Course c, boolean haveBook)
    {
        
        Qanda q = c.getQuestion();
        System.out.println(this.name + " asks you the following question: ");
        System.out.println(q.getQuestion());
        String[] answers = q.getAnswers();
        int index = 1;
        boolean skip = haveBook;
        for(String str : answers)
            {
                if(skip && !c.checkAnswer(index)){
                    //Just skip to print an answer.
                    skip = false;
                }
                else{
                    System.out.println(index +". "+ str);                                        
                }
                index++;
            }

        Console con = System.console();
        System.out.println("What do you answer?");
        String input = con.readLine();
        int i = Integer.parseInt(input);

        if(c.checkAnswer(i)){
            return true;
            
        }
        else{
            return false;
            
        }                                              
        
    }


    /**
     *Returns the course the teacher is teaching,
     @return a Course
     */    
    public Course getCourse(){
        return this.course;
    }


    /**
     * {@inheritDoc}
     */
    public String toString(){
        String what = "Teacher named ";
        return what + this.name;
    }
}
