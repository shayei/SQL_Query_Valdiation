
<img src="https://banner2.cleanpng.com/20180705/yke/kisspng-sql-database-computer-icons-download-sql-icon-5b3ed4c9cc9013.8225606715308443618379.jpg" width="600" height="350">

# SQL Query Syntax Checker


```
The program returns for each query whether it is valid or not (valid, Invalid) only.
```

For example, two schemas are attached to the file:

> Customers(Name:STRING, Age:INTEGER)

> Orders(CustomerName:STRING, Product:STRING, Price: INTEGRER)


If the query is invalid, it is necessary to report where it failed.
The program shoud return one of the following four options:
1. Parsing <o_d> failed
2. Parsing <attribute_list> failed
3. Parsing <table_list> failed
4. Parsing <condition> failed


Sample valid input queries:

> SELECT Customers.Name FROM Customers WHERE Customer.Age=25;

> SELECT Customers.Name FROM Customers WHERE Customer.Name=”Mike”;

> SELECT DISTINCT Customers.Name FROM Customers WHERE Customer.Age=25;

> SELECT Customers.Name,Orders.Price FROM Customers,Orders WHERE Customer.Name=Orders.CustomerName;

> SELECT Customers.CustomerName,Orders.Price FROM Customers,Orders WHERE Customer.Name=Orders.CustomerName AND Orders.Price>1000;

> SELECT Customers.Name,Orders.Price FROM Customers,Orders WHERE (Customer.Name=Orders.CustomerName) AND Orders.Price>1000;

> SELECT Customers.Name,Orders.Price FROM Customers,Orders WHERE (Customer.Name=Orders.CustomerName) OR (Orders.Price>59);

### ENJOY
