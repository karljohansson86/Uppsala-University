public class CustomerSlow extends Customer {

    private boolean shouldBeServed = false;
    
    public CustomerSlow(int startTime, int amountOfGroceries) {
        super(startTime, amountOfGroceries);
    }

    @Override
    public void serve() {

        if (this.shouldBeServed) { super.serve(); }
        this.shouldBeServed = !this.shouldBeServed;
        
    };

    @Override
    public String toString() {
        return "♚ ";
    }

}
