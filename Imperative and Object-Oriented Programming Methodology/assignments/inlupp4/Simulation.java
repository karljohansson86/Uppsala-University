import java.util.Random;
import java.util.ArrayList;
import java.util.Iterator;


public class Simulation{
    
    private Store store;
    private int time = 0;
    
    private int intensity = 75;
    private int maxGroceries = 5;
    private int thresholdForNewRegister = 3;

    private int customersServed = 0;
    private int maxWaitTime = 0;
    private int waitTime = 0;


    public Simulation() {

         String sizeQ = "How many registers";
         String treshQ = "Average queue length for opening another register";
         String maxGrocQ = "Maximum grocery amount for a customer";
         String intensityQ = "Customer spawn rate (%)";
                      
         int size = Asker.questionInt("(2/5) " + sizeQ, 1, 8);
         this.store = new Store(size);
         
         this.thresholdForNewRegister =
             Asker.questionInt("(3/5) " + treshQ, 1, 9);
         
         this.maxGroceries = Asker.questionInt("(4/5) " + maxGrocQ, 1, 9);
         this.intensity = Asker.questionInt("(5/5) " + intensityQ, 1, 100);          
        
    }

    public Simulation(Boolean defaultMode) {
	this.store = new Store(4);
    }

    public Simulation(Random random) {
	this.store = new Store(4);
        this.intensity = random.nextInt(90) + 10;
        this.maxGroceries = random.nextInt(8) + 1;
        this.thresholdForNewRegister =
            (random.nextInt(this.intensity + this.maxGroceries) / 11) + 1;
    
    }
    
    
    public void startSim() {
        this.store.openStore();
    }

    public void step() {
        
	this.time = this.time + 1;
	this.store.step();
        
	ArrayList<Customer> served = this.store.getDoneCustomers();
        this.store.keepOneRegisterOpen();
	this.gatherInfo(served);
        
	createRandomCustomer();
        
	int len = this.store.getAverageQueueLength();
	if (len > thresholdForNewRegister) {
	    this.store.openNewRegister();
	}	
	
    }
    
    private void createRandomCustomer() {
	Random random = new Random();
	int randomInt = random.nextInt(100);
        
	if (randomInt < this.intensity) {

            int randomGroc = random.nextInt(this.maxGroceries) + 1;
            int customerType = random.nextInt(3);
            Customer c = null;
            
            switch (customerType) {
            case 0:
                c = new CustomerRegular(this.time, randomGroc);
                break;
            case 1:
                c = new CustomerQuick(this.time, randomGroc);
                break;
            case 2:
                c = new CustomerSlow(this.time, randomGroc);
                break;
            default:
                break;
            }
            
	    this.store.newCustomer(c);
	}
    }
    

    private void gatherInfo(ArrayList<Customer> served) {
        
        Iterator<Customer> iterator = served.iterator();
        Customer c = null;
        
        while (iterator.hasNext()) {
	    c = iterator.next();
	    this.customersServed++;

            int lifeTime = this.time - c.getBornTime();
	    this.waitTime = this.waitTime + lifeTime;

            if (this.maxWaitTime < lifeTime) {
		this.maxWaitTime = lifeTime;
	    }
	}
    }

    
    public double getAverageWaitTime() {
	return (this.customersServed == 0) ?
            0 : (this.waitTime / (double) this.customersServed);
    }


    public String toString() {

        String s1 = "Number of customers served: " + this.customersServed;
        String s2 = "\nMax wait-time: " + this.maxWaitTime;
        String roundDouble = String.format("%.2f", this.getAverageWaitTime());
        String s3 = "\nAverage wait-time: " + roundDouble;
        String s4 =
            "\n\nIntensity: " + this.intensity +
            ", Max groceries: " + this.maxGroceries +
            ", Threshold: " + this.thresholdForNewRegister;

        return this.store + s1 + s2 + s3 + s4;

    }

}
