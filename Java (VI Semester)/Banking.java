import yuvrajeyes.Bank;

public class Banking {

    public static void main(String[] args) {

        Bank bank = new Bank("SBI");
        Bank.Branch branch = new Bank.Branch("BLP");
        bank.addBranch(branch);

        branch.addNewCustomer("Yuvraj", 123456, 800);

        bank.addCustomer("BLP", "Gopal", 12345, 0.);

        branch.addNewCustomer("Yuvraj", 78956, 100.);

        branch.deposit(123456, 2000.);
        branch.withdraw(123456, 300.);

        Bank.Branch.Customer customer1 = new Bank.Branch.Customer("Yuvraj", 123456, 0.);
        branch.addNewCustomer(customer1);

        Bank.Branch.Customer customer2 = new Bank.Branch.Customer("Singh", 789456, 0.);
        bank.addCustomer("BLP", customer2);

        branch.getCustomersList();
        System.out.println();
        branch.getCustomersList("Yuvraj");
        System.out.println();
        branch.getCustomersList(true);
        System.out.println();

        System.out.println("...........");
        bank.getCustomersList();
        System.out.println("...........");
        bank.addBranch("LKO");
        System.out.println("...........");
        bank.getCustomersListByCustomerName("Gopal");
    }
}
