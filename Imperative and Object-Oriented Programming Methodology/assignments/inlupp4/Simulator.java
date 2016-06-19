import java.util.Random;
import java.util.Scanner;


public class Simulator {

    public static void main(String[] args) throws InterruptedException {

        Options o = new Options();
        selectMode(o, args);

        Simulation s = o.sim;
        int steps = o.steps;

        if (steps < 0) { return; }

        s.startSim();
        
        for(int i = 0; i < steps; i++){
            System.out.print("\033[2J\033[;H");
            s.step();
            System.out.println(s);
            System.out.print("Step: " + (i+1) + " / " + steps + "\n");
            Thread.sleep(500);
        }
        
        System.out.println("");
        System.out.println("Simulation finished!");
        
    }

    private static void selectMode(Options o, String args[]) {

         if (args.length == 1) {
            if (args[0].equals("--d")) {
                o.sim = new Simulation(true);
                
            } else {
                System.out.println("\n" + args[0] + " is not a supported option");
                System.out.println("Available options: --d (default values)");
                o.steps = -1;
            }

            return;
         }

         boolean rMode = Asker.questionBool("\nSimulate using random parameters?");

         if (rMode) {
             Random random = new Random();
             o.sim = new Simulation(random);

         } else {
             o.steps = Asker.questionInt("(1/5) How many steps to simulate", 1, 100);
             o.sim = new Simulation();
         }
                   
    }
    
}
