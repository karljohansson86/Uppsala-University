
/**
 * Representation of Question and answers for courses
 *
 * @author Henrik Johansson
 * @author Karl Johansson
 */

public class Qanda
{
    /**
     * The question
     */
    private String question = null;
    
    /**
     * Multiple choice answers
     */
    private String answer1, answer2, answer3 = null;

    /**
     * Whick of the answer that is correct
     */
    private int correct = 0;
    
    /**
     * Constructor of the question
     * 
     * @param q The question
     * @param a1 The first answer option
     * @param a2 The second answer option
     * @param a3 The third answer option
     * @param correct which is the correct answer
     */
    public Qanda(String q, String a1, String a2, String a3, int correct)
    {
        this.question = q;
        this.answer1 = a1;
        this.answer2 = a2;
        this.answer3 = a3;
        this.correct = correct;
    }

    /**
     * Get the question
     */
    public String getQuestion()
    {
        return this.question;
    }

    /**
     * Get the multiple choice answers
     *
     * @return StringArray with the different answers
     */
    public String[] getAnswers()
    {
        String[] strArr = {answer1, answer2, answer3};
        return strArr;
    }
    
    /**
     * Get the correct answer
     *
     * @return String that is the correct answer
     */
    public String getCorrectAnswer() {
        String[] strArr = getAnswers();
        return strArr[this.correct-1];
    }

    /**
     * Check if correct answer
     *
     * @return True if correct answer else false
     */
    public boolean checkAnswer(int answer)
    {
        return (answer == correct);
            
    }

}
