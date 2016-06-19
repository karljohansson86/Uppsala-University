public class CustomerQuick extends Customer {

    public CustomerQuick(int startTime, int amountOfGroceries) {
        super(startTime, amountOfGroceries);
    }

    @Override
    public void serve() {

        int groceries = this.getGroceries();
        if (groceries < 1) {
            return;
        } else if (groceries == 1) {
            this.setGroceries(groceries - 1);
        } else {
            this.setGroceries(groceries - 2);
        }
        
    };

    @Override
    public String toString() {
        return "♞ ";
    }

}
