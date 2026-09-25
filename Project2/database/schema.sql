drop table Supply;
drop table Inventory;
drop table Include;
drop table Sales_Transaction;
drop table Member;
drop table Store;
drop table Customer;
drop table Product;
drop table Vendor;

create table Customer (
    customer_id     char(5) not null,
    customer_name   varchar(20) not null,
    phone           char(11) unique not null,
    email           varchar(50) unique not null,
    primary key (customer_id));

create table Store (
    store_id        char(5) not null,
    store_name      varchar(20) not null,
    street_name     varchar(20) not null,
    street_number   int not null,
    open_time       time not null,
    close_time      time not null,
    ownership_type  varchar(20) not null,
    primary key (store_id));

create table Member (
    customer_id     char(5) not null,
    store_id        char(5) not null,
    point           int,
    primary key (customer_id, store_id),
	foreign key (customer_id) references Customer(customer_id),
    foreign key (store_id) references Store(store_id),
    check (point >= 0));

create table Sales_Transaction (
    transaction_id      char(5) not null,
    customer_id         char(5) not null,
    store_id            char(5) not null,
    datetime            datetime not null,
    payment_method      varchar(20) not null,
    member_discount     int,
    primary key (transaction_id),
    foreign key (customer_id) references Customer(customer_id),
    foreign key (store_id) references Store(store_id));

create table Product (
    UPC             char(5) not null,
    product_name    varchar(20) not null,
    brand           varchar(20) not null,
    product_type    varchar(20) not null,
    weight          numeric(10, 2) not null,
    price           numeric(10,2) not null,
    refund_period   int not null,
    primary key (UPC),
	check (weight >= 0),
    check (refund_period >= 0));

create table Include (
    transaction_id  char(5) not null,
    UPC             char(5) not null,
    amount          int not null,
    item_discount   numeric(10,2),
    primary key (transaction_id, UPC),
    foreign key (transaction_id) references Sales_Transaction(transaction_id),
    foreign key (UPC) references Product(UPC));

create table Inventory (
    UPC                 char(5) not null,
    store_id            char(5) not null,
    quantity            int not null,
    reorder_threshold   int not null,
    reorder_quantity    int not null,
    recent_reorder      datetime not null,
    primary key (UPC, store_id),
    foreign key (UPC) references Product(UPC),
    foreign key (store_id) references Store(store_id));

create table Vendor (
    vendor_id       char(5) not null,
    vendor_name     varchar(20) not null,
    contact_info    char(11) not null,
    primary key (vendor_id));

create table Supply (
    UPC         char(5) not null,
    vendor_id   char(5) not null,
    primary key (UPC, vendor_id),
    foreign key (UPC) references Product(UPC),
    foreign key (vendor_id) references Vendor(vendor_id));