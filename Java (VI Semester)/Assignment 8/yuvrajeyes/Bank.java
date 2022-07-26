package yuvrajeyes;

import java.util.ArrayList;
import java.util.List;

public class Bank {

    private final String name;
    private final List<Branch> branches = new ArrayList<>();

    public Bank(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public List<Branch> getBranches() {
        return branches;
    }

    private Bank.Branch findBranch(String branchName) {
        for (Bank.Branch branch : this.branches) {
            if (branch.getName().equalsIgnoreCase(branchName)) {
                return branch;
            }
        }
        return null;
    }

    public void addBranch(String branchName) {
        if (findBranch(branchName) == null) {
            this.branches.add(new Branch(branchName));
        } else {
            System.out.println("Branch (" + branchName + ") already exists.");
        }
    }

    public void addBranch(Bank.Branch branch) {
        if (findBranch(branch.getName()) == null) {
            this.branches.add(branch);
        } else {
            System.out.println("Branch (" + branch.getName() + ") already exists.");
        }
    }

    public void addCustomer(String branchName, String customerName, int customerAccountNumber, double amount) {
        Bank.Branch existingBranch = findBranch(branchName);
        if (existingBranch != null) {
            Bank.Branch.Customer customer = new Branch.Customer(customerName, customerAccountNumber, amount);
            existingBranch.addNewCustomer(customer);
        } else {
            System.out.println("A/C creation is unsuccessful. Branch " + branchName + " doesn't exists.");
        }
    }

    public void addCustomer(String branchName, Bank.Branch.Customer customer) {
        Bank.Branch existingBranch = findBranch(branchName);
        if (existingBranch != null) {
            existingBranch.addNewCustomer(customer);
            System.out.println("A/C created successfully.");
        } else {
            System.out.println("A/C creation is unsuccessful. " + branchName + "not exists.");
        }
    }

    public void getCustomersList(boolean... showTransactions) {
        for (Bank.Branch branch : this.branches) {
            getCustomersListByBranch(branch.getName(), showTransactions.length > 0 && (showTransactions[0]));
        }
    }

    public void getCustomersListByCustomerName(String customerName, boolean... showTransactions) {
        for (Bank.Branch branch : this.branches) {
            branch.getCustomersList(customerName, showTransactions.length > 0 && (showTransactions[0]));
        }
    }

    public void getCustomersListByBranch(String branchName, boolean... showTransactions) {
        Bank.Branch branch = findBranch(branchName);
        if (branch != null) {
            branch.getCustomersList(showTransactions.length > 0 && (showTransactions[0]));
        }
    }

    private Bank.Branch.Customer findCustomer(int customerAccountNumber) {
        for (Bank.Branch branch : this.branches) {
            for (Branch.Customer customer : branch.getCustomers()) {
                if (customer.getAccountNumber() == customerAccountNumber) {
                    return customer;
                }
            }
        }
        return null;
    }

    public void getTransactions(int customerAccountNumber) {
        Bank.Branch.Customer existingCustomer = findCustomer(customerAccountNumber);
        if (existingCustomer != null) {
            existingCustomer.getTransactionsDetails();
        } else {
            System.out.println("\nCustomer not found with the A/C Number(" + customerAccountNumber + ")");
        }
    }


    public static class Branch {

        private final String name;
        private final List<Customer> customers;

        public Branch(String name) {
            this.name = name.toUpperCase();
            this.customers = new ArrayList<>();
        }

        public String getName() {
            return name;
        }

        public List<Customer> getCustomers() {
            return customers;
        }

        public void getCustomersList(boolean... showTransactions) {
            int noOfCustomers = 0;
            System.out.println("\nCustomers in branch (" + getName() + ")");
            String details = "%1$-20s %2$-20s %3$-20s\n";
            System.out.format(details, "Name", "A/C Number", "Balance");
            for (Branch.Customer customer : this.customers) {
                System.out.format(details, customer.getName(), customer.getAccountNumber(), customer.getBalance());
                if (showTransactions.length > 0 && (showTransactions[0])) {
                    System.out.println("Transactions details are.....");
                    getTransactions(customer.getAccountNumber());
                    System.out.println();
                }
                noOfCustomers++;
            }
            if (noOfCustomers == 0) {
                System.out.println("There is no customer.");
            }
        }

        public void getCustomersList(String customerName, boolean... showTransactions) {
            int noOfCustomers = 0;
            System.out.println("\nName of the branch (" + getName() + ")");
            String details = "%1$-20s %2$-20s %3$-20s\n";
            System.out.format(details, "Name", "A/C Number", "Balance");
            for (Branch.Customer customer : this.customers) {
                if (customer.getName().equalsIgnoreCase(customerName)) {
                    System.out.format(details, customer.getName(), customer.getAccountNumber(), customer.getBalance());
                    if (showTransactions.length > 0 && (showTransactions[0])) {
                        getTransactions(customer.getAccountNumber());
                        System.out.println();
                    }
                    noOfCustomers++;
                }
            }
            if (noOfCustomers == 0) {
                System.out.println("There is no customer.");
            }
        }

        private Branch.Customer findCustomer(int customerAccountNumber) {
            for (Branch.Customer checkedCustomer : this.customers) {
                if (checkedCustomer.getAccountNumber() == customerAccountNumber) {
                    return checkedCustomer;
                }
            }
            return null;
        }

        public void addNewCustomer(String customerName, int customerAccountNumber, double initialAmount) {
            if (findCustomer(customerAccountNumber) == null) {
                Branch.Customer customer = new Customer(customerName, customerAccountNumber, initialAmount);
                addNewCustomer(customer);
            } else {
                System.out.println("Customer with A/C Number " + customerAccountNumber + " is already exists.");
            }

        }

        public void addNewCustomer(Branch.Customer customer) {
            if (findCustomer(customer.getAccountNumber()) == null) {
                this.customers.add(customer);
                System.out.println("Customer added successfully.");
            } else {
                System.out.println("Customer with A/C Number " + customer.getAccountNumber() + " is already exists.");
            }

        }

        public void getTransactions(int customerAccountNumber) {
            Branch.Customer existingCustomer = findCustomer(customerAccountNumber);
            if (existingCustomer != null) {
                existingCustomer.getTransactionsDetails();
            } else {
                System.out.println("\nCustomer not found with the A/C Number(" + customerAccountNumber + ")");
            }
        }

        public void withdraw(int customerAccountNumber, double amount) {
            Branch.Customer existingCustomer = findCustomer(customerAccountNumber);
            if (existingCustomer != null) {
                existingCustomer.withdraw(amount);
            } else {
                System.out.println("\nCustomer not found with the A/C Number(" + customerAccountNumber + ")");
            }
        }

        public void deposit(int customerAccountNumber, double amount) {
            Branch.Customer existingCustomer = findCustomer(customerAccountNumber);
            if (existingCustomer != null) {
                existingCustomer.deposit(amount);
            } else {
                System.out.println("\nCustomer not found with the A/C Number(" + customerAccountNumber + ")");
            }
        }


        public static class Customer {

            private final String name;
            private final int AccountNumber;
            private final List<Double> openingBalance = new ArrayList<>();
            private final List<Double> closingBalance = new ArrayList<>();
            private double balance;
            private final List<Transaction> transactions = new ArrayList<>();

            public Customer(String name, int AccountNumber, double initialAmount) {
                this.name = name.toUpperCase();
                this.AccountNumber = AccountNumber;
                this.balance = initialAmount;
                this.openingBalance.add(0.);
                this.closingBalance.add(initialAmount);
                this.transactions.add(new Transaction(initialAmount));
            }

            public String getName() {
                return name;
            }

            public int getAccountNumber() {
                return AccountNumber;
            }

            public double getBalance() {
                return balance;
            }

            public List<Transaction> getTransactions() {
                return transactions;
            }

            public void deposit(double amount) {
                this.transactions.add(new Transaction(amount, "deposit"));
                this.openingBalance.add(this.balance);
                this.balance += amount;
                this.closingBalance.add(this.balance);
                System.out.println(amount + " deposited to A/C Number " + this.getAccountNumber() + ".");
            }

            public void withdraw(double amount) {
                if (amount <= this.balance) {
                    this.transactions.add(new Transaction(amount, "withdraw"));
                    this.openingBalance.add(this.balance);
                    this.balance -= amount;
                    this.closingBalance.add(this.balance);
                    System.out.println(amount + " withdrawn from A/C Number " + this.getAccountNumber() + ".");
                } else {
                    System.out.println("Transaction unsuccessful. Available balance is " + this.balance +
                            " but transaction amount was " + amount + ".");
                }
            }

            public void getTransactionsDetails() {
                String details = "%1$-20s %2$-20s %3$-20s %4$-20s\n";
                System.out.format(details, "Opening Balance", "Transaction Mode", "Transaction Amount", "Closing Balance");
                int i = 0;
                while (i < this.transactions.size()) {
                    System.out.format(details,
                            this.openingBalance.get(i),
                            this.transactions.get(i).getMode(),
                            this.transactions.get(i).getAmount(),
                            this.closingBalance.get(i));
                    i++;
                }
                if (i == 0) {
                    System.out.println("No transactions were made.");
                }
            }


            public static class Transaction {

                private final double amount;
                private final String mode;

                public Transaction(double amount) {
                    this.amount = amount;
                    this.mode = "deposit";
                }

                public Transaction(double amount, String mode) {
                    this.mode = mode;
                    this.amount = amount;
                }

                public double getAmount() {
                    return amount;
                }

                public String getMode() {
                    return mode;
                }
            }
        }
    }
}
