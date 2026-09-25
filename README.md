# CSE4110 Database Systems

**Semester:** Spring 2025  

This repository contains two course projects for **CSE4110 Database Systems**.  
The projects follow the database development process from **conceptual modeling** to **relational design, normalization, physical implementation, and application development**.

---

## Projects

### Project 1 — E-R Diagram Design

Project 1 focuses on the **conceptual design of a convenience-store chain database** using the Entity-Relationship (E-R) model.

The system models major business components such as:

- Stores
- Products
- Vendors
- Inventory
- Customers
- Sales transactions
- Membership and purchase relationships

The E-R model was designed to support business questions such as:

- Which stores carry a specific product, and how much inventory do they have?
- Which products have the highest sales volume?
- Which store generates the highest revenue?
- Which vendors supply the most products?
- Which products require restocking?
- What products are commonly purchased together by loyalty customers?
- How do franchise and corporate stores compare in product variety?

Project 1 is a **conceptual-design project only**; no database implementation or SQL code is required.

For the full design assumptions, entity/relationship descriptions, cardinalities, and query coverage, see:

[**Project 1 README**]([./Project1/README.md](https://github.com/Deli0926/CSE4110-Database_Systems/blob/main/Project%201/README.md))

---

### Project 2 — Database Implementation and Application Development

Project 2 extends the E-R design from Project 1 into a complete relational database implementation.

The project includes:

- E-R model to relational-schema transformation
- Functional dependency analysis
- BCNF normalization
- Logical schema design
- Physical schema design
- MySQL database creation
- Sample-data generation
- C++ application development
- Implementation of all seven Project 1 sample queries

The final database consists of the following relations:

- `Customer`
- `Store`
- `Member`
- `Sales_Transaction`
- `Product`
- `Include`
- `Inventory`
- `Vendor`
- `Supply`

A menu-driven C++ application connects to MySQL through the **MySQL C API** and allows the user to execute the implemented queries interactively.

For schema details, normalization, build instructions, SQL setup, and query descriptions, see:

[**Project 2 README**](./Project2/README.md)

---

## Project Progression

The two projects are designed as a continuous workflow:

```text
Project 1
Conceptual E-R Design
        ↓
Entity / Relationship Modeling
        ↓
Cardinality & Participation Constraints
        ↓
Project 2
Relational Schema Reduction
        ↓
BCNF Normalization
        ↓
Physical Schema Design
        ↓
MySQL Implementation
        ↓
C++ Database Application
```

Project 1 defines **what information the system should represent**, while Project 2 determines **how that model is normalized, implemented, populated, and queried in an actual database system**.

---

## Repository Structure

```text
.
├── Project1/
│   ├── README.md
│   ├── ...
│
├── Project2/
│   ├── README.md
│   ├── logical_schema.png
│   ├── physical_schema.png
│   ├── project_report.pdf
│   ├── schema.sql
│   ├── sample_data.sql
│   ├── main.cpp
│   └── ...
│
└── README.md
```

---

## Technologies

Project 1 is centered on conceptual database modeling, while Project 2 uses the following implementation environment:

- **Database:** MySQL 8.0
- **Design Tool:** MySQL Workbench
- **Language:** C++
- **Database API:** MySQL C API
- **Compiler:** MSVC
- **Development Environment:** Windows 11 / Visual Studio Code

---

## Summary

Together, these projects cover the major stages of relational database development:

1. Requirements analysis
2. Conceptual E-R modeling
3. Relational-schema design
4. Functional dependency analysis
5. BCNF normalization
6. Physical schema design
7. Database implementation
8. Sample-data population
9. SQL query development
10. Database application development

The repository therefore documents the progression from a high-level convenience-store data model to a functioning MySQL-backed C++ application.
