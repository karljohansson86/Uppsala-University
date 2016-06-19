public class CustomerRegular extends Customer {

    public CustomerRegular(int startTime, int amountOfGroceries) {
        super(startTime, amountOfGroceries);
    }

    public String toString() {
        return "♟ "; // + this.groceries;
    }

}
