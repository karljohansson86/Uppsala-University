import java.util.Scanner;


public class Asker {

    
    public static boolean questionBool(String question) {
        
        Scanner reader = new Scanner(System.in);
        boolean validInput = false;
        boolean answer = false;
        char choice = 0;

        while (!validInput) {

            System.out.print(question + " [YN]: ");
            
            choice = Character.toUpperCase(reader.next().charAt(0));
            if (choice == 'Y') {
                answer = true;
                validInput = true;
            } else if (choice == 'N') {
                validInput = true;
            } else {
                System.out.println("\nNot a valid option");
            }

        }

        //reader.nextLine();
        return answer;
    }

    
    public static int questionInt(String question, int min, int max) {
        
        Scanner reader = new Scanner(System.in);
        boolean validInput = false;
        int choice = -1;

        while (!validInput) {

            System.out.print(question + "? [" + min + "-" + max +"]: ");
            
            try {
                choice = Integer.parseInt(System.console().readLine());
                
                if (choice >= min && choice <= max) {
                    validInput = true;
                } else {
                    System.out.println("\nNumber not inside range. Try again " +
                                       min + "-" + max);
                }

            } catch (NumberFormatException e) {
                System.out.println("\nNot a valid number. Try again");
                //continue;
                //reader.nextLine();
            }

        }

        //reader.nextLine();
        return choice;
    }
    
    
}
