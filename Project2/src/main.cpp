#include <iostream>
#include <string>
#include <iomanip>
#include <mysql.h>

void displayMenu();
void executeQuery(MYSQL* conn, MYSQL_RES* &res, MYSQL_ROW &row, const char* query);

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "1234"; // 여기에 비밀번호 입력
    const char *database = "store"; // 여기에 데이터베이스 이름 입력

    // MySQL 초기화
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed\n";
        return 1;
    }
    
    // 1) Create a local variable whose value is SSL_MODE_DISABLED
    mysql_ssl_mode sslmode = SSL_MODE_DISABLED;
    // 2) Pass its address to mysql_options()
    if (mysql_options(conn, MYSQL_OPT_SSL_MODE, &sslmode)) {
        // mysql_options() returns nonzero on error
        std::cerr << "mysql_options() failed: " << mysql_error(conn) << "\\n";
        mysql_close(conn);
        return 1;
    }

    // MySQL 서버 연결
    if (mysql_real_connect(conn, server, user, password, database, 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return 1;
    }
    
    bool run = true;
    while (run) {
        int choice;

        displayMenu();
        std::cout << "Select: ";
        std::cin >> choice;

        std::string search, query;
        switch (choice) {
            case 1: {
                std::cout << "------ TYPE 1 ------\n"
                << "** which stores currently carry a certain product (by UPC, name, or brand), "
                << "and how much inventory do they have? **\n"
                << "\n";
                std::string upc, name, brand;
                std::cout << "Enter product identifier (UPC, name, or brand): ";
                std::cin.ignore();
                std::getline(std::cin, search);
                query =
                    "SELECT s.store_id, s.store_name, p.UPC, p.product_name, p.brand, i.quantity "
                    "FROM Inventory i "
                    "JOIN Store s ON i.store_id = s.store_id "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "WHERE i.quantity > 0 AND (p.UPC = '" + search + "' OR LOWER(p.product_name) = '" + search + "' OR LOWER(p.brand) = '" + search + "')";
                executeQuery(conn, res, row, query.c_str());
                break;
            }
            case 2:
                std::cout << "------ TYPE 2 ------\n"
                << "** which products have the highest sales volume in each store over the past month? **\n"
                << "\n";
                query =
                    "WITH sales_summary AS (SELECT s.store_id, s.store_name, p.UPC, p.product_name, SUM(i.amount) AS total_amount "
                    "FROM Include i "
                    "JOIN Sales_Transaction st ON i.transaction_id = st.transaction_id "
                    "JOIN Store s ON st.store_id = s.store_id "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "WHERE st.datetime >= CURRENT_DATE - INTERVAL 30 DAY "
                    "GROUP BY s.store_id, s.store_name, p.UPC, p.product_name), "
                    "store_max AS (SELECT store_id, MAX(total_amount) AS max_amount FROM sales_summary GROUP BY store_id) "
                    "SELECT s.* FROM sales_summary s JOIN store_max m ON s.store_id = m.store_id AND s.total_amount = m.max_amount";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 3:
                std::cout << "------ TYPE 3 ------\n"
                << "** which store has generated the highest overall revenue this quarter? **\n"
                << "\n";
                query = 
                    "SELECT st.store_id, s.store_name, SUM(p.price * i.amount) AS overall_revenue "
                    "FROM Sales_Transaction st "
                    "JOIN Include i ON st.transaction_id = i.transaction_id "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "JOIN Store s ON st.store_id = s.store_id "
                    "WHERE QUARTER(st.datetime) = QUARTER(CURRENT_DATE) AND YEAR(st.datetime) = YEAR(CURRENT_DATE) "
                    "GROUP BY st.store_id, s.store_name "
                    "ORDER BY overall_revenue DESC "
                    "LIMIT 1";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 4:
                std::cout << "------ TYPE 4 ------\n"
                << "** which vendor supplies the most products across the chain, "
                << "and how many total units have been sold? **\n"
                << "\n";
                query =
                    "SELECT v.vendor_id, v.vendor_name, COUNT(DISTINCT s.UPC) AS product_count, SUM(i.amount) AS total_units_sold "
                    "FROM Vendor v "
                    "JOIN Supply s ON v.vendor_id = s.vendor_id "
                    "LEFT JOIN Include i ON s.UPC = i.UPC "
                    "GROUP BY v.vendor_id, v.vendor_name "
                    "ORDER BY product_count DESC "
                    "LIMIT 1";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 5:
                std::cout << "------ TYPE 5 ------\n"
                << "** which products in each store are below the reorder threshold and need restocking? **\n"
                << "\n";
                query = 
                    "SELECT s.store_id, s.store_name, p.UPC, p.product_name, i.quantity, i.reorder_threshold "
                    "FROM Inventory i "
                    "JOIN Store s ON i.store_id = s.store_id "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "WHERE i.quantity < i.reorder_threshold";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 6:
                std::cout << "------ TYPE 6 ------\n"
                << "** List the top 3 items that loyalty program customers typically purchase with coffee. **\n"
                << "Enter a product name: ";
                std::cin.ignore();
                std::getline(std::cin, search);
                query =
                    "WITH Purchase AS (SELECT DISTINCT st.transaction_id "
                    "FROM Sales_Transaction st "
                    "JOIN Include i ON st.transaction_id = i.transaction_id "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "JOIN Member m ON st.customer_id = m.customer_id "
                    "WHERE p.product_type = '" + search + "'), "
                    "WithPurchase AS (SELECT i.UPC, p.product_name, SUM(i.amount) AS total_sold "
                    "FROM Include i "
                    "JOIN Product p ON i.UPC = p.UPC "
                    "WHERE i.transaction_id IN (SELECT transaction_id FROM Purchase) AND p.product_type != '" + search + "' "
                    "GROUP BY i.UPC, p.product_name) "
                    "SELECT w.UPC, w.product_name, w.total_sold "
                    "FROM WithPurchase w "
                    "ORDER BY total_sold DESC "
                    "LIMIT 3";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 7:
                std::cout << "------ TYPE 7 ------\n"
                << "** Among franchise-owned stores, which one offers the widest variety of products, "
                << "and how does that compare to corporate-owned stores? **\n"
                << "\n";
                query = 
                    "WITH Franchise AS (SELECT s.store_id, s.store_name, COUNT(DISTINCT i.UPC) AS variety "
                    "FROM Store s JOIN Inventory i ON s.store_id = i.store_id "
                    "WHERE s.ownership_type = 'Franchise' "
                    "GROUP BY s.store_id, s.store_name "
                    "ORDER BY variety DESC "
                    "LIMIT 1), "
                    "Corporate AS (SELECT s.store_id, s.store_name, COUNT(DISTINCT i.UPC) AS variety "
                    "FROM Store s JOIN Inventory i ON s.store_id = i.store_id "
                    "WHERE s.ownership_type = 'Corporate' "
                    "GROUP BY s.store_id, s.store_name "
                    "ORDER BY variety DESC "
                    "LIMIT 1) "
                    "SELECT f.store_id AS F_store_id, f.store_name AS F_store_name, f.variety AS F_variety, "
                    "c.store_id AS C_store_id, c.store_name AS C_store_name, c.variety AS C_variety "
                    "FROM Franchise f, Corporate c";
                executeQuery(conn, res, row, query.c_str());
                break;
            case 0:
                std::cout << "Program terminated\n";
                run = false;
                break;
            default:
                std::cout << "Wrong Type\n";
                break;
        }
    }

    // 리소스 해제
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}

void displayMenu() {
    std::cout << "---------- SELECT QUERY TYPES ----------\n";
    for (int i = 0; i < 7; i++) {
        std::cout << (i + 1) << ". TYPE " << (i + 1) << "\n";
    }
    std::cout << "0. QUIT\n";
}

void executeQuery(MYSQL* conn, MYSQL_RES* &res, MYSQL_ROW &row, const char* query) {
    // 쿼리 실행
    if (mysql_query(conn, query)) {
        std::cerr << "SELECT failed. Error: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return;
    }

    // 결과 저장
    res = mysql_store_result(conn);
    if (res == nullptr) {
        std::cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return;
    }

    // 필드 개수 가져오기
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);

    // 헤더 출력
    for (int i = 0; i < num_fields; i++) {
        std::cout << std::setw(20) << std::left << fields[i].name << "\t";
    }
    std::cout << "\n";

    // 행 출력
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            std::cout << std::setw(20) << std::left << (row[i] ? row[i] : "NULL") << "\t";
        }
        std::cout << "\n";
    }
}